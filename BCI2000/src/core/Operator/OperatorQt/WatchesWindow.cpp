//////////////////////////////////////////////////////////////////////
// $Id: WatchesWindow.cpp 8081 2024-04-27 11:45:06Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A window displaying user-editable expressions.
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
///////////////////////////////////////////////////////////////////////
#include "WatchesWindow.h"
#include "Debugging.h"
#include "Exception.h"
#include "MainWindow.h"
#include "OperatorUtils.h"
#include "TimeUtils.h"
#include "ThreadUtils.h"
#include "Preferences.h"
#include "PrecisionTime.h"

#include <QBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QPicture>
#include <QPainter>
#include <QPushButton>
#include <QStatusBar>
#include <QTableWidget>
#include <QStaticText>
#include <QStyledItemDelegate>

#include <deque>
#include <vector>
#include <mutex>

#ifndef SHOW_MARKED_SAMPLES // debug samples marked for ignore when determining scale
#define SHOW_MARKED_SAMPLES 0
#endif

#ifndef DEBUG_POLYLINE // draw elementary lines colored according to distinction of cases
#define DEBUG_POLYLINE 0
#endif

#ifndef DRAW_FULL_POLYLINE // draw all lines
#define DRAW_FULL_POLYLINE 0
#endif

#ifndef ENSURE_HISTORY_SORTED
#define ENSURE_HISTORY_SORTED 0
#endif

namespace
{
// https://www.w3.org/TR/SVG/types.html#ColorKeywords
const QString sPlotColors[] = {
    "<text>", "red", "green", "blue", "blueviolet",
    "gray",  "lightgray", "fuchsia", "orangered"
};
const int sNumPlotColors = sizeof(sPlotColors) / sizeof(*sPlotColors);

const float sBarWidth = 3;
const float sMaxLinesPerPixel = 10;

enum
{
    expressionCol,
    decimationCol,
    valueCol,
    rangeCol,
    plotCol,
    numColumns
};

static const double cDefaultDuration = 20; // seconds

QBrush GetBrush(int idx)
{
    QBrush brush;
    const QString& colorName = sPlotColors[idx % sNumPlotColors];
    if (colorName == "<text>")
        brush = QPalette().text();
    else
        brush = QColor(colorName);
    return brush;
}

QString GetColorName(int idx)
{
    return GetBrush(idx).color().name();
}

// Draw a polyline as fast and efficiently as possible to avoid performance issues.
// Draws a maximum of maxLinesPerPixel lines over the range of a single pixel.
void DrawPolyline(QPainter& painter, const std::vector<QPointF>& points, int maxLinesPerPixel)
{
#if DRAW_FULL_POLYLINE
    painter.drawPolyline(points.data() + 1, points.size() - 1);
    return;
#endif

    const float deltaX = 1.0f / maxLinesPerPixel;
    int idxBegin = 1;
    while (idxBegin < points.size() - 1)
    {
        int idxEnd = idxBegin + 2; // point range: point at idxBegin and the following
        // extend point range as long as points are within the deltaX limit
        while (idxEnd < points.size() && points[idxEnd].x() - points[idxBegin].x() < deltaX)
            ++idxEnd;

        // determine x and y ranges
        float ymax = points[idxBegin].y(),
            ymin = ymax;
        for (int i = idxBegin + 1; i < idxEnd; ++i)
        {
            ymax = std::max<float>(ymax, points[i].y());
            ymin = std::min<float>(ymin, points[i].y());
        }
        float xmin = points[idxBegin].x(),
            xmax = points[idxEnd - 1].x();
        // reduce range to a single line depending on orientation
        if (ymax == ymin)
        {   // horizonal line with low resolution
#if DEBUG_POLYLINE
            painter.setPen(Qt::black);
#endif
            painter.drawLine(xmin, ymax, xmax, ymax);
        }
        else if (xmin == xmax)
        {
            // vertical line with low resolution
#if DEBUG_POLYLINE
            painter.setPen(Qt::blue);
#endif
            painter.drawLine(xmin, ymin, xmin, ymax);
        }
        else
        {   // multiple lines with resolution above maxLinesPerPixel,
            // replace with single vertical line representing ymin and ymax
            float xcenter = (xmax + xmin) / 2;
#if DEBUG_POLYLINE
            painter.setPen(Qt::red);
#endif
            painter.drawLine(xcenter, ymin, xcenter, ymax);
        }
        idxBegin = idxEnd - 1;
    }
}

struct RangeWidget : QWidget
{
    void SetRange(double min, double max)
    {
        mText.setText(QString("%1 ... %2").arg(min).arg(max));
        update();
    }
    void paintEvent(QPaintEvent *) override
    {
        QSizeF size = mText.size();
        qreal left = (rect().width() - size.width()) / 2;
        qreal top = (rect().height() - size.height()) / 2;
        QPainter(this).drawStaticText(QPoint(left, top), mText);
    }
    QStaticText mText;
};

struct ValueWidget : QWidget // could be a QLabel but that sends expensive events on setText()
{
    void setText(const QString& text)
    {
        mText.setText(text);
        update();
    }
    void clear()
    {
        mText.setText("");
        update();
    }
    void paintEvent(QPaintEvent*) override
    {
        QSizeF size = mText.size();
        qreal left = (rect().width() - size.width()) / 2;
        qreal top = (rect().height() - size.height()) / 2;
        QPainter(this).drawStaticText(QPoint(left, top), mText);
    }
    QStaticText mText;
};

QString ParseDecimation(const QString& inDecimation, int& value)
{
    value = inDecimation.toInt();
    if (!inDecimation.compare("auto", Qt::CaseInsensitive) || value < 1) {
        value = -1;
        return "auto";
    }
    return QString::number(value);
}

static struct DecimationItemDelegate : QStyledItemDelegate
{
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QWidget* pEditor = QStyledItemDelegate::createEditor(parent, option, index);
        QLineEdit* pLineEdit = dynamic_cast<QLineEdit*>(pEditor);
        if (pLineEdit)
            pLineEdit->setAlignment(Qt::AlignCenter);
        return pEditor;
    }
} sDecimationItemDelegate;

