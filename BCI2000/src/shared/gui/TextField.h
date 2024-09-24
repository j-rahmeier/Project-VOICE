////////////////////////////////////////////////////////////////////////////////
// $Id: TextField.h 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A GraphObject displaying a line of text.
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
#ifndef TEXT_FIELD_H
#define TEXT_FIELD_H

#include "GraphObject.h"

#include "Color.h"
#include <string>

class TextField : public GUI::GraphObject
{
  public:
    TextField(GUI::GraphDisplay &display, int zOrder = MessageZOrder);
    virtual ~TextField();
    // Properties
    TextField &SetText(const std::string &);
    std::string Text() const;
    TextField &SetTextHeight(float);
    float TextHeight() const;
    TextField &SetTextColor(RGBColor);
    RGBColor TextColor() const;
    TextField &SetColor(RGBColor);
    RGBColor Color() const;

  protected:
    // GraphObject event handlers
    void OnChange(GUI::DrawContext &) override;
    void OnResize(GUI::DrawContext &) override;
    void OnMove(GUI::DrawContext &) override;
    void OnPaint(const GUI::DrawContext &) override;

  protected:
    // We introduce a separate, protected DoPaint() function to simplify
    // implementation of a derived TextStimulus class that renders itself
    // dependent on its highlighted state.
    void DoPaint(const GUI::DrawContext &, RGBColor textColor, RGBColor backgroundColor);

  private:
    struct Private;
    Private *p;
};

#endif // TEXT_FIELD_H
