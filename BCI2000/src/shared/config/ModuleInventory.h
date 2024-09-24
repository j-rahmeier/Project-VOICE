////////////////////////////////////////////////////////////////////////////////
// $Id: ModuleInventory.h 8075 2024-04-25 16:16:28Z mellinger $
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
#ifndef MODULE_INVENTORY_H
#define MODULE_INVENTORY_H

#include <map>
#include <set>
#include <string>

namespace bci
{
struct CaseInsensitiveLess;

class Inventory : public std::map<int, std::set<std::string, CaseInsensitiveLess>>
{
  public:
    enum Category
    {
        SignalSource,
        SignalProcessing,
        Application,
        Operator,
        Tool,
        Helper,
        Unknown,
    };
    static const char* CategoryToString(int);

    Inventory();
    int Categorize(const std::string&) const;
};

struct CaseInsensitiveLess
{
    bool operator()(const std::string&, const std::string&) const;
};

} // namespace bci

#endif // MODULE_INVENTORY_H
