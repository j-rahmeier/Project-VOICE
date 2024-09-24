////////////////////////////////////////////////////////////////////////////////
// $Id: ColorListChooser.cpp 7695 2023-11-06 17:13:42Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A color chooser dialog used to define a color list.
//   The dialog's "custom colors" are used to hold colors, and the first
//   black entry delimits the list of colors.
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
#include "ColorListChooser.h"

#include <QColorDialog>
#include <algorithm>

bool ColorListChooser::Display(RGBColor &ioSelected, ColorList &ioCustomColors) const
{
    for (int i = 0; i < std::min(static_cast<int>(ioCustomColors.size()), QColorDialog::customCount()); ++i)
        QColorDialog::setCustomColor(i, ioCustomColors[i].ToQRgb());
    for (int i = static_cast<int>(ioCustomColors.size()); i < QColorDialog::customCount(); ++i)
        QColorDialog::setCustomColor(i, RGBColor(RGBColor::Black).ToQRgb());
    QColor result = QColorDialog::getColor(ioSelected.ToQRgb(), nullptr, mPrompt.c_str(), QColorDialog::DontUseNativeDialog);
    if (result.isValid())
    {
        ioSelected = RGBColor::FromQRgb(result.rgba());
        int numUserColors = 0;
        while (numUserColors < QColorDialog::customCount() &&
               QColorDialog::customColor(numUserColors) != RGBColor::Black)
            ++numUserColors;
        if (numUserColors == 0)
            ioCustomColors.resize(1, RGBColor::White);
        else
        {
            ioCustomColors.resize(numUserColors);
            for (int i = 0; i < numUserColors; ++i)
                ioCustomColors[i] = RGBColor::FromQRgb(QColorDialog::customColor(i).rgba());
        }
    }
    return result.isValid();
}

const ColorListChooser& ColorListChooser::Display(ColorList& ioCustomColors) const
{
    RGBColor selected = RGBColor::White;
    Display(selected, ioCustomColors);
    return *this;
}
