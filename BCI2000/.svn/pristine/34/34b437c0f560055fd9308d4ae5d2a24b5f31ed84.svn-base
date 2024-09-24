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
#include "TextRenderer.h"

#include <cmath>

#include <schrift.h>
#include <util/utf8_to_utf32.h>

struct TextRenderer::Private
{
    RGBColor mTextColor = RGBColor::Black;
    int mTextSize = 12;
    SFT_Font *mpSftFont = nullptr;

    uint32_t* mpExternalData = nullptr;
    int mWidth = 0, mHeight = 0;

    Size ProcessText(int x, int y, const std::string&, bool render);
    void PutCharacterImage(int x, int y, SFT_Image*);
};

TextRenderer::TextRenderer()
: p(new Private)
{
}

TextRenderer::~TextRenderer()
{
    DetachFromImage();
    ::sft_freefont(p->mpSftFont);
    delete p;
}

TextRenderer& TextRenderer::SetTextColor(RGBColor c)
{
    p->mTextColor = c;
    return *this;
}

RGBColor TextRenderer::TextColor() const
{
    return p->mTextColor;
}

TextRenderer& TextRenderer::SetTextSize(int size)
{
    p->mTextSize = size;
    return *this;
}

int TextRenderer::TextSize() const
{
    return p->mTextSize;
}

TextRenderer& TextRenderer::LoadFont(const std::string& file)
{
    if (p->mpSftFont)
        ::sft_freefont(p->mpSftFont);
    p->mpSftFont = ::sft_loadfile(file.c_str());
    return *this;
}

TextRenderer& TextRenderer::LoadFont(const void* pMemory, int byteLength)
{
    if (p->mpSftFont)
        ::sft_freefont(p->mpSftFont);
    p->mpSftFont = ::sft_loadmem(pMemory, byteLength);
    return *this;
}

void TextRenderer::AttachToImage(int width, int height, uint32_t* pData)
{
    p->mWidth = width;
    p->mHeight = height;
    p->mpExternalData = pData;
}

void TextRenderer::DetachFromImage()
{
    p->mWidth = 0;
    p->mHeight = 0;
    p->mpExternalData = nullptr;
}

TextRenderer::Size TextRenderer::MeasureText(const std::string& text)
{
    return p->ProcessText(0, 0, text, false);
}

void TextRenderer::RenderText(int x, int y, const std::string& text)
{
    p->ProcessText(x, y, text, true);
}

TextRenderer::Size TextRenderer::Private::ProcessText(int x, int y, const std::string& text, bool render)
{
    if (!mpSftFont) {
        return Size();
    }

    std::vector<uint32_t> utf32text(text.length() + 1, 0);
    union { const char* c; const uint8_t* u; } pText = { text.c_str() };
    int count = ::utf8_to_utf32(pText.u, utf32text.data(), utf32text.size());
    utf32text.resize(count);

    SFT sft = { 0 };
    sft.flags = SFT_DOWNWARD_Y;
    sft.font = mpSftFont;
    sft.xOffset = 0;
    sft.yOffset = 0;
    sft.xScale = mTextSize;
    sft.yScale = sft.xScale;

    SFT_Image sftImage = {0};
    std::vector<uint8_t*> data;

    SFT_LMetrics lmetrics = { 0 };
    ::sft_lmetrics(&sft, &lmetrics);
    double lineHeight = lmetrics.ascender + lmetrics.descender;

    double width = 0;
    for (const auto& codepoint : utf32text) {
        SFT_Glyph glyph = { 0 };
        ::sft_lookup(&sft, codepoint, &glyph);
        SFT_GMetrics gmetrics = { 0 };
        ::sft_gmetrics(&sft, glyph, &gmetrics);
        if (render) {
            sftImage.height = gmetrics.minHeight;
            sftImage.width = gmetrics.minWidth;
            data.resize(sftImage.height * sftImage.width);
            sftImage.pixels = data.data();
            ::sft_render(&sft, glyph, sftImage);
            PutCharacterImage(
                x + std::round(width + gmetrics.leftSideBearing),
                y + std::round(lineHeight + gmetrics.yOffset),
                &sftImage
            );
        }
        width += gmetrics.advanceWidth;
    }
    Size size = { std::round(width), std::round(lineHeight) };
    return size;
}

// Apply the raster mask contained in a SFT_Image at given position to draw a character into
// the image we are attached to.
void TextRenderer::Private::PutCharacterImage(int xpos, int ypos, SFT_Image* pImage)
{
    uint8_t* mask = static_cast<uint8_t*>(pImage->pixels);
    for (int x = 0; x < pImage->width; ++x) {

        if (xpos + x < 0)
            continue;
        if (xpos + x >= mWidth)
            continue;

        for (int y = 0; y < pImage->height; ++y) {

            if (ypos + y < 0)
                continue;
            if (ypos + y >= mHeight)
                continue;

            int idx1 = x + y * pImage->width,
                idx2 = xpos + x + (ypos + y) * mWidth;

                float alpha = mask[idx1] / 255.;
                RGBColor current = mpExternalData[idx2],
                            modified = alpha * mTextColor + (1 - alpha) * current;
                mpExternalData[idx2] = modified;
        }
    }
}

