////////////////////////////////////////////////////////////////////////////////
// $Id: Window.h 7776 2023-12-20 16:24:18Z mellinger $
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
#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <string>

class Window
{
public:
  enum { Normal, Minimized, Maximized };

  Window();
  ~Window();
  HWND Handle(bool create = true);
  void Show(int nCmdShow = SW_SHOWNORMAL);
  void Hide();
  void Close();

  int State() const;
  Window& SetStyle(int style);
  int Style() const;
  Window& SetLeft(int left);
  int Left() const;
  int NormalLeft() const;
  Window& SetTop(int top);
  int Top() const;
  int NormalTop() const;
  Window& SetWidth(int width);
  int Width() const;
  int NormalWidth() const;
  Window& SetHeight(int height);
  int Height() const;
  int NormalHeight() const;
  Window& SetTitle(const std::string& title);
  const std::string& Title() const;
  Window& SetSystemBackground(bool);
  bool SystemBackground() const;
  Window& Invalidate(const RECT* = nullptr);

protected:
  virtual void OnCreate() {}
  virtual bool OnPreTranslate(MSG*) { return false; }
  virtual void OnMove(int, int) {}
  virtual void OnResize(int, int) {}
  virtual bool OnClose() { return true; }
  virtual void OnPaint() {}

private:
  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  void Create();
  void SyncGeometry();

private:
  friend class EventLoop;
  static const int sMagic = 'bCi2';
  const int mMagic = sMagic;
  int mStyle = WS_OVERLAPPEDWINDOW;
  int mState = Normal;
  int mLeft = 100, mTop = 100, mWidth = 640, mHeight = 480,
      mNormalLeft = 100, mNormalTop = 100,
      mNormalWidth = 640, mNormalHeight = 480;
  HWND mHandle = NULL;
  std::string mTitle;
  bool mSystemBackground;
  static const char* sClassName;
};

#endif // WINDOW_H

