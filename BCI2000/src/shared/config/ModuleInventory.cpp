////////////////////////////////////////////////////////////////////////////////
// $Id: ModuleInventory.cpp 8075 2024-04-25 16:16:28Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Access to an inventory of modules. The inventory contains
//   all modules present in the current build.
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
#include "ModuleInventory.h"

#undef MODULE
#define MODULE(category, name) {category, #name},

namespace bci
{

const char* Inventory::CategoryToString(int category)
{
    switch (category)
    {
#define CATEGORY(x) case x: return #x;
        CATEGORY(SignalSource)
        CATEGORY(SignalProcessing)
        CATEGORY(Application)
        CATEGORY(Operator)
        CATEGORY(Tool)
        CATEGORY(Helper)
        CATEGORY(Unknown)
    }
    return "Unknown";
}

Inventory::Inventory()
{
    const struct
    {
        int category;
        const char *name;
    } modules[] = {
#include "Inventory.inc"
        {0, NULL} // avoid a zero-sized array when inventory is empty
    };
    for (size_t i = 0; i < sizeof(modules) / sizeof(*modules) - 1; ++i)
        (*this)[modules[i].category].insert(modules[i].name);
}

int Inventory::Categorize(const std::string& module) const
{
    for (const auto& entry : *this)
        if (entry.second.find(module) != entry.second.end())
            return entry.first;
    return Unknown;
}

bool CaseInsensitiveLess::operator()(const std::string& a, const std::string& b) const
{
    return ::stricmp(a.c_str(), b.c_str()) < 0;
}

} // namespace bci
