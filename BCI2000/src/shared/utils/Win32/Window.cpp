////////////////////////////////////////////////////////////////////////////////
// $Id: Window.cpp 7776 2023-12-20 16:24:18Z mellinger $
// Author: mellinger@neurotechcenter.com
// Description: A thin C++ wrapper around a Win32 window.
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
#include "Window.h"

namespace {
  int Width(const RECT* p)
  {
    return p->right - p->left;
  }
  int Height(const RECT* p)
  {
    return p->bottom - p->top;
  }
  bool RectLarger(const RECT* p1, const RECT* p2)
  {
    int a1 = Width(p1) * Height(p1), a2 = Width(p2) * Height(p2);
    return a1 > a2;
  }
  BOOL CALLBACK OnMonitor(HMONITOR, HDC dc, LPRECT pRect, LPARAM data)
  {
    RECT* largestRect = reinterpret_cast<RECT*>(data);
    if (dc && RectLarger(pRect, largestRect))
      *largestRect = *pRect;
    return TRUE;
  }
  void EnsureRectVisible(RECT* pRect)
  {
    RECT largestRect = { 0 };
    HDC dc = ::GetDC(NULL);
    ::EnumDisplayMonitors(dc, pRect, &OnMonitor, reinterpret_cast<LPARAM>(&largestRect));
    ::ReleaseDC(NULL, dc);
    if (Width(&largestRect) < 10 && Height(&largestRect) < 10)
    {
      pRect->right = 10 + Width(pRect);
      pRect->bottom = 10 + Height(pRect);
      pRect->left = 10;
      pRect->top = 10;
    }
  }
}


Window::Window()
: mSystemBackground(false)
{
  if (!sClassName) {
    sClassName = "Window";
    WNDCLASSA wc = { 0 };
    wc.lpszClassName = sClassName;
    wc.hInstance = ::GetModuleHandleA(nullptr);
    wc.lpfnWndProc = &Window::WindowProc;
    wc.hbrBackground = ::CreateSolidBrush(0x000000);
    wc.hCursor = ::LoadCursorA(NULL, IDC_ARROW);
    wc.hIcon = ::LoadIconA(::GetModuleHandleA(nullptr), MAKEINTRESOURCE(1));
    ::RegisterClassA(&wc);
  }
}

Window::~Window()
{
  if (mHandle)
   ::DestroyWindow(mHandle);
}

HWND Window::Handle(bool create)
{
  if (create && !mHandle)
    Create();
  return mHandle;
}

void Window::Show(int nCmdShow)
{
  ::ShowWindow(Handle(), nCmdShow);
}

void Window::Hide()
{
  Show(SW_HIDE);
}

void Window::Close()
{
  ::SendMessageA(Handle(), WM_CLOSE, 0, 0);
}

int Window::State() const
{
  return mState;
}

