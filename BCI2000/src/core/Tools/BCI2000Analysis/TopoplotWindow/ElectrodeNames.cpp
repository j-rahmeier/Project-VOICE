////////////////////////////////////////////////////////////////////////////////
// $Id: ElectrodeNames.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Data for electrode names and locations in the 10-20 system.
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
#include "ElectrodeNames.h"
#include "GUI.h"
#include "Numeric.h"
#include "StringUtils.h"
#include <map>
#include <string>

namespace
{
struct Electrode
{
    int idx;
    float angle;
    float radius;
    const char *name;
};
const Electrode sElectrodes[] = {{1, -68, 0.3, "Fc5"},
                                 {2, -60, 0.219, "Fc3"},
                                 {3, -42, 0.14, "Fc1"},
                                 {4, 0., 0.101, "Fcz"},
                                 {5, 42, 0.14, "Fc2"},
                                 {6, 60, 0.219, "Fc4"},
                                 {7, 68, 0.3, "Fc6"},
                                 {8, -90, 0.304, "C5"},
                                 {9, -90, 0.203, "C3"},
                                 {10, -90, 0.101, "C1"},
                                 {11, 0., 0., "Cz"},
                                 {12, 90, 0.101, "C2"},
                                 {13, 90, 0.203, "C4"},
                                 {14, 90, 0.304, "C6"},
                                 {15, -112, 0.3, "Cp5"},
                                 {16, -120, 0.219, "Cp3"},
                                 {17, -138, 0.14, "Cp1"},
                                 {18, 180, 0.101, "Cpz"},
                                 {19, 138, 0.14, "Cp2"},
                                 {20, 120, 0.219, "Cp4"},
                                 {21, 112, 0.3, "Cp6"},
                                 {22, -18, 0.406, "Fp1"},
                                 {23, 0., 0.406, "Fpz"},
                                 {24, 18, 0.406, "Fp2"},
                                 {25, -36, 0.406, "Af7"},
                                 {26, -23, 0.343, "Af3"},
                                 {27, 0., 0.304, "Afz"},
                                 {28, 23, 0.343, "Af4"},
                                 {29, 36, 0.406, "Af8"},
                                 {30, -54, 0.406, "F7"},
                                 {31, -46, 0.363, "F5"},
                                 {32, -35, 0.275, "F3"},
                                 {33, -21, 0.219, "F1"},
                                 {34, 0., 0.203, "Fz"},
                                 {35, 21, 0.219, "F2"},
                                 {36, 35, 0.275, "F4"},
                                 {37, 46, 0.363, "F6"},
                                 {38, 54, 0.406, "F8"},
                                 {39, -72, 0.406, "Ft7"},
                                 {40, 72, 0.406, "Ft8"},
                                 {41, -90, 0.406, "T7"},
                                 {42, 90, 0.406, "T8"},
                                 {43, -90, 0.499, "T9"},
                                 {44, 90, 0.499, "T10"},
                                 {45, -108, 0.406, "Tp7"},
                                 {46, 108, 0.406, "Tp8"},
                                 {47, -126, 0.406, "P7"},
                                 {48, -134, 0.343, "P5"},
                                 {49, -145, 0.275, "P3"},
                                 {50, -159, 0.219, "P1"},
                                 {51, 180, 0.181, "Pz"},
                                 {52, 159, 0.219, "P2"},
                                 {53, 145, 0.275, "P4"},
                                 {54, 134, 0.343, "P6"},
                                 {55, 126, 0.406, "P8"},
                                 {56, -144, 0.406, "Po7"},
                                 {57, -157, 0.343, "Po3"},
                                 {58, 180, 0.304, "Poz"},
                                 {59, 157, 0.343, "Po4"},
                                 {60, 144, 0.406, "Po8"},
                                 {61, -162, 0.406, "O1"},
                                 {62, 180, 0.406, "Oz"},
                                 {63, 162, 0.406, "O2"},
                                 {64, 180, 0.499, "Iz"},
                                 {0}};
struct CiLess
{
    bool operator()(const std::string &a, const std::string &b) const
    {
        return StringUtils::CiLess(a, b);
    }
};
struct ElectrodesByName : std::map<std::string, GUI::Point, CiLess>
{
    ElectrodesByName(const Electrode *pElectrodes)
    {
        for (auto p = pElectrodes; p->name != nullptr; ++p)
        {
            float angle = p->angle;
            angle *= Pi<float>() / 180;
            float radius = p->radius * 0.8;
            GUI::Point point{0.5 + ::sin(angle) * radius, 0.5 - ::cos(angle) * radius};
            this->insert(std::make_pair(p->name, point));
        }
    }
};
const ElectrodesByName &sElectrodesByName()
{
    static ElectrodesByName instance(sElectrodes);
    return instance;
}
}; // namespace

namespace ElectrodeNames
{
const GUI::Point *getLocationFromName(const std::string &inName)
{
    auto &db = sElectrodesByName();
    auto i = db.find(inName);
    if (i != db.end())
        return &i->second;
    return nullptr;
}

} // namespace ElectrodeNames
