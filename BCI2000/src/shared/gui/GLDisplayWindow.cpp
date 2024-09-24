////////////////////////////////////////////////////////////////////////////////
// $Id: GLDisplayWindow.cpp 8270 2024-07-23 19:34:53Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de, griffin.milsap@gmail.com
// Description: A GraphDisplay descendant which is a frameless GUI window for
//   an application's user display.
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
#include "GLDisplayWindow.h"

#include "Bitmap.h"
#include "DisplayClock.h"
#include "EnvVariable.h"
#include "FileUtils.h"
#include "OpenGLContext.h"
#include "SelfPipeQueue.h"
#include "SimpleStatistics.h"
#include "Thread.h"
#include "ThreadUtils.h"
#include "SynchronizedObject.h"
#include "BCIStream.h"

#if USE_QT
#include <QMouseEvent>
#include <QScreen>
#include <QThread>
#include <QWindow>
#endif

#if _WIN32
#include "dwmapi.h"
#endif

namespace
{
#if USE_QT
class GuiWindow : QWindow
{
  public:
    GuiWindow(GUI::GLDisplayWindow &inParent) : mrParent(inParent)
    {
        setFlags(Qt::FramelessWindowHint);
        setSurfaceType(QSurface::OpenGLSurface);
        setPosition(0, 0);
        Handle();
    }
    GUI::OSWindowHandle Handle()
    {
        return GUI::OSWindowHandle(winId());
    }
    void Adjust(GUI::Rect &outRect, bool &outScreenChanged)
    {
        QPlatformScreen *pOriginalScreen = this->screen()->handle();
        qreal originalRate = this->screen()->refreshRate();
        this->setTitle(QString::fromLocal8Bit(mrParent.Title().c_str()));
        this->resize(mrParent.Width(), mrParent.Height());
        this->setPosition(mrParent.Left(), mrParent.Top());
        if (mrParent.Visible() && mrParent.Fullscreen())
        {   
            // The showMinimized()/showNormal() sequence forces our window to the front.
            // It seems that only front windows can transition to be shown full screen.
            this->showMinimized();
            this->showNormal();
            this->showFullScreen();
        }
        else if (mrParent.Visible())
        {
            this->showNormal();
        }
        else
        {
            // Window is hidden.
            this->hide();
        }
#if _WIN32  // If we are visible, set DWM priority to "high" to avoid losing frames.
        ::DwmEnableMMCSS(this->isVisible());
#endif
        if (mrParent.CursorVisible())
            this->unsetCursor();
        else
            this->setCursor(Qt::BlankCursor);
        outScreenChanged = this->screen()->handle() != pOriginalScreen
                            || this->screen()->refreshRate() != originalRate;
        QRect currentRect = this->geometry();
        float devicePixelRatio = this->devicePixelRatio();
        outRect.left = currentRect.left() * devicePixelRatio;
        outRect.right = currentRect.right() * devicePixelRatio;
        outRect.top = currentRect.top() * devicePixelRatio;
        outRect.bottom = currentRect.bottom() * devicePixelRatio;
    }
    GUI::Rect PhysicalRect_mm() const
    {
        auto wpix = this->geometry(), spix = this->screen()->geometry();
        wpix.translate(-spix.topLeft());
        spix.translate(-spix.topLeft());
        GUI::Rect smm = PhysicalScreenRect_mm();
        GUI::Rect wmm = {wpix.left() * smm.Width() / spix.width(), wpix.top() * smm.Height() / spix.height(),
                         wpix.right() * smm.Width() / spix.width(), wpix.bottom() * smm.Height() / spix.height()};
        return wmm;
    }
    GUI::Rect PhysicalScreenRect_mm() const
    {
        auto size = screen()->physicalSize();
        GUI::Rect r = {0, 0, size.width(), size.height()};
        return r;
    }
    int LogicalDotsPerInchX() const
    {
        return this->screen()->logicalDotsPerInchX();
    }
    int LogicalDotsPerInchY() const
    {
        return this->screen()->logicalDotsPerInchY();
    }