QTableWidgetItem* CreateDecimationItem(const QString& inDecimation, int* pValue = nullptr)
{
    int value = 0;
    QString text = ParseDecimation(inDecimation, value);
    if (pValue)
        *pValue = value;
    QTableWidgetItem* pItem = new QTableWidgetItem(text);
    pItem->setTextAlignment(Qt::AlignCenter);
    return pItem;
}

struct WatchPlot : QWidget
{
  typedef std::list<std::vector<double>> HistoryType;

  WatchPlot(double maxTime, const double &now, RangeWidget *pRangeWidget)
        : mMaxTime(maxTime), mrNow(now), mpRangeWidget(pRangeWidget), mMarkNextSample(true),
          mRangeMin(WatchesWindow::nan),
          mRangeMax(WatchesWindow::nan)
    {
    }

    void SetFixedRange(double min, double max)
    {
         mRangeMin = min;
         mRangeMax = max;
    }

    void MarkMostRecentSample()
    {
        std::lock_guard<std::mutex> lock(mMutex);
        if (mHistory.empty())
            return;
        mHistory.back().back() = true;
    }

    void MarkNextSample()
    {
      std::lock_guard<std::mutex> lock(mMutex);
      mMarkNextSample = true;
    }

    void SetMaxTime(double maxTime)
    {
        Assert(ThreadUtils::InMainThread());
        mMaxTime = maxTime;
        double now = TimeUtils::MonotonicTime().Seconds();
        auto it = mHistory.begin();
        while (it != mHistory.end() && now - (*it)[0] > mMaxTime)
            ++it;
        if (it != mHistory.begin())
            --it;
        while (it != mHistory.begin())
            mHistory.pop_front();
    }

