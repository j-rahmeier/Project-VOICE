////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.com
// Description: A class that renders text into an RGB32 image.
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
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "Color.h"
#include <string>

class TextRenderer
{
public:
    TextRenderer();
    ~TextRenderer();
    TextRenderer(const TextRenderer&) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;

    // Foreground color of the text.
    TextRenderer& SetTextColor(RGBColor);
    RGBColor TextColor() const;

    // Size of the text, in pixels.
    TextRenderer& SetTextSize(int);
    int TextSize() const;

    // Loads a TTF font from a file.
    TextRenderer& LoadFont(const std::string& file);
    // Loads a TTF font from memory.
    TextRenderer& LoadFont(const void* pMemory, int byteLength);

    // Attach to an RGB32 image. Text will be drawn onto this image.
    void AttachToImage(int width, int height, uint32_t *pData);
    // Detach from current image.
    void DetachFromImage();

    struct Size { int width = 0, height = 0; };
    // Return the size of the given text, in pixels.
    Size MeasureText(const std::string&);
    // Draw text with the current font, at the given location.
    // Cannot use "DrawText()" without interfering with Win32 macro.
    void RenderText(int x, int y, const std::string&);

private:
    struct Private;
    Private* p;
};

#endif // TEXT_RENDERER_H

