////////////////////////////////////////////////////////////////////////////////
// $Id$
// Description: A GenericFilter::Chain::FilterHook that implements default
//   visualization.
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
#include "VisualizationHook.h"

#include "Directory.h"
#include "SignalProperties.h"
#include "GenericSignal.h"

namespace {
    std::string VisParamName(const GenericFilter* pFilter)
    {
        return "Visualize" + pFilter->Name();
    }
}

void VisualizationHook::OnInstantiate(GenericFilter* pFilter, const std::string& posString)
{
    if (pFilter->AllowsVisualization())
    {
        mVisualizations[pFilter].SetVisID(posString);
        std::string paramDefinition = "Visualize:Processing%20Stages";
        paramDefinition +=
            " int " + VisParamName(pFilter) + "= 0 0 0 1 // Visualize " + pFilter->Name() + " output (boolean)";
        pFilter->AddParameters(&paramDefinition, 1);
    }
}

void VisualizationHook::OnDispose(GenericFilter*)
{
    mVisualizations.clear();
}

void VisualizationHook::OnPreflight(GenericFilter* pFilter, const SignalProperties& Input, const SignalProperties& Output)
{
    // Access the visualization parameter name so the system does not complain at Initialize().
    pFilter->OptionalParameter(VisParamName(pFilter));
}

void VisualizationHook::OnInitialize(GenericFilter* pFilter, const SignalProperties& Input, const SignalProperties& Output)
{
    bool visEnabled = false;
    if (pFilter->AllowsVisualization())
    {
        visEnabled = int(pFilter->Parameter(VisParamName(pFilter))) != 0;
        if (visEnabled)
            mVisualizations[pFilter].Send(Output).Send(GenericSignal(Output, GenericSignal::NaN));
        mVisualizations[pFilter].Send(CfgID::Visible, visEnabled);
    }
    mVisualizations[pFilter].SetEnabled(visEnabled);
}

void VisualizationHook::OnProcess(GenericFilter* pFilter, const GenericSignal& Input, const GenericSignal& Output)
{
    if (mVisualizations[pFilter].Enabled())
        mVisualizations[pFilter].Send(Output);
}

