////////////////////////////////////////////////////////////////////////////////
// $Id: FrameComposer.cpp 7788 2024-01-08 21:27:12Z mellinger $
// Author: mellinger@neurotechcenter.com
// Description: A class that places subimages into a larger image.
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
#include "FrameComposer.h"
#include "SysError.h"

#include <vector>

#include <Windows.h>

namespace {
  DWORD CreateOffscreenBuffer(int width, int height, HBITMAP* pHBitmap, BYTE** ppMemory)
  {
    BITMAPV5HEADER bi = { 0 };
    bi.bV5Size = sizeof(BITMAPV5HEADER);
    bi.bV5Width = width;
    bi.bV5Height = height;
    bi.bV5Planes = 1;
    bi.bV5BitCount = 32;
    bi.bV5Compression = BI_RGB;
    bi.bV5RedMask = 0x00FF0000;
    bi.bV5GreenMask = 0x0000FF00;
    bi.bV5BlueMask = 0x000000FF;
    bi.bV5AlphaMask = 0xFF000000;

    HDC dc = ::GetDC(NULL);
    *pHBitmap = ::CreateDIBSection(dc, reinterpret_cast<BITMAPINFO*>(&bi),
      DIB_RGB_COLORS, reinterpret_cast<void**>(ppMemory), NULL, 0);
    int err = *pHBitmap ? NO_ERROR : ::GetLastError();
    ::ReleaseDC(NULL, dc);
    return err;
  }
}
  
struct FrameComposer::Private
{
  int mWidth = 0, mHeight = 0;
  int mRows = 0, mCols = 0;
  std::vector<std::vector<bool>> mFilledBitmap;

  BYTE* mpImageData = nullptr;
  HBITMAP mImage = NULL;

  std::string mError;

  ~Private();

  void createFilledBitmap();

  void createBitmapBuffer();
  void deleteBitmapBuffer();
};

FrameComposer::Private::~Private()
{
  deleteBitmapBuffer();
}

void FrameComposer::Private::createFilledBitmap()
{
  mFilledBitmap = std::vector<std::vector<bool>>(mRows, std::vector<bool>(mCols, false));
}

void FrameComposer::Private::createBitmapBuffer()
{
  deleteBitmapBuffer();
  if (mWidth <= 0 || mHeight <=0)
    return;
  int err = CreateOffscreenBuffer(mWidth, mHeight, &mImage, &mpImageData);
  if (err)
    mError = SysError(err).Message();
}

void FrameComposer::Private::deleteBitmapBuffer()
{
  if (mImage)
    ::DeleteObject(mImage);
  mpImageData = nullptr;
}

FrameComposer::FrameComposer()
: p(new Private)
{
}

FrameComposer::~FrameComposer()
{
  delete p;
}

FrameComposer& FrameComposer::setHeight(int height)
{
  if (p->mHeight != height)
  {
    p->mHeight = height;
    p->createBitmapBuffer();
  }
  return *this;
}

int FrameComposer::height() const
{
  return p->mHeight;
}

FrameComposer& FrameComposer::setWidth(int width)
{
  if (p->mWidth != width)
  {
    p->mWidth = width;
    p->createBitmapBuffer();
  }
  return *this;
}

int FrameComposer::width() const
{
  return p->mWidth;
}

FrameComposer& FrameComposer::setRows(int rows)
{
  if (p->mRows != rows)
  {
    p->mRows = rows;
    p->createFilledBitmap();
  }
  return *this;
}

int FrameComposer::rows() const
{
  return p->mRows;
}

FrameComposer& FrameComposer::setCols(int cols)
{
  if (p->mCols != cols)
  {
    p->mCols = cols;
    p->createFilledBitmap();
  }
  return *this;
}

int FrameComposer::cols() const
{
  return p->mCols;
}


FrameComposer& FrameComposer::putSubImage(int row, int col, int rowSpan, int colSpan, int width, int height, void* pData)
{
  if (row >= p->mRows || col >= p->mCols || row < 0 || col < 0 || rowSpan < 1 || colSpan < 1)
  {
    p->mError = "Index out of bounds";
    return *this;
  }

  for (int r = row; r < row + rowSpan; ++r)
    for (int c = col; c < col + colSpan; ++c)
      p->mFilledBitmap[r][c] = true;

  if (width <= 0 || height <= 0)
  {
    p->mError = "Subimage size invalid";
    return *this;
  }

  int left = col * p->mWidth / p->mCols,
      right = (col + colSpan) * p->mWidth / p->mCols,
      top = row * p->mHeight / p->mRows,
      bottom = (row + rowSpan) * p->mHeight / p->mRows;

  BYTE* pSubImageData = nullptr;
  HBITMAP subImageBitmap = NULL;
  int err = CreateOffscreenBuffer(width, height, &subImageBitmap, &pSubImageData);
  if (err)
  {
    p->mError = SysError(err).Message();
    return *this;
  }
  ::memcpy(pSubImageData, pData, width * height * sizeof(RGBQUAD));

  HDC dc = ::GetDC(NULL);
  HDC memDC1 = ::CreateCompatibleDC(dc);
  HDC memDC2 = ::CreateCompatibleDC(dc);
  ::ReleaseDC(NULL, dc);

  HGDIOBJ oldBitmap1 = ::SelectObject(memDC1, p->mImage);
  HGDIOBJ oldBitmap2 = ::SelectObject(memDC2, subImageBitmap);
  if (!::StretchBlt(memDC1, left, top, right - left, bottom - top, memDC2, 0, height - 1, width, -height, SRCCOPY))
  {
    int err = ::GetLastError();
    p->mError = "Could not perform StretchBlt: ";
    p->mError += SysError(err).Message();
  }
  ::SelectObject(memDC2, oldBitmap2);
  ::DeleteDC(memDC2);
  ::SelectObject(memDC1, oldBitmap1);
  ::DeleteDC(memDC1);
  ::DeleteObject(subImageBitmap);

  return *this;
}

FrameComposer& FrameComposer::putSubImage(int row, int col, int height, int width, void* data)
{
  return putSubImage(row, col, 1, 1, height, width, data);
}

bool FrameComposer::imageDone() const
{
  for (const auto& row : p->mFilledBitmap)
    for (bool b : row)
      if (!b)
        return false;
  return true;
}

FrameComposer& FrameComposer::clear(RGBColor color)
{
  BYTE* pData = p->mpImageData;
  for (int x = 0; x < p->mWidth; ++x)
  {
    for (int y = 0; y < p->mHeight; ++y)
    {
      *pData++ = color.B();
      *pData++ = color.G();
      *pData++ = color.R();
      *pData++ = 0;
    }
  }
  for (auto& row : p->mFilledBitmap)
    for (auto& b : row)
      b = false;
  return *this;
}

void* FrameComposer::imageData() const
{
  return p->mpImageData;
}

std::string FrameComposer::error() const
{
  return p->mError;
}

