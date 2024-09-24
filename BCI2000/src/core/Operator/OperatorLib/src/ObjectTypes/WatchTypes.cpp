////////////////////////////////////////////////////////////////////////////////
// $Id: WatchTypes.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Watch types for the script interpreter.
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
#include "WatchTypes.h"

#include "BCIException.h"
#include "StateMachine.h"
#include "Watches.h"

namespace Interpreter
{

// WatchType
WatchType WatchType::sInstance;
const ObjectType::MethodEntry WatchType::sMethodTable[] = {
    METHOD(New),         {"Create", &New}, {"Add", &New},    METHOD(Delete),  {"Clear", &Delete},
    {"Remove", &Delete}, METHOD(Check),    {"Show", &Check}, METHOD(Trigger), END};

bool WatchType::New(CommandInterpreter &inInterpreter)
{
    return Create(inInterpreter, false);
}

bool WatchType::Create(CommandInterpreter &inInterpreter, bool inSingleToken)
{
    enum
    {
        none,
        expr,
        sysstate
    };
    int kind = none;
    static const struct
    {
        std::string pat;
        int kind;
    } kinds[] = {
        {"SYSTEM STATE", sysstate},
        {"SYSTEMSTATE", sysstate},
    };
    static const size_t nKinds = sizeof(kinds) / sizeof(*kinds);
    static const std::string addressClause = "AT \\(\\<*\\>\\)";

    std::vector<std::string> tokens;
    std::string token;
    if (inSingleToken)
    {
        std::string s;
        while (!inInterpreter.MatchRemainingTokens(addressClause) && !(s = inInterpreter.GetOptionalToken()).empty())
        {
            if (!token.empty())
                token += " ";
            token += s;
        }
        tokens.push_back(token);
        kind = expr;
    }
    else
    {
        bool match = (kind != none);
        for (size_t i = 0; !match && i < nKinds; ++i)
            if ((match = inInterpreter.MatchTokens(kinds[i].pat)))
            {
                tokens.push_back(inInterpreter.GetMatchingTokens(kinds[i].pat));
                kind = kinds[i].kind;
            }
        while (!inInterpreter.MatchRemainingTokens(addressClause) &&
               !(token = inInterpreter.GetOptionalToken()).empty())
            tokens.push_back(token);
    }
    if (kind == none)
        kind = expr;

    std::string address;
    std::vector<std::string> matches;
    if (inInterpreter.GetMatchingTokens(addressClause, matches))
        address = matches[1];
    else
        inInterpreter.Unget();

    Watch *pWatch = 0;
    if (kind == sysstate)
    {
        if (tokens.size() != 1)
            throw bciexception << "Unexpected additional arguments for SYSTEM STATE watch";
        pWatch = new SystemStateWatch(inInterpreter, address);
        token = " " + tokens[0];
    }
    else if (kind == expr)
    {
        int decimate = 1;
        if (tokens.size() > 1 && !::stricmp(tokens[0].c_str(), "Decimate")) {
            decimate = ::atoi(tokens[1].c_str());
            if (decimate < 1)
                throw bciexception << "Decimation is " << decimate << " must be >= 1";
            tokens.erase(tokens.begin());
            tokens.erase(tokens.begin());
        }
        if (tokens.empty())
            throw bciexception << "No watch expression given";
        token.clear();
        for (size_t j = 0; j < tokens.size(); ++j)
        {
            std::string &exp = tokens[j];
            std::string::iterator i = exp.begin();
            while (i != exp.end())
                if (::isspace(*i))
                    i = exp.erase(i);
                else
                    ++i;
            token.append(" ").append(exp);
        }
        ExpressionWatch *pExprWatch = new ExpressionWatch(inInterpreter, address);
        try
        {
            pExprWatch->SetExpressions(tokens);
            pExprWatch->SetDecimation(decimate);
            pWatch = pExprWatch;
        }
        catch (const bci::Exception &)
        {
            delete pExprWatch;
            throw;
        }
    }
    else
    {
        return false;
    }
    if (pWatch && !pWatch->Address().empty())
    {
        inInterpreter.Out() << pWatch->Address();
        pWatch->SetTag(token);
    }
    else
    {
        delete pWatch;
        throw bciexception << "Could not create watch" << (address.empty() ? "" : (" at address \"" + address + "\""));
    }
    return true;
}

bool WatchType::Delete(CommandInterpreter &inInterpreter)
{
    delete Find(inInterpreter).First();
    return true;
}

bool WatchType::Check(CommandInterpreter &inInterpreter)
{
    Watch::Set s = Find(inInterpreter);
    inInterpreter.Out() << s.First()->Tag() << "\n " << s.First()->Check();
    return true;
}

bool WatchType::Trigger(CommandInterpreter &inInterpreter)
{
    Watch::Set s = Find(inInterpreter);
    inInterpreter.Out() << s.First()->Tag() << "\n " << s.First()->Trigger();
    return true;
}

Watch::Set WatchType::Find(CommandInterpreter &inInterpreter)
{
    std::string addr = inInterpreter.GetToken();
    Watch::Set s = inInterpreter.StateMachine().Watches().SelectByAddress(addr);
    if (s.Empty())
        throw bciexception << "No watch associated with address \"" << addr << "\"";
    else if (s.Size() > 1)
        throw bciexception << s.Size() << " watches associated with address \"" << addr << "\"";
    return s;
}

// WatchesType
WatchesType WatchesType::sInstance;
const ObjectType::MethodEntry WatchesType::sMethodTable[] = {
    METHOD(Check), {"Update", &Check}, METHOD(Trigger),    METHOD(List),        {"Show", &List},
    METHOD(Count), METHOD(Delete),     {"Clear", &Delete}, {"Remove", &Delete}, END};

bool WatchesType::Check(CommandInterpreter &inInterpreter)
{
    Watch::Set s = Select(inInterpreter);
    for (Watch *p = s.First(); p; p = s.Next(p))
        p->Check();
    return true;
}

bool WatchesType::Trigger(CommandInterpreter &inInterpreter)
{
    Watch::Set s = Select(inInterpreter);
    for (Watch *p = s.First(); p; p = s.Next(p))
        p->Trigger();
    return true;
}

bool WatchesType::List(CommandInterpreter &inInterpreter)
{
    Watch::Set s = Select(inInterpreter);
    for (Watch *p = s.First(); p; p = s.Next(p)) {
        if (p->Address().empty())
            inInterpreter.Out() << "[" << p->ID() << "]";
        else
            inInterpreter.Out() << p->Address();
        inInterpreter.Out() << " ";
        auto pExprWatch = dynamic_cast<ExpressionWatch*>(p);
        if (pExprWatch) {
            std::vector<std::string> strings;
            auto exprs = pExprWatch->Expressions();
            for (const auto& expr : exprs)
                strings.push_back(expr.AsString());
            inInterpreter.Out() << "[" << StringUtils::Join(", ", strings) << "] ";
        }
        auto pStateWatch = dynamic_cast<SystemStateWatch*>(p);
        if (pStateWatch) {
            inInterpreter.Out() << "[SYSTEM STATE] ";
        }
        inInterpreter.Out() << "Decimation=" << p->Decimation() << "\n";
    }
    return true;
}

bool WatchesType::Count(CommandInterpreter &inInterpreter)
{
    inInterpreter.Out() << Select(inInterpreter).Size();
    return true;
}

bool WatchesType::Delete(CommandInterpreter &inInterpreter)
{
    Watch::Set s = Select(inInterpreter);
    for (Watch *p = s.First(); p; p = s.Next(p))
        delete p;
    return true;
}

Watch::Set WatchesType::Select(CommandInterpreter &inInterpreter)
{
    std::string token = inInterpreter.GetOptionalToken();
    const Watch::Set &watches = inInterpreter.StateMachine().Watches();
    return token.empty() ? watches : watches.SelectByAddress(token, Watch::Set::WildcardYes);
}

} // namespace Interpreter