    void AddDataPoint(const QString &inData, double inTime)
    {
        Assert(ThreadUtils::InMainThread());
        std::lock_guard<std::mutex> lock(mMutex);
        auto values = inData.split('\t');
        values.pop_front();
        if (!mHistory.empty() && mHistory.front().size() != values.size() + 2)
            mHistory.clear();
        std::vector<double> data(1, inTime);
        data.reserve(values.size() + 2);
        for (const auto &value : qAsConst(values))
        {
            bool ok = false;
            double d = value.toDouble(&ok);
            if (!ok)
                d = WatchesWindow::nan;
            data.push_back(d);
        }
        data.push_back(mMarkNextSample);
        mMarkNextSample = false;
        InsertIntoHistory(data);
        SetMaxTime(mMaxTime);
        update();
    }

    bool HistoryIsSortedAndUnique()
    {
        double t = 0;
        for (const auto& d : mHistory)
        {
            if (d.front() <= t)
                return false;
            t = d.front();
        }
        return true;
    }

    // Check whether values at data point1 equal data point2, and propagate mark if so.
    void PropagateMark(const std::vector<double>& data1, std::vector<double>& data2)
    {
        double marked = data1.back();
        if (!marked)
            return;

        bool isEqual = true;
        for (int idx = 1; isEqual && idx < data1.size() - 1; ++idx)
            isEqual = isEqual && data1[idx] == data2[idx];
        if (isEqual)
            data2.back() = marked;
    }

    void InsertIntoHistory(const std::vector<double>& data)
    {   // Data points may arrive out of order, so we must
        // spend some effort to keep the history ordered by timestamp.
        // We assume that history is always sorted by timestamp.
        // Also, data points may appear multiple times for the same
        // timestamp, so we must replace timestamps that already exist.
        if (data.empty())
            return;
        if (mHistory.empty())
        {
            mHistory.push_back(data);
            return;
        }
#if ENSURE_HISTORY_SORTED
        // ensure precondition
        if (!HistoryIsSortedAndUnique())
            throw std_runtime_error << "History is not sorted and unique";
#endif

        auto i = mHistory.rbegin();
        HistoryType::iterator j; // forward iterator pointing at new data
        while (i != mHistory.rend() && data.front() < i->front())
            ++i;
        if (i != mHistory.rend() && i->front() == data.front())
        {
            // If we replace an entry that is marked, mark the replacement, too.
            double marked = i->back() || data.back();
            *i = data;
            i->back() = marked;
            j = i.base();
        }
        else
        {
            j = mHistory.insert(i.base(), data);
        }
        // If two subsequent entries have the same values, and the first one is marked,
        // mark the second one, too.
        // We need only consider the new entry's predecessor and successor.
        if (j != mHistory.begin())
        {
            auto k = j--; // j points to entry n-1, k to entry n
            if (k != mHistory.end())
               PropagateMark(*j, *k);
            ++j;
        }
        if (j != mHistory.end())
        {
            auto k = j++; // k points to entry n, j to entry n+1
            if (j != mHistory.end())
                PropagateMark(*k, *j);
            --j;
        }
#if ENSURE_HISTORY_SORTED
        // ensure postcondition
        if (!HistoryIsSortedAndUnique())
            throw std_runtime_error << "History is not sorted and unique";
#endif
    }

