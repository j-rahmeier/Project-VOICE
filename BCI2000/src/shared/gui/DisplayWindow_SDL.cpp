////////////////////////////////////////////////////////////////////////////////
// $Id: DisplayWindow_SDL.cpp 7647 2023-10-06 14:58:50Z mellinger $
// Author: Max Marcus
// Description: A GraphDisplay descendant which is a frameless GUI window for
//   an application's user display, based on SDL.
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
#include "DisplayWindow_SDL.h"

#if __APPLE__ && __has_include( "SDL.h" )
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif


#include "BCIStream.h"
#include "OpenGLContext.h"
#include "SelfPipeQueue.h"
#include "SimpleStatistics.h"
#include "Thread.h"
#include "ThreadUtils.h"
#include "EnvVariable.h"
#include "TimeUtils.h"
#include "DisplayClock.h"
#include "SynchronizedObject.h"
#include "FileUtils.h"

namespace GUI {

class OffscreenBuffer : public Lockable<std::mutex>
{
  public:
	OffscreenBuffer() : mWidthPx(0), mHeightPx(0), mValid(false), mpGL(nullptr), mpFramebuffer(nullptr)
	{
	}
	~OffscreenBuffer()
	{
		Destroy();
	}

	bool Valid() const
	{
		return mValid;
	}
	int WidthPx() const
	{
		return mWidthPx;
	}
	int HeightPx() const
	{
		return mHeightPx;
	}

	OffscreenBuffer &SetWidthPx(int w)
	{
		mValid = false;
		mWidthPx = w;
		return *this;
	}
	OffscreenBuffer &SetHeightPx(int h)
	{
		mValid = false;
		mHeightPx = h;
		return *this;
	}

	void Create(GUI::OpenGLContext *pContext, const RGBColor &c)
	{
		Assert(!mpGL);
		if (mWidthPx > 0 && mHeightPx > 0)
		{
			mpGL = new GUI::OpenGLContext(pContext);
			WithGLContext(mpGL)
			{
				mpFramebuffer = new GUI::Framebuffer(mWidthPx, mHeightPx);
				mpGL.load()->SetRenderTarget(mpFramebuffer);
				mpGL.load()->SetViewport(0, 0, mWidthPx, mHeightPx);
			}
		}
		mBitmap = GUI::Bitmap(mWidthPx, mHeightPx);
		mValid = true;
	}
	void Destroy()
	{
		mValid = false;
		delete &*mpGL;
		mpGL = nullptr;
		delete mpFramebuffer;
		mpFramebuffer = nullptr;
		mBitmap = GUI::Bitmap(0, 0);
	}
	void Sync()
	{
		mpGL.load()->SwapBuffers();
		if (mHeightPx * mWidthPx != 0)
		  mBitmap.CopyFrom(mpFramebuffer->Front());
	}
	GUI::OpenGLContext *Gl()
	{
		return mpGL;
	}
	GUI::Framebuffer *Fb()
	{
		return mpFramebuffer;
	}
	GUI::Bitmap& Bitmap()
	{
		return mBitmap;
	}

  private:
	int mWidthPx, mHeightPx;
	std::atomic<bool> mValid;
	std::atomic<GUI::OpenGLContext *> mpGL;
	GUI::Framebuffer *mpFramebuffer;
	GUI::Bitmap mBitmap;
};


struct  SDL_Context : OpenGLContext {
	SDL_Window* win;
	SDL_GLContext glContext;

	SDL_DisplayMode mode;

	int mLeft, mTop;
	int mWidth, mHeight;

    bool current = false;

	std::string Title;

