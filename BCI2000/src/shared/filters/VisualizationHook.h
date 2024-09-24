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
#ifndef VISUALIZATION_HOOK_H
#define VISUALIZATION_HOOK_H

#include "Uncopyable.h"
#include "GenericFilter.h"
#include "GenericVisualization.h"

class VisualizationHook : public GenericFilter::Chain::FilterHook, private Uncopyable
{
  protected:
    void OnInstantiate(GenericFilter*, const std::string& position) override;
    void OnDispose(GenericFilter*) override;
    void OnPreflight(GenericFilter*, const SignalProperties& Input, const SignalProperties& Output) override;
    void OnInitialize(GenericFilter*, const SignalProperties& Input, const SignalProperties& Output) override;
    void OnProcess(GenericFilter*, const GenericSignal& Input, const GenericSignal& Output) override;

  private:
    class FilterVis : public GenericVisualization
    {
        public:
        FilterVis() : mEnabled(false)
        {
        }
        FilterVis(const FilterVis &f) : GenericVisualization(f), mEnabled(f.mEnabled)
        {
        }
        FilterVis &SetEnabled(bool b)
        {
            mEnabled = b;
            return *this;
        }
        bool Enabled() const
        {
            return mEnabled;
        }

        private:
        bool mEnabled;
    };
    typedef std::map<GenericFilter *, FilterVis> VisualizationsType;
    VisualizationsType mVisualizations;
};

#endif // VISUALIZATION_HOOK_H