    void paintEvent(QPaintEvent *) override
    {
        Assert(ThreadUtils::InMainThread());
        std::lock_guard<std::mutex> lock(mMutex);
        SetMaxTime(mMaxTime);
        if (!mHistory.empty())
        {
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing);
          
            QRectF r = rect().adjusted(1, 1, -1, -1);
            QPointF origin(r.left(), r.top() + r.height() / 2);
            float tscale = r.width() / mMaxTime,
                  vmax = -Inf<float>(), vmin = Inf<float>(),
                  markedmax = vmax, markedmin = vmin;
            float t1 = mrNow, t0 = mrNow - ::fmod(t1, mMaxTime);
            float xbar = tscale * (t1 - t0) + origin.x();

            mLineData.resize(2 * mHistory.size() + 1);
#if SHOW_MARKED_SAMPLES
            std::vector<bool> marked(mLineData.size());
#endif
            // Draw from the left, so the more recent part is visible to the left
            // of the moving bar.
            int i = 0;
            for (const auto &point : mHistory)
            {
#if SHOW_MARKED_SAMPLES
                marked[i+1] = point.back();
#endif
                float x = tscale * (point[0] - t0) + origin.x();
                x = std::min(x, xbar + sBarWidth);
                mLineData[i++].setX(x);
                mLineData[i++].setX(x);
                if (point.back()) // ignore marked data points for scaling purposes
                {
                    for (int j = 1; j < point.size() - 1; ++j)
                    {
                        markedmax = std::max<float>(markedmax, point[j]);
                        markedmin = std::min<float>(markedmin, point[j]);
                    }
                }
                else
                {
                    for (int j = 1; j < point.size() - 1; ++j)
                    {
                        vmax = std::max<float>(vmax, point[j]);
                        vmin = std::min<float>(vmin, point[j]);
                    }
                }
            }
            mLineData[i++].setX(xbar);

            float vscale = 0, v0 = 0;
            if (!std::isnan(mRangeMin) && !std::isnan(mRangeMax))
            {
                vscale = r.height() / (mRangeMax - mRangeMin + 1);
                v0 = (mRangeMax + mRangeMin) / 2;
            }
            else if (vmax > -Inf<float>() && vmin < Inf<float>())
            {
                vscale = r.height() / (vmax - vmin + 1);
                v0 = (vmax + vmin) / 2;
            }

            for (int j = 1; j < mHistory.front().size() - 1; ++j)
            {
                int i = 1;
                for (const auto &point : mHistory)
                {
                    float y = origin.y() - vscale * (point[j] - v0);
                    mLineData[i++].setY(y);
                    mLineData[i++].setY(y);
                }
#if SHOW_MARKED_SAMPLES
                for (int i = 1; i < mLineData.size() - 1; ++i)
                {
                    painter.setPen(marked[i] ? Qt::red : Qt::black);
                    painter.drawLine(mLineData[i], mLineData[i+1]);
                }
#else
                painter.setPen(QPen(GetBrush(j - 1), 1));
                DrawPolyline(painter, mLineData, sMaxLinesPerPixel);
#endif
            }

            // Shift by one window to the right, so the older part is visible to the right
            // of the moving bar.
            i = 0;
            for (const auto& point : mHistory)
            {
#if SHOW_MARKED_SAMPLES
                marked[i+1] = point.back();
#endif
                float x = tscale * (point[0] - t0 + mMaxTime) + origin.x();
                x = std::max(x, xbar);
                mLineData[i++].setX(x);
                mLineData[i++].setX(x);
            }
            mLineData[i++].setX(tscale * (t1 - t0 + mMaxTime) + origin.x());

            for (int j = 1; j < mHistory.front().size() - 1; ++j)
            {
                int i = 1;
                for (const auto& point : mHistory)
                {
                    float y = origin.y() - vscale * (point[j] - v0);
                    mLineData[i++].setY(y);
                    mLineData[i++].setY(y);
                }
#if SHOW_MARKED_SAMPLES
                for (int i = 1; i < mLineData.size() - 1; ++i)
                {
                    painter.setPen(marked[i] ? Qt::red : Qt::black);
                    painter.drawLine(mLineData[i], mLineData[i + 1]);
                }
#else
                painter.setPen(QPen(GetBrush(j - 1), 1));
                DrawPolyline(painter, mLineData, sMaxLinesPerPixel);
#endif
            }

            // Paint the moving bar.
            QRectF bar(xbar, rect().top(), sBarWidth, rect().height());
            painter.fillRect(bar, Qt::gray);

            painter.end();

#if SHOW_MARKED_SAMPLES
            if (mpRangeWidget)
              mpRangeWidget->SetRange(vmin, vmax);
#else
            if (mpRangeWidget)
              mpRangeWidget->SetRange(std::min(vmin, markedmin), std::max(vmax, markedmax));
#endif
        }
    }

    HistoryType mHistory;
    bool mMarkNextSample;
    double mMaxTime;
    const double &mrNow;
    RangeWidget *mpRangeWidget;
    double mRangeMin, mRangeMax;
    std::vector<QPointF> mLineData;
    std::mutex mMutex;
};