	SDL_Context(std::string Title, int x, int y, int width, int height) : OpenGLContext((OpenGLContext*) nullptr) {
		if (!SDL_WasInit(SDL_INIT_VIDEO))
			SDL_Init(SDL_INIT_VIDEO);

		//SDL_SetHint( SDL_HINT_MAC_OPENGL_ASYNC_DISPATCH, "1" );

		mWidth = width;
		mHeight = height;
		mLeft = x;
		mTop = y;
		win = SDL_CreateWindow(Title.c_str(), x, y, mWidth, mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);

		glContext = SDL_GL_CreateContext(win);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_GL_SetSwapInterval(SwapInterval());
	}

	~SDL_Context() {
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(win);
	}

	void SetPosition(int x, int y, int w, int h) {
		mWidth = w, mHeight = h;
		mLeft = x, mTop = y;
		SDL_SetWindowSize(win, w, h);
		SDL_SetWindowPosition( win, x, y );
	}

    SDL_Context& operator=(const SDL_Context& in) = delete;

	explicit operator bool() {
		return win != nullptr;
	}

	bool operator!() {
		return win == nullptr;
	}

	int Width() {
		return mWidth;
	}
	
	int Height() { 
		return mHeight;
	}

	void MakeCurrent() override {
		SDL_GL_MakeCurrent(win, glContext);
		current = true;
	}
	void DoneCurrent() override {
		current = false;
	}
	bool IsCurrent() const override {
		return current;
	}
	
	void SwapBuffers() override {
		SDL_GL_SwapWindow(win);
	}
};

struct DisplayWindow_SDL::Data 
{

	std::atomic<bool> mStopRendering { false }; 

	DisplayWindow_SDL& mrDisplay;
	int mBitmapBufferWidth = 0, mBitmapBufferHeight = 0;

	std::string mTitle;
	int mTop = 0, mLeft = 0, mWidth = 640, mHeight = 480;
	int mpWindowTop = 0, mpWindowLeft = 0;

    bool mVisible, mCursorVisible;
    std::atomic<bool> mAsyncRendering{ false };
    std::atomic<int> mUpdateRateDecimation{ 1 };

	SDL_Context* mContext = nullptr;
	SDL_Context* pGL = nullptr;

	OffscreenBuffer mScaledContentBuffer;

	SynchronizedObject<ClockTick, std::recursive_mutex> mFrame;
	SynchronizedObject<ClockTick::Statistics, std::recursive_mutex> mFrameClockStatistics;
	SynchronizedObject<struct RenderStatistics, std::recursive_mutex> mRenderStatistics;
	SynchronizedObject<struct PresentationStatistics, std::recursive_mutex>  mPresentationStatistics;
	SynchronizedObject<StringUtils::NameValueList> mpInfo;

	SelfPipeQueue<RenderState, 64> mRenderingQueue;

	Data(DisplayWindow_SDL&);
	~Data();
	bool Adjust(DrawContext&);
	void Update(void*);
	DisplayWindow_SDL& OnChange();

	int RenderingThreadFunc();