Window& Window::SetStyle(int style)
{
  if (style != mStyle) {
    mStyle = style;
    if (mHandle) {
      ::SetWindowLongA(mHandle, GWL_STYLE, mStyle);
      ::SetWindowPos(mHandle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }
  }
  return *this;
}

int Window::Style() const
{
  return mStyle;
}

void Window::Create()
{
  if (!mHandle) {
    RECT rect = { 0 };
    rect.left = mLeft;
    rect.top = mTop;
    rect.right = mLeft + mWidth;
    rect.bottom = mTop + mHeight;
    ::AdjustWindowRectEx(&rect, mStyle, FALSE, 0);
    EnsureRectVisible(&rect);
    HWND handle = ::CreateWindowExA(
      0, sClassName, mTitle.c_str(), mStyle,
      rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
      NULL, NULL, ::GetModuleHandleA(nullptr), this
    );
    // mHandle must have been assigned from window procedure
    if (handle != mHandle)
        throw std::logic_error("Window::Create(): handles do not match");
    OnCreate();
  }
}

Window& Window::SetLeft(int left)
{
  mLeft = left;
  SyncGeometry();
  return *this;
}

int Window::Left() const
{
  return mLeft;
}

int Window::NormalLeft() const
{
  return mNormalLeft;
}

Window& Window::SetTop(int top)
{
  mTop = top;
  SyncGeometry();
  return *this;
}

int Window::Top() const
{
  return mTop;
}

int Window::NormalTop() const
{
  return mNormalTop;
}

Window& Window::SetWidth(int width)
{
  mWidth = width;
  SyncGeometry();
  return *this;
}

int Window::Width() const
{
  return mWidth;
}

int Window::NormalWidth() const
{
  return mNormalWidth;
}

Window& Window::SetHeight(int height)
{
  mHeight = height;
  SyncGeometry();
  return *this;
}

int Window::Height() const
{
  return mHeight;
}

int Window::NormalHeight() const
{
  return mNormalHeight;
}

Window& Window::SetTitle(const std::string& s)
{
  mTitle = s;
  if (mHandle)
    ::SetWindowTextA(mHandle, mTitle.c_str());
  return *this;
}

const std::string& Window::Title() const
{
  return mTitle;
}

Window& Window::SetSystemBackground(bool b)
{
  mSystemBackground = b;
  return *this;
}

bool Window::SystemBackground() const
{
  return mSystemBackground;
}

Window& Window::Invalidate(const RECT* pRect)
{
  ::InvalidateRect(mHandle, pRect, mSystemBackground);
  return *this;
}

LRESULT Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  Window* this_ = nullptr;
  if (uMsg == WM_NCCREATE || uMsg == WM_CREATE) {
    auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
    this_ = reinterpret_cast<Window*>(cs->lpCreateParams);
    this_->mHandle = hwnd;
    ::SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this_));
  }
  else {
    this_ = reinterpret_cast<Window*>(::GetWindowLongPtrA(hwnd, GWLP_USERDATA));
  }
  switch (uMsg) {
  case WM_MOVE:
    this_->mLeft = LOWORD(lParam);
    this_->mTop = HIWORD(lParam);
    if (this_->mState == Normal) {
        this_->mNormalLeft = this_->mLeft;
        this_->mNormalTop = this_->mTop;
    }
    this_->OnMove(LOWORD(lParam), HIWORD(lParam));
    break;
  case WM_SIZE:
    this_->mWidth = LOWORD(lParam);
    this_->mHeight = HIWORD(lParam);
    switch (wParam)
    {
        case SIZE_MAXIMIZED:
            this_->mState = Maximized;
            break;
        case SIZE_MINIMIZED:
            this_->mState = Minimized;
            break;
        case SIZE_RESTORED:
        default:
            this_->mState = Normal;
            break;
    }
    if (this_->mState == Normal) {
        this_->mNormalWidth = this_->mWidth;
        this_->mNormalHeight = this_->mHeight;
    }
    this_->OnResize(LOWORD(lParam), HIWORD(lParam));
    break;
  case WM_ERASEBKGND:
    if (this_->mSystemBackground)
      return ::DefWindowProcA(hwnd, uMsg, wParam, lParam);
    break;
  case WM_CLOSE:
    if (this_->OnClose())
      ::DestroyWindow(hwnd);
    break;
  case WM_DESTROY:
    ::PostQuitMessage(0);
    break;
  case WM_PAINT:
    this_->OnPaint();
    /* fall through */
  default:
    return ::DefWindowProcA(hwnd, uMsg, wParam, lParam);
  }
  return 0;
}

void Window::SyncGeometry()
{
  if (mHandle) {
    RECT rect = { 0 };
    rect.left = mLeft;
    rect.top = mTop;
    rect.right = mLeft + mWidth;
    rect.bottom = mTop + mHeight;
    ::AdjustWindowRectEx(&rect, mStyle, FALSE, 0);
    EnsureRectVisible(&rect);
    ::MoveWindow(mHandle, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
  }
}

const char* Window::sClassName = nullptr;