struct WindowState
{
    double historyLength;
    int colWidth[numColumns];
};

WatchesWindow * spInstance = nullptr;
std::mutex sInstanceMutex;

} // namespace

struct WatchesWindow::Watch : ValueWidget, bci::OperatorModule::ExpressionWatch
{
    Watch(WatchPlot *pPlot) : mpPlot(pPlot)
    {
    }
    void OnValuesChanged(int count, const char * const* inValues) override
    {
        QStringList values;
        for (int i = 0; i < count; ++i)
          values.append(inValues[i]);
        std::unique_lock<std::mutex> lock(sInstanceMutex);
        if (spInstance)
          QMetaObject::invokeMethod(spInstance, "OnValuesReceived", Qt::AutoConnection, 
                                    Q_ARG(void*, this), Q_ARG(QStringList, values));
    }
    WatchPlot *mpPlot;
};

void WatchesWindow::Initialize(MainWindow *parent)
{
    std::unique_lock<std::mutex> lock(sInstanceMutex);
    Assert(!spInstance);
    spInstance = new WatchesWindow(parent);
}

void WatchesWindow::Deinitialize()
{
    std::unique_lock<std::mutex> lock(sInstanceMutex);
    WatchesWindow* pInstance = spInstance;
    spInstance = nullptr;
    lock.unlock();
    delete pInstance;
}

WatchesWindow *WatchesWindow::Instance()
{
    return spInstance;
}