  protected:
    void mousePressEvent(QMouseEvent *ev)
    {
        if (ev->button() == Qt::LeftButton)
        {
            GUI::Point p = {static_cast<float>(ev->pos().x()), static_cast<float>(ev->pos().y())};
            mrParent.Click(p);
            ev->accept();
        }
    }
    GUI::GLDisplayWindow &mrParent;
};
#endif // USE_QT

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

} // namespace

namespace GUI
{

struct GLDisplayWindow::Data
{
    GUI::GLDisplayWindow *mpSelf;

    std::string mTitle;
    float mTop, mLeft, mWidth, mHeight;
    bool mVisible, mFullscreen, mCursorVisible;
    std::atomic<bool> mAsyncRendering{ false };
    std::atomic<int> mUpdateRateDecimation{ 1 };

    SynchronizedObject<ClockTick, std::recursive_mutex> mFrame;
    SynchronizedObject<ClockTick::Statistics, std::recursive_mutex> mFrameClockStatistics;
    SynchronizedObject<struct RenderStatistics, std::recursive_mutex> mRenderStatistics;
    SynchronizedObject<struct PresentationStatistics, std::recursive_mutex> mPresentationStatistics;
    SynchronizedObject<StringUtils::NameValueList> mpInfo;

    GuiWindow *mpWindow;
    GUI::OpenGLContext *mpWindowGL;
    OffscreenBuffer mScaledContentBuffer;
    SelfPipeQueue<RenderState, 64> mRenderingQueue;

