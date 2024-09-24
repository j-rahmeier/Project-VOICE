////////////////////////////////////////////////////////////////////////////////
// $Id: VisPropertyTypes.cpp 7843 2024-02-16 15:08:37Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: VisProperty-related object types for the script interpreter.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "VisPropertyTypes.h"

#include "BCIException.h"
#include "BCI_OperatorLib.h"
#include "CommandInterpreter.h"
#include "StateMachine.h"

#define PROPERTY_SETS_PARAM "VisPropertySets"

namespace Interpreter
{

//// VisPropertyType
VisPropertyType VisPropertyType::sInstance;
const ObjectType::MethodEntry VisPropertyType::sMethodTable[] = 
    {METHOD(Set), METHOD(Get), METHOD(Reset), END};

bool VisPropertyType::Set(CommandInterpreter &inInterpreter)
{
    std::string visID, cfgID;
    IDType numCfgID;
    GetVisPropertyIDs(inInterpreter, visID, cfgID, numCfgID);
    std::string value = inInterpreter.GetRemainingTokens();
    WithLock(inInterpreter.StateMachine())
        inInterpreter.StateMachine().Visualizations()[visID].Put(numCfgID, value);
    inInterpreter.StateMachine().ExecuteCallback(BCI_OnVisProperty, visID.c_str(), numCfgID, value.c_str());
    inInterpreter.Log() << "Set vis property " << visID << "." << cfgID << " to " << value;
    return true;
}

bool VisPropertyType::Get(CommandInterpreter &inInterpreter)
{
    std::string visID, cfgID;
    IDType numCfgID;
    GetVisPropertyIDs(inInterpreter, visID, cfgID, numCfgID);
    WithLock(inInterpreter.StateMachine())
    {
        std::string result;
        inInterpreter.StateMachine().Visualizations()[visID].Get(numCfgID, result);
        inInterpreter.Out() << result;
    }
    return true;
}

bool VisPropertyType::Reset(CommandInterpreter& inInterpreter)
{
    std::string visID, cfgID;
    IDType numCfgID;
    GetVisPropertyIDs(inInterpreter, visID, cfgID, numCfgID);
    std::string value = inInterpreter.GetRemainingTokens();
    if (!value.empty())
        throw bciexception << "Unexpected token: " << value;
    WithLock(inInterpreter.StateMachine())
        inInterpreter.StateMachine().Visualizations()[visID].Erase(numCfgID);
    inInterpreter.StateMachine().ExecuteCallback(BCI_OnVisProperty, visID.c_str(), numCfgID, nullptr);
    inInterpreter.Log() << "Reset vis property " << visID << "." << cfgID;
    return true;
}

void VisPropertyType::GetVisPropertyIDs(CommandInterpreter &inInterpreter, std::string &outVisID, std::string &outCfgID,
                                        IDType &outNumCfgID)
{
    std::string visIDcfgID = inInterpreter.GetToken();
    std::istringstream iss(visIDcfgID);
    std::getline(iss, outVisID, '.');
    std::getline(iss, outCfgID);
    if (outVisID.empty() || outCfgID.empty())
        throw bciexception << "Expected <visID>.<configID> pair";
    outNumCfgID = CfgID(outCfgID);
    if (outNumCfgID == static_cast<IDType>(CfgID::None))
        throw bciexception << "Invalid config ID " << outCfgID;
}

//// VisPropertiesType
VisPropertiesType VisPropertiesType::sInstance;
const ObjectType::MethodEntry VisPropertiesType::sMethodTable[] = {METHOD(Set), END};

void VisPropertiesType::OnInitialize(StateMachine &s) const
{
    if (!s.Parameters().Exists("/" PROPERTY_SETS_PARAM))
    {
        Param p =
            Param::fromDefinition("Visualize:Property%20Sets matrix /" PROPERTY_SETS_PARAM "= 0 1 % % % "
                                  "// row titles are properties in the form \"SRCD.Left\", columns are property sets");
        s.Parameters().ByPath("/" PROPERTY_SETS_PARAM) = p;
    }
}

bool VisPropertiesType::Set(CommandInterpreter &inInterpreter)
{
    Param p;
    WithLock(inInterpreter.StateMachine())
    {
        const std::string paramName = PROPERTY_SETS_PARAM;
        const ParamList &parameters = inInterpreter.StateMachine().Parameters();
        if (!parameters.Exists("/" + paramName))
            return false;
        Param p = parameters.ByPath("/" + paramName);
    }
    std::string setID = inInterpreter.GetRemainingTokens();
    if (!p.ColumnLabels().Exists(setID))
        throw bciexception << "Invalid vis property set ID: " << setID;
    int col = p.ColumnLabels()[setID];
    for (int row = 0; row < p.NumRows(); ++row)
    {
        if (!p.Value(row, col).AsString().Empty())
        {
            std::istringstream iss(p.RowLabels()[row]);
            std::string visID_enc, cfgID;
            std::getline(iss >> std::ws, visID_enc, '.');
            iss >> cfgID;
            std::istringstream iss2(visID_enc);
            EncodedString visID;
            iss2 >> visID;
            IDType numCfgID = CfgID(cfgID);
            if (numCfgID != static_cast<IDType>(CfgID::None))
            {
                std::string value = p.Value(row, col).ToString();
                WithLock(inInterpreter.StateMachine()) inInterpreter.StateMachine().Visualizations()[visID].Put(
                    numCfgID, value);
                inInterpreter.StateMachine().ExecuteCallback(BCI_OnVisProperty, visID.c_str(), numCfgID, value.c_str());
            }
        }
    }
    inInterpreter.Log() << "Applied vis property set " << setID;
    return true;
}

} // namespace Interpreter