WatchesWindow::WatchesWindow(MainWindow *parent)
    : mpMenuEntry(nullptr), mpTable(nullptr), mHistoryLengthSeconds(cDefaultDuration), mLastUpdate(-1),
      mUpdateInterval(1.0 / 24 /* seconds */), mTimeSlice(Time::FromMilliseconds(1))
{
    setWindowTitle(Preferences::Instance()->mInstancePrefix + parent->windowTitle() + " Watches");
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
                   Qt::WindowCloseButtonHint);
    setAttribute(Qt::WA_QuitOnClose, false);
    mpMenuEntry = parent->GetWindowMenu()->addAction("Watches");
    mpMenuEntry->setShortcut(QKeySequence("Ctrl+Alt+W"));
    mpMenuEntry->setCheckable(true);
    mpMenuEntry->setChecked(this->isVisible());
    connect(mpMenuEntry, SIGNAL(triggered(bool)), this, SLOT(setVisible(bool)));

    QAction *pAction = new QAction("Save Watches", this);
    connect(pAction, SIGNAL(triggered(bool)), this, SLOT(SaveWatches()));
    addAction(pAction);
    pAction = new QAction("Restore Watches", this);
    connect(pAction, SIGNAL(triggered(bool)), this, SLOT(RestoreWatches()));
    addAction(pAction);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    QLayout *pLayout = new QVBoxLayout;
    pLayout->setContentsMargins(0, 0, 0, 0);

    mpTable = new QTableWidget(this);
    mpTable->setColumnCount(numColumns);
    mpTable->setColumnWidth(numColumns - 1, 10);
    mpTable->horizontalHeader()->setStretchLastSection(true);
    mpTable->verticalHeader()->setVisible(false);
    mpTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mpTable->setSelectionMode(QAbstractItemView::SingleSelection);
    mpTable->setSelectionBehavior(QAbstractItemView::SelectItems);
    mpTable->setItemDelegateForColumn(decimationCol, &sDecimationItemDelegate);
    connect(mpTable->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)), SLOT(OnHeaderDoubleClick(int)));
    connect(mpTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(OnItemChanged(QTableWidgetItem*)));
    pLayout->addWidget(mpTable);

    QStatusBar *pStatusBar = new QStatusBar;
    pStatusBar->setSizeGripEnabled(true);
    pLayout->addWidget(pStatusBar);

    QPushButton *pButton = new QPushButton("Save Watches");
    connect(pButton, SIGNAL(clicked()), this, SLOT(SaveWatches()));
    pStatusBar->addWidget(pButton);
    pButton = new QPushButton("Restore Watches");
    connect(pButton, SIGNAL(clicked()), this, SLOT(RestoreWatches()));
    pStatusBar->addWidget(pButton);
    pButton = new QPushButton("Clear Watches");
    connect(pButton, SIGNAL(clicked()), this, SLOT(ClearWatches()));
    pStatusBar->addWidget(pButton);

    setLayout(pLayout);

    AddEmptyLine();
    QByteArray state;
    if (!OperatorUtils::RestoreWidget(this, &state))
        setGeometry(100, 100, 400, 100);
    if (state.isEmpty())
    {
        mpTable->setColumnWidth(valueCol, mpTable->columnWidth(valueCol) / 2);
        mpTable->setColumnWidth(rangeCol, mpTable->columnWidth(rangeCol) / 2);
        SetHistoryLengthSeconds(cDefaultDuration);
    }
    else
    {
        union {
            const char *c;
            WindowState *s;
        } pWindowState = {state.data()};
        for (int col = 0; col < numColumns - 1; ++col)
            mpTable->setColumnWidth(col, pWindowState.s->colWidth[col]);
        SetHistoryLengthSeconds(pWindowState.s->historyLength);
    }
    startTimer(mUpdateInterval * 1e3 + 1);
}

WatchesWindow::~WatchesWindow()
{
    union {
        WindowState s;
        char c[sizeof(WindowState)];
    } windowState = {0};
    windowState.s.historyLength = mHistoryLengthSeconds;
    for (int col = 0; col < numColumns; ++col)
        windowState.s.colWidth[col] = mpTable->columnWidth(col);
    auto bytes = QByteArray(windowState.c, sizeof(windowState));
    OperatorUtils::SaveWidget(this, &bytes);
}

void WatchesWindow::timerEvent(QTimerEvent *)
{
    Update();
}

void WatchesWindow::Update()
{
    double now = TimeUtils::MonotonicTime().Seconds();
    if (now - mLastUpdate > 0.9 * mUpdateInterval)
    {
        for (int i = 0; i < mpTable->rowCount(); ++i)
        {
            QWidget *pWidget = mpTable->cellWidget(i, plotCol);
            if (pWidget)
                pWidget->update();
        }
        mLastUpdate = now;
    }
}

void WatchesWindow::MarkMostRecentSamples()
{
    for (int i = 0; i < mpTable->rowCount(); ++i)
    {
        QWidget* pWidget = mpTable->cellWidget(i, plotCol);
        WatchPlot* pPlot = dynamic_cast<WatchPlot*>(pWidget);
        if (pPlot)
            pPlot->MarkMostRecentSample();
    }
}

void WatchesWindow::MarkNextSamples()
{
  for (int i = 0; i < mpTable->rowCount(); ++i)
  {
    QWidget* pWidget = mpTable->cellWidget(i, plotCol);
    WatchPlot* pPlot = dynamic_cast<WatchPlot*>(pWidget);
    if (pPlot)
      pPlot->MarkNextSample();
  }
}

void WatchesWindow::closeEvent(QCloseEvent *)
{
    if (mpMenuEntry)
        mpMenuEntry->setChecked(false);
}

void WatchesWindow::paintEvent(QPaintEvent* ev)
{
    mLastUpdate = TimeUtils::MonotonicTime().Seconds();
    QWidget::paintEvent(ev);
}