    int RenderingThreadFunc();
    struct RenderingThread : Thread
    {
        GLDisplayWindow::Data *d;
        RenderingThread(GLDisplayWindow::Data *d) : Thread("Rendering Thread"), d(d)
        {
        }
        int OnExecute()
        {
            try {
                return d->RenderingThreadFunc();
            }
            catch (const std::exception& e) {
                bcierr << e.what();
            }
            return -1;
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
    std::atomic<bool> mStopRendering{ false };

    GLDisplayWindow &OnChange();
    bool Adjust(GUI::DrawContext &);
    void Update(void *);

    Data(GLDisplayWindow *);
    ~Data();
};

GLDisplayWindow::Data::Data(GLDisplayWindow *pSelf)
    : mpSelf(pSelf), mTitle(FileUtils::ApplicationTitle()),
      mLeft(0), mTop(0), mWidth(0), mHeight(0),
      mVisible(false), mFullscreen(false), mCursorVisible(true),
      mRenderingThread(this), mpWindow(0), mpWindowGL(0), mFrame(ClockTick::Null)
{
    mpWindow = new GuiWindow(*pSelf);
    mAsyncRendering = false;
    mUpdateRateDecimation = 0;
}

GLDisplayWindow::Data::~Data()
{
    mAsyncRendering = false;
    mRenderingThread.Stop();
    delete mpWindowGL;
    delete mpWindow;
}

bool GLDisplayWindow::Data::Adjust(GUI::DrawContext &dc)
{
    bool screenChanged = false;
    GUI::Rect rect = { 0 };
    mpWindow->Adjust(rect, screenChanged);

    bool sizeChanged = false;
    if (rect.Width() != dc.rect.Width() || rect.Height() != dc.rect.Height())
    {
        dc.rect.left = 0;
        dc.rect.top = 0;
        dc.rect.right = rect.Width();
        dc.rect.bottom = rect.Height();
        sizeChanged = true;
    }
    if (sizeChanged || screenChanged || !mVisible)
    {
        mRenderingThread.Stop();
        mRenderingQueue.Clear();

        delete mpWindowGL;
        mpWindowGL = nullptr;
        if (dc.gl)
        {
            sizeChanged = true;
            dc.gl = 0;
        }
    }
    if (mVisible)
    {
        OpenGLContext *pGL = mpWindowGL;
        if (!pGL)
        {
            Assert(mpWindow->Handle());
            pGL = new OpenGLContext(mpWindow->Handle());
            if (pGL->Valid())
            {
                Assert(mpWindowGL == nullptr);
                mpWindowGL = pGL;
                GUI::Rect screen = mpWindow->PhysicalScreenRect_mm(), window = mpWindow->PhysicalRect_mm();
                sizeChanged = true;

                static const float mmPerInch = 25.4;
                float xpxPerMM = mpWindow->LogicalDotsPerInchX() / mmPerInch,
                      ypxPerMM = mpWindow->LogicalDotsPerInchY() / mmPerInch;
                WithLocked(&info = *mpInfo.Mutable())
                {
                    info.Clear();
                    info["GLVendor"] << pGL->Vendor();
                    info["GLRenderer"] << pGL->Renderer();
                    info["GLVersion"] << pGL->Version();
                    info["PhysicalWidth"] << window.Width() << "mm";
                    info["PhysicalHeight"] << window.Height() << "mm";
                    info["PhysicalLeftEdge"] << window.left << "mm";
                    info["PhysicalTopEdge"] << window.top << "mm";
                    info["PhysicalScreenWidth"] << screen.Width() << "mm";
                    info["PhysicalScreenHeight"] << screen.Height() << "mm";
                    info["LogicalResolutionX"] << xpxPerMM << "px/mm";
                    info["LogicalResolutionY"] << ypxPerMM << "px/mm";
                }
                mRenderingThread.Start();
            }
            else
                delete pGL;
        }
    }
    return sizeChanged || screenChanged;
}

void GLDisplayWindow::Data::Update(void *p)
{
    mRenderingQueue.Push(RenderState(-1, TimeUtils::MonotonicTime(), p));
}

int GLDisplayWindow::Data::RenderingThreadFunc()
{
    OpenGLContext renderGL(mpWindowGL);
    Framebuffer *pContentBuffer = nullptr;
    GUI::DrawContext dc = mpSelf->Context();
    WithGLContext(renderGL)
    {
        int maxTextureSize = renderGL.MaxTextureSize();
        if (dc.rect.Width() > maxTextureSize || dc.rect.Height() > maxTextureSize)
        {
            throw std_runtime_error << "Window dimensions exceed maximum texture size of "
                                    << maxTextureSize << "x" << maxTextureSize;
        }
        pContentBuffer = new Framebuffer(dc.rect.Width(), dc.rect.Height());
        renderGL.SetRenderTarget(pContentBuffer);
        renderGL.Clear(mpSelf->Color());
    }
    dc.gl = &renderGL;
    mpSelf->SetContext(dc);
    WithLock(mScaledContentBuffer) mScaledContentBuffer.Create(&renderGL, mpSelf->Color());

    AV::DisplayClock clock(mpWindow->Handle());
    ClockTick frame = clock.LastVbl();
    WithLocked(p = mFrameClockStatistics.Mutable())
    {
        ::memset(&*p, 0, sizeof(*p));
        p->type = frame.type;
    }
    WithLocked(p = mPresentationStatistics.Mutable())::memset(&*p, 0, sizeof(*p));
    WithLocked(p = mRenderStatistics.Mutable())::memset(&*p, 0, sizeof(*p));
    WithLocked(p = mFrame.Mutable())
    {
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
    WithThreadTask(task, ThreadUtils::Priority::Maximum)
    {
        const Time::Interval timeSlice = Time::Seconds(1e-3);
        ThreadUtils::TimeSlice ts(timeSlice);
        while (!mStopRendering)
        {
            // update to current frame
            frame = clock.LastVbl();
            int frameDelta;
            Time::Interval timeDelta;
            WithLocked(p = mFrame.Mutable())
            {
                timeDelta = frame.time - p->time;
                frameDelta = Round((frame.time - p->time) * frame.rate);
                frame.count = p->count + frameDelta;
                *p = frame;
            }
            // update frame clock statistics
            for (int i = 0; i < frameDelta; ++i)
                frameObs.Observe(timeDelta.Seconds() / frameDelta);
            WithLocked(p = mFrameClockStatistics.Mutable())
            {
                if (frameDelta > framesPerUpdate)
                    p->missed += (frameDelta - 1) / framesPerUpdate;
                p->interval = Time::Seconds(frameObs.Mean());
                p->jitter = Time::Seconds(::sqrt(frameObs.Variance()));
            }
            // determine rendering deadline
            ClockTick nextFrame = frame;
            nextFrame.count += framesPerUpdate;
            nextFrame.time += framesPerUpdate * frame.rate.Period();

            Time renderBegin = TimeUtils::MonotonicTime();
            // pop all requests from rendering queue
            for (RenderState r; mRenderingQueue.Pop(r);)
            {
                if (r.contentFrame < 0 || r.contentFrame > nextFrame.count)
                    r.contentFrame = nextFrame.count;
                requests.push_back(r);
            }
            // update GraphObjects state
            if (!requests.empty())
            {
                bool presentation = mpSelf->Tick(nextFrame);
                for (auto &r : requests)
                {
                    r.presentation = presentation;
                    mpSelf->Emit(OnUpdateReceived, &r);
                }
            }
            if (mpSelf->InvalidRegion().Empty())
                requests.clear();
            else
            { // paint into content buffer
                mpSelf->Paint();
                auto &b = mScaledContentBuffer;
                if (b.Gl())
                    WithLock(b) WithGLContext(b.Gl())
                    {
                        // copy content buffer to visualization backbuffer
                        b.Gl()->Clear(mpSelf->Color());
                        pContentBuffer->Front()->CopyToCurrentViewport();
                        b.Sync();
                    }
                Time renderEnd = TimeUtils::MonotonicTime();
                renderObs.Observe((renderEnd - renderBegin).Seconds());
            }

            Time blitBegin = TimeUtils::MonotonicTime(), swapEnd;
            Time timeDone = nextFrame.time - nextFrame.rate.Period() / 2;
            WithGLContext(mpWindowGL) do
            {
                // copy content buffer to OpenGL display backbuffer
                mpWindowGL->SetViewport(0, 0, dc.rect.Width(), dc.rect.Height());
                mpWindowGL->Clear(mpSelf->Color());
                pContentBuffer->Front()->CopyToCurrentViewport();
                // Wait for vbl and swap OpenGL back and front buffers
                mpWindowGL->SwapBuffers();
                if (mpWindowGL->SwapInterval() < 1)
                    clock.WaitForVbl();
                swapEnd = clock.LastVbl().time;
                if (mpWindowGL->SoftwareRasterized())
                    while (swapEnd < nextFrame.time)
                        swapEnd = clock.WaitForVbl().LastVbl().time;
            } while (swapEnd < timeDone);

            // time to adapt scaled content buffer
            if (!mScaledContentBuffer.Valid())
                WithLock(mScaledContentBuffer)
                {
                    mScaledContentBuffer.Destroy();
                    mScaledContentBuffer.Create(&renderGL, mpSelf->Color());
                }

            // UpdateRateDecimation property may have changed
            framesPerUpdate = mUpdateRateDecimation;
            bool adaptUpdateRate = framesPerUpdate < 1;
            if (adaptUpdateRate)
                framesPerUpdate = 1;

            // update render statistics
            WithLocked(p = mRenderStatistics.Mutable())
            {
                p->render.mean = Time::Seconds(renderObs.Mean());
                p->render.std = Time::Seconds(::sqrt(renderObs.Variance()));
                if (adaptUpdateRate && renderObs.Count() > goodStatisticsMinCount)
                    framesPerUpdate = Ceil((p->render.mean + p->render.std) * frame.rate);
                Assert(framesPerUpdate > 0);
                p->decimation = framesPerUpdate;
            }

            // update presentation statistics
            //   displayTime is VBL time _before_ transmission of full frame
            //   this is lower bound for CRT, and one frame short for FP displays
            int frameDelay = Floor((swapEnd - nextFrame.time) * frame.rate);
            int64_t displayFrame = nextFrame.count + frameDelay;
            Time displayTime = nextFrame.time + frameDelay * frame.rate.Period();
            for (auto &r : requests)
            {
                r.displayFrame = displayFrame;
                r.displayed = displayTime;
                if (r.contentFrame < r.displayFrame)
                    ++mPresentationStatistics.Mutable()->late;
                if (!IsNaN(r.requested))
                    latencyObs.Observe((r.displayed - r.requested).Seconds());
                mpSelf->Emit(OnUpdateDisplayed, &r);
            }
            WithLocked(p = mPresentationStatistics.Mutable())
            {
                p->count += requests.empty() ? 0 : 1;
                if (latencyObs.Count() == 0)
                {
                    p->latency.mean = Time::Forever();
                    p->latency.std = Time::Forever();
                }
                else
                {
                    p->latency.mean = Time::Seconds(latencyObs.Mean());
                    p->latency.std = Time::Seconds(::sqrt(latencyObs.Variance()));
                }
            }
            requests.clear();

            // Request next frame
            if (mAsyncRendering && !mStopRendering)
                mRenderingQueue.Push(RenderState(displayFrame + framesPerUpdate));
        }
    }
    mRenderingQueue.Clear();
    WithLock(mScaledContentBuffer) mScaledContentBuffer.Destroy();
    delete pContentBuffer;
    dc = mpSelf->Context();
    dc.gl = nullptr;
    mpSelf->SetContext(dc);
    return 0;
}

GLDisplayWindow &GLDisplayWindow::Data::OnChange()
{
    DrawContext dc = mpSelf->Context();
    if (Adjust(dc))
        mpSelf->SetContext(dc);
    return *mpSelf;
}

// GLDisplayWindow
GLDisplayWindow::GLDisplayWindow() : d(new Data(this))
{
}

GLDisplayWindow::~GLDisplayWindow()
{
    delete d;
}

#define PROPERTY_DEF(name, type, apply)                                                                                \
    DisplayWindow &GLDisplayWindow::Set##name(type value)                                                                \
    {                                                                                                                  \
        d->m##name = value;                                                                                            \
        return apply;                                                                                                  \
    }                                                                                                                  \
    type GLDisplayWindow::name() const                                                                                   \
    {                                                                                                                  \
        return d->m##name;                                                                                             \
    }

PROPERTY_DEF(Title, const std::string &, d->OnChange())
PROPERTY_DEF(Left, int, d->OnChange())
PROPERTY_DEF(Top, int, d->OnChange())
PROPERTY_DEF(Width, int, d->OnChange())
PROPERTY_DEF(Height, int, d->OnChange())
PROPERTY_DEF(Visible, bool, d->OnChange())
PROPERTY_DEF(Fullscreen, bool, d->OnChange())
PROPERTY_DEF(CursorVisible, bool, d->OnChange())
PROPERTY_DEF(AsyncRendering, bool, d->OnChange())
PROPERTY_DEF(UpdateRateDecimation, int, *this)

DisplayWindow &GLDisplayWindow::Update(void *p)
{
    d->Update(p);
    return *this;
}

ClockTick GLDisplayWindow::CurrentFrame() const
{
    return *d->mFrame;
}

ClockTick::Statistics GLDisplayWindow::FrameClockStatistics() const
{
    return *d->mFrameClockStatistics;
}

struct GUI::DisplayWindow::RenderStatistics GLDisplayWindow::RenderStatistics() const
{
    return *d->mRenderStatistics;
}

struct DisplayWindow::PresentationStatistics GLDisplayWindow::PresentationStatistics() const
{
    return *d->mPresentationStatistics;
}

StringUtils::NameValueList GLDisplayWindow::Info() const
{
    return *d->mpInfo;
}

DisplayWindow &GLDisplayWindow::ConfigureBitmapBuffer(int inWidthPx, int inHeightPx)
{
    OffscreenBuffer &b = d->mScaledContentBuffer;
    WithLock(b) 
      b.SetWidthPx(inWidthPx).SetHeightPx(inHeightPx);
    return *this;
}

void GLDisplayWindow::OnBitmapData(GUI::Bitmap &ioImage, const GUI::Rect *inpRectPx) const
{
    OffscreenBuffer &b = d->mScaledContentBuffer;
    if (b.Valid())
        WithLock(b) ioImage = d->mScaledContentBuffer.Bitmap();
    else
        GraphDisplay::OnBitmapData(ioImage, inpRectPx);
}

} // namespace GUI