	struct RenderingThread : Thread
	{
		DisplayWindow_SDL::Data *d;
		RenderingThread(DisplayWindow_SDL::Data *d) : Thread("Rendering Thread"), d(d)
		{
		}
		int OnExecute()
		{
			return d->RenderingThreadFunc();
		}
		~RenderingThread()
		{
			TerminateAndWait();
		}
		void Stop()
		{
			d->mStopRendering = true;
			Wait();
			d->mStopRendering = false;
		}
	} mRenderingThread;
};

DisplayWindow_SDL::Data::Data(DisplayWindow_SDL& display)
	: mrDisplay(display), mRenderingThread(this), mTitle(FileUtils::ApplicationTitle()), mVisible(false)
{
    mAsyncRendering = false;
    mUpdateRateDecimation = 1;
}

DisplayWindow_SDL::Data::~Data() {
	mAsyncRendering = false;
	mRenderingThread.Stop();
	delete mContext;
}

DisplayWindow_SDL& DisplayWindow_SDL::Data::OnChange() {
	DrawContext dc = mrDisplay.Context();
	if (Adjust(dc)) 
		mrDisplay.SetContext(dc);
	return mrDisplay;
}

bool DisplayWindow_SDL::Data::Adjust(DrawContext& dc) {
	bool sizeChanged = false;
	if (mWidth != dc.rect.Width() || mHeight != dc.rect.Height() || mLeft != dc.rect.left || mTop != dc.rect.top ) {
		dc.rect.left = 0;
		dc.rect.top = 0;
		dc.rect.right = mWidth;
		dc.rect.bottom = mHeight;
		sizeChanged = true;
	}
	if ( mLeft != 0 ) { 
		mpWindowLeft = mLeft;
		mLeft = 0;
	}
	if ( mTop != 0 ) {
		mpWindowTop = mTop;
		mTop = 0;
	}

	if (mContext && sizeChanged)
		mContext->SetPosition( mpWindowLeft, mpWindowTop, mWidth, mHeight );
	
	if (mVisible) {
		SDL_Context* pGL = mContext;
		if (!pGL) {
			pGL = new SDL_Context(mrDisplay.Title(), mpWindowLeft, mpWindowTop, mWidth, mHeight);
			if (pGL->Valid()) {
				Assert(mContext == nullptr);
				mContext = pGL;
				sizeChanged = true;
				mRenderingThread.Start();
			} else 
				delete pGL;
		}
	}
	return sizeChanged;
}

void DisplayWindow_SDL::Data::Update(void* p) {
	mRenderingQueue.Push(RenderState(-1, TimeUtils::MonotonicTime(), p));
}

int DisplayWindow_SDL::Data::RenderingThreadFunc() {
	DrawContext dc = mrDisplay.Context();

	WithLock(mScaledContentBuffer) mScaledContentBuffer.Create(mContext, mrDisplay.Color());

	dc.rect.left = 0;
	dc.rect.top = 0;
	dc.rect.right = mWidth;
	dc.rect.bottom = mHeight;

	dc.gl = mContext;
	mrDisplay.SetContext(dc);

    AV::DisplayClock clock(nullptr);
	ClockTick frame = clock.LastVbl();
	WithLocked(p = mFrameClockStatistics.Mutable()) {
		::memset(&*p, 0, sizeof(*p));
		p->type = frame.type;
	}
	WithLocked(p = mPresentationStatistics.Mutable()) ::memset(&*p, 0, sizeof(*p));
	WithLocked(p = mRenderStatistics.Mutable()) ::memset(&*p, 0, sizeof(*p));
	WithLocked(p = mFrame.Mutable()) {
		*p = frame;
		--p->count;
		p->time -= frame.rate.Period();
	}

	SimpleStatistics<float> frameObs(frame.rate.Period().Seconds(), 0.005, 100), 
		renderObs(frame.rate.Period().Seconds(), 0.01, 100), latencyObs;
	const int goodStatisticsMinCount = 30;
	latencyObs.SetWindowLength(goodStatisticsMinCount).Reset();

	int framesPerUpdate = 1;
	std::vector<RenderState> requests;
	requests.reserve(mRenderingQueue.MaxSize());
	requests.push_back(RenderState(-1));

	std::string task;
	if (!EnvVariable::Get("GUI::DisplayWindow::TaskName", task))
		task = "Pro Audio";
	WithThreadTask(task, ThreadUtils::Priority::Maximum) {
		const Time::Interval timeSlice = Time::Seconds(1e-3);
		ThreadUtils::TimeSlice ts(timeSlice);
		while (!mStopRendering) {
			frame = clock.LastVbl();
			int frameDelta;
			Time::Interval timeDelta;
			WithLocked(p = mFrame.Mutable()) {
				timeDelta = frame.time - p->time;
				frameDelta = Round((frame.time - p->time) * frame.rate);
				frame.count = p->count + frameDelta;
				*p = frame;
			}

			for (int i = 0; i < frameDelta; i++) 
				frameObs.Observe(timeDelta.Seconds() / frameDelta);

			WithLocked(p = mFrameClockStatistics.Mutable()) {
				if (frameDelta > framesPerUpdate)
					p->missed += (frameDelta - 1) / framesPerUpdate;
				p->interval = Time::Seconds(frameObs.Mean());
				p->jitter = Time::Seconds(::sqrt(frameObs.Variance()));
			}

			ClockTick nextFrame = frame;
			nextFrame.count += framesPerUpdate;
			nextFrame.time += framesPerUpdate * frame.rate.Period();

			Time renderBegin = TimeUtils::MonotonicTime();

			for (RenderState r; mRenderingQueue.Pop(r);) {
				if (r.contentFrame < 0 || r.contentFrame > nextFrame.count)
					r.contentFrame = nextFrame.count;
				requests.push_back(r);
			}

			if (!requests.empty()) {
				bool presentation = mrDisplay.Tick(nextFrame);
				for (auto &r : requests) {
					r.presentation = presentation;
					mrDisplay.Emit(OnUpdateReceived, &r);
				}
			}

			if (mrDisplay.InvalidRegion().Empty())
				requests.clear();
			else {
				mrDisplay.Paint();
				auto& b = mScaledContentBuffer;
				if (b.Gl())
					WithLock(b) WithGLContext(b.Gl()) {
						b.Gl()->Clear(mrDisplay.Color());
						mContext->Get2DBuffer()->CopyToCurrentViewport();
						b.Sync();
					}

				Time renderEnd = TimeUtils::MonotonicTime();
				renderObs.Observe((renderEnd - renderBegin).Seconds());
			}

			Time blitBegin = TimeUtils::MonotonicTime(), swapEnd;
			Time timeDone = nextFrame.time - nextFrame.rate.Period() / 2;
			do {
				//mContext.SetViewport(0, 0, mWidth, mHeight);
				//mContext.Clear(mrDisplay.Color());
				//pContentBuffer->Front()->CopyToCurrentViewport();
				//mContext.SwapBuffers();

				//if (mContext.SwapInterval() == 0)
				clock.WaitForVbl();
				swapEnd = clock.LastVbl().time;
				if (mContext->SoftwareRasterized())
					while (swapEnd < nextFrame.time)
						swapEnd = clock.WaitForVbl().LastVbl().time;
			} while (swapEnd < timeDone);
			mContext->DoneCurrent();

			if (!mScaledContentBuffer.Valid())
				WithLock(mScaledContentBuffer) {
					mScaledContentBuffer.Destroy();
					mScaledContentBuffer.Create(mContext, mrDisplay.Color());
				}

			framesPerUpdate = mUpdateRateDecimation < 1 ? 1 : mUpdateRateDecimation.load();

			WithLocked(p = mRenderStatistics.Mutable()) {
				p->render.mean = Time::Seconds(renderObs.Mean());
				p->render.std = Time::Seconds(::sqrt(renderObs.Variance()));
				if (mUpdateRateDecimation < 1 && renderObs.Count() > goodStatisticsMinCount)
					framesPerUpdate = Ceil((p->render.mean + p->render.std) * frame.rate);
				Assert(framesPerUpdate > 0);
				p->decimation = framesPerUpdate;
			}

			int frameDelay = Floor((swapEnd - nextFrame.time) * frame.rate);
			int64_t displayFrame = nextFrame.count + frameDelay;
			Time displayTime = nextFrame.time + frameDelay * frame.rate.Period();
			for (auto &r : requests) {
				r.displayFrame = displayFrame;
				r.displayed = displayTime;
				if (r.contentFrame < r.displayFrame)
					++mPresentationStatistics.Mutable()->late;
				if (!IsNaN(r.requested))
					latencyObs.Observe((r.displayed - r.requested).Seconds());
				mrDisplay.Emit(OnUpdateDisplayed, &r);
			}
			WithLocked(p = mPresentationStatistics.Mutable()) {
				p->count += requests.empty() ? 0 : 1;
				if (latencyObs.Count() == 0) {
					p->latency.mean = Time::Forever();
					p->latency.std = Time::Forever();
				} else {
					p->latency.mean = Time::Seconds(latencyObs.Mean());
					p->latency.std = Time::Seconds(::sqrt(latencyObs.Variance()));
				}
			}
			requests.clear();

			if (mAsyncRendering && !mStopRendering)
                mRenderingQueue.Push(RenderState(displayFrame + framesPerUpdate));
		}
	}
	mRenderingQueue.Clear();
	WithLock(mScaledContentBuffer) mScaledContentBuffer.Destroy();
	dc = mrDisplay.Context();
	dc.gl = nullptr;
	mrDisplay.SetContext(dc);
	return 0;
}


DisplayWindow_SDL::DisplayWindow_SDL()
	: d(new Data(*this))
{
	Invalidate();
}

DisplayWindow_SDL::~DisplayWindow_SDL()
{
	delete d;
}

struct DisplayWindow::PresentationStatistics DisplayWindow_SDL::PresentationStatistics() const {
	return *d->mPresentationStatistics;
}

DisplayWindow& DisplayWindow_SDL::Update(void*)
{
	d->OnChange();
	return *this;
}

StringUtils::NameValueList DisplayWindow_SDL::Info() const
{
	return *d->mpInfo;
}

bci::ClockTick::Statistics DisplayWindow_SDL::FrameClockStatistics() const
{
	return *d->mFrameClockStatistics;
}

struct DisplayWindow::RenderStatistics DisplayWindow_SDL::RenderStatistics() const {
	return *d->mRenderStatistics;
}

DisplayWindow& DisplayWindow_SDL::ConfigureBitmapBuffer(int w, int h)
{
	d->mBitmapBufferWidth = w;
	d->mBitmapBufferHeight = h;
	OffscreenBuffer &b = d->mScaledContentBuffer;
	WithLock(b)
		b.SetWidthPx(w).SetHeightPx(h);
	return *this;
}

void DisplayWindow_SDL::OnBitmapData(GUI::Bitmap& ioImage, const GUI::Rect* inpRectPx) const
{
	OffscreenBuffer &b = d->mScaledContentBuffer;
	if (b.Valid())
		WithLock(b)
			ioImage = d->mScaledContentBuffer.Bitmap();
	else
		GraphDisplay::OnBitmapData(ioImage, inpRectPx);
}

ClockTick DisplayWindow_SDL::CurrentFrame() const {
	return *d->mFrame;
}

#define PROPERTY_DEF(name, type, apply)                                                                                \
	DisplayWindow &DisplayWindow_SDL::Set##name(type value)                                                                \
	{                                                                                                                  \
		d->m##name = value;                                                                                            \
		return apply;                                                                                                  \
	}                                                                                                                  \
	type DisplayWindow_SDL::name() const                                                                                   \
	{                                                                                                                  \
		return d->m##name;                                                                                             \
	}

PROPERTY_DEF(Title, const std::string &, d->OnChange())
PROPERTY_DEF(Left, int, d->OnChange())
PROPERTY_DEF(Top, int, d->OnChange())
PROPERTY_DEF(Width, int, d->OnChange())
PROPERTY_DEF(Height, int, d->OnChange())
PROPERTY_DEF(Visible, bool, d->OnChange())
PROPERTY_DEF(CursorVisible, bool, d->OnChange())
PROPERTY_DEF(AsyncRendering, bool, d->OnChange())
PROPERTY_DEF(UpdateRateDecimation, int, *this)

DisplayWindow &DisplayWindow_SDL::SetFullscreen(bool b)
{
    if (b)
        bciwarn << "Cannot set window to fullscreen on this platform";
    return *this;
}

bool DisplayWindow_SDL::Fullscreen() const
{
    return false;
}

}