void WatchesWindow::OnHeaderDoubleClick(int idx)
{
    if (sender() == mpTable->horizontalHeader() && idx == plotCol)
    {
        bool ok = false;
        double length = QInputDialog::getDouble(
            this, "Edit history length", "New history length in seconds:", mHistoryLengthSeconds, 0, 3600, 1, &ok);
        if (ok)
            SetHistoryLengthSeconds(length);
    }
}

namespace {
    void ParseCommas(std::string& s)
    { // *inside of expressions, leave commas alone
      // *outside of expressions, replace commas with \t
      // *expressions can only have commas inside pairs of brackets ( ),
      // so all we need to do is count brackets while we process the string.
        int brackets = 0;
        for (auto& c : s)
        {
            switch (c)
            {
                case '(':
                    ++brackets;
                    break;
                case ')':
                    --brackets;
                    break;
                case ',':
                    if (!brackets)
                        c = '\t';
                    break;
            }
        }
    }
}


void WatchesWindow::OnItemChanged(QTableWidgetItem *pItem)
{
    if (pItem->column() == expressionCol) {

        QString data = pItem->data(Qt::DisplayRole).toString().trimmed();
        std::string expressions = data.toStdString();
        if (!expressions.empty() && expressions.front() == '{' && expressions.back() == '}')
        {
            expressions = expressions.substr(1, expressions.length() - 2);
            ParseCommas(expressions);
        }

        int row = pItem->row();
        Watch *pWatch = dynamic_cast<Watch *>(mpTable->cellWidget(row, valueCol));
        if (pWatch)
        {
            pWatch->clear();
            pWatch->Register(expressions.c_str());
            int decimation = -1;
            QTableWidgetItem* pItem = CreateDecimationItem("auto", &decimation);
            mpTable->setItem(row, decimationCol, pItem);
            pWatch->SetDecimation(decimation);
        }
        if (data.isEmpty())
            mpTable->removeRow(row);
        if (mpTable->rowCount() < 1)
            AddEmptyLine();
        else
        {
            QTableWidgetItem *pLastLineItem = mpTable->item(mpTable->rowCount() - 1, expressionCol);
            if (pLastLineItem && !pLastLineItem->data(Qt::DisplayRole).toString().trimmed().isEmpty())
                AddEmptyLine();
        }
    }
    else if (pItem->column() == decimationCol) {
        int decimation = -1;
        QString text = ParseDecimation(pItem->text(), decimation);
        pItem->setText(text);
        int row = pItem->row();
        Watch* pWatch = dynamic_cast<Watch*>(mpTable->cellWidget(row, valueCol));
        if (pWatch)
            pWatch->SetDecimation(decimation);
    }
}

void WatchesWindow::OnValuesReceived(void *pData, const QStringList& values)
{
    Watch *pWatch = static_cast<Watch *>(pData);
    // Watch may no longer exist by the time this is executed in the main thread
    bool found = false;
    for (int row = 0; row < mpTable->rowCount(); ++row)
    {
        if (pWatch == mpTable->cellWidget(row, valueCol))
        {
            found = true;
            break;
        }
    }
    if (!found)
        return;

    for (const auto& value : values)
    {
        QStringList valueList = value.split('\t');
        double timestamp = -1;
        if (!valueList.empty())
        {
            bool ok = false;
            timestamp = valueList.front().toDouble(&ok);
            if (!ok || timestamp < 0)
                timestamp = TimeUtils::MonotonicTime().Seconds();
            valueList.pop_front();
        }
        int i = 0;
        for (auto& s : valueList)
        {
            QString color = GetColorName(i++);
            s = QString("<font color='%1'>%2</font>").arg(color, s);
        }
        pWatch->setText(valueList.join(", ").trimmed());
        pWatch->mpPlot->AddDataPoint(value, timestamp);
    }
    Update();
}

