////////////////////////////////////////////////////////////////////////////////
// $Id: TextGroupStimulus.cpp 8268 2024-07-23 16:38:37Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A stimulus consisting of a text field.
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
#include "TextGroupStimulus.h"

TextGroupStimulus::TextGroupStimulus( GUI::GraphDisplay& display )
: TextField( display, TextStimulusZOrder ),
  mIntensifiedColor( RGBColor::White )
{
}

TextGroupStimulus::~TextGroupStimulus()
{
}

TextGroupStimulus&
TextGroupStimulus::SetIntensifiedColor( RGBColor c )
{
  mIntensifiedColor = c;
  Change();
  return *this;
}

RGBColor
TextGroupStimulus::IntensifiedColor() const
{
  return mIntensifiedColor;
}

void
TextGroupStimulus::OnPaint( const GUI::DrawContext& dc )
{
  bool doPaint = false;
  RGBColor backgroundColor = RGBColor::Black,
           textColor = RGBColor::Black;
  if( BeingPresented() )
    switch( PresentationMode() )
    {
      case ShowHide:
        doPaint = true;
        backgroundColor = Color();
        textColor = TextColor();
        break;

      case Intensify:
        doPaint = true;
        backgroundColor = Color();
        textColor = IntensifiedColor();
        break;

      case Grayscale:
        doPaint = true;
        backgroundColor = Color().ToGray();
        textColor = TextColor().ToGray();
        break;

      case Invert:
        doPaint = true;
        backgroundColor = TextColor();
        textColor = Color();
        break;

      case Dim:
        doPaint = true;
        backgroundColor = DimFactor() * Color();
        textColor = DimFactor() * TextColor();
        break;

      default:
        ;
    }
  else // not being presented
    switch( PresentationMode() )
    {
      case ShowHide:
        doPaint = false;
        break;

      case Intensify:
      case Grayscale:
      case Invert:
      case Dim:
        doPaint = true;
        backgroundColor = Color();
        textColor = TextColor();
        break;

      default:
        ;
    }
  if( doPaint )
    TextField::DoPaint( dc, textColor, backgroundColor );
}