void WatchesWindow::AddEmptyLine()
{
    int row = mpTable->rowCount();
    mpTable->setRowCount(mpTable->rowCount() + 1);
    RangeWidget *pRange = new RangeWidget;
    WatchPlot* pPlot = new WatchPlot(mHistoryLengthSeconds, mLastUpdate, pRange);
    Watch *pWatch = new Watch(pPlot);
    mpTable->setCellWidget(row, valueCol, pWatch);
    mpTable->setCellWidget(row, rangeCol, pRange);
    mpTable->setCellWidget(row, plotCol, pPlot);
    for (int col : {valueCol, rangeCol, plotCol})
        mpTable->cellWidget(row, col)->setFocusPolicy(Qt::ClickFocus); // avoid Qt warning about edit failure
}

void WatchesWindow::RemoveLastLine()
{
    if (mpTable->rowCount() > 0)
        mpTable->setRowCount(mpTable->rowCount() - 1);
}

void WatchesWindow::SetHistoryLengthSeconds(double inSeconds)
{
    mHistoryLengthSeconds = inSeconds;
    for (int row = 0; row < mpTable->rowCount(); ++row)
    {
        Watch *pWatch = dynamic_cast<Watch *>(mpTable->cellWidget(row, valueCol));
        if (pWatch && pWatch->mpPlot)
        {
            pWatch->mpPlot->SetMaxTime(inSeconds);
            pWatch->mpPlot->update();
        }
    }
    QStringList labels;
    labels << "Expression"
           << "Decim"
           << "Value"
           << "Range" << QString("Past %1s").arg(mHistoryLengthSeconds);
    mpTable->setHorizontalHeaderLabels(labels);
}

void WatchesWindow::AddWatch(const QString& inExpression, int inDecimation, double rangeMin, double rangeMax)
{
    int row = mpTable->rowCount() - 1;
    auto pItem = new QTableWidgetItem(inExpression);
    mpTable->setItem(row, expressionCol, pItem);
    QString text = QString::number(inDecimation);
    if (inDecimation < 1)
        text = "auto";
    pItem = CreateDecimationItem(text);
    mpTable->setItem(row, decimationCol, pItem);
    auto pPlot = dynamic_cast<WatchPlot*>(mpTable->cellWidget(row, plotCol));
    Assert(pPlot);
    pPlot->SetFixedRange(rangeMin, rangeMax);
    show();
}

void WatchesWindow::SaveWatches()
{
    QByteArray data;
    for (int row = 0; row < mpTable->rowCount(); ++row)
    {
        auto* pItemEx = mpTable->item(row, expressionCol),
            * pItemDec = mpTable->item(row, decimationCol);
        if (pItemEx && pItemDec) {
            data.append(pItemEx->text().toLocal8Bit()).append(1, 1);
            data.append(pItemDec->text().toLocal8Bit()).append(1, 0);
        }
    }
    OperatorUtils::PutPersistentData("Watches", data);
}

void WatchesWindow::RestoreWatches()
{
    ClearWatches();
    QByteArray data;
    OperatorUtils::GetPersistentData("Watches", data);
    QString expression, decimation;
    enum { inExpression, inDecimation } state = inExpression;
    for (int i = 0; i < data.length(); ++i)
    {
        char c = data[i];
        if (c == 0) {
            int row = mpTable->rowCount() - 1;
            auto pItem = new QTableWidgetItem(expression);
            mpTable->setItem(row, expressionCol, pItem);
            expression.clear();

            pItem = CreateDecimationItem(decimation);
            mpTable->setItem(row, decimationCol, pItem);
            decimation.clear();

            state = inExpression;
        }
        else if (c == 1) {
            state = inDecimation;
        }
        else if (state == inExpression) {
            expression += c;
        }
        else if (state == inDecimation) {
            decimation += c;
        }
    }
}

void WatchesWindow::ClearWatches()
{
    mpTable->setRowCount(0);
    AddEmptyLine();
}
