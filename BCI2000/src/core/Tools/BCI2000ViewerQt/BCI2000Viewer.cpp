////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000Viewer.cpp 8298 2024-08-02 21:39:37Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The main window of the BCI2000Viewer tool.
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
#include "BCI2000Viewer.h"
#include "ui_BCI2000Viewer.h"

#include "QtMain.h"
#include "NotesWidget.h"
#include "AboutBox.h"
#include "AudioPlayer.h"
#include "AudioSink.h"
#include "ColorListChooser.h"
#include "ExecutableHelp.h"
#include "FileUtils.h"
#include "ProcessUtils.h"
#include "Settings.h"
#include "TimeValue.h"
#include "Streambuf.h"

#include <QtGui>
#include <iomanip>
#include <sstream>

Q_DECLARE_METATYPE(int64_t)

const double cAudioExtensionSamplingRate_Hz = 44100; // hardcoded in audio extension
const int cAudioExtensionDefaultBufferSize = 1024; // hardcoded in audio extension
const int cMaxWebcams = 32;

const RGBColor BCI2000Viewer::cAxisColor = RGBColor::White;
const RGBColor BCI2000Viewer::cChannelColorsDefault[] = {RGBColor::White, RGBColor::White,  RGBColor::White,
                                                         RGBColor::White, RGBColor::Yellow, ColorList::End};

const QColor BCI2000Viewer::cHeadingColor = QColor(0xffffb0);
const int BCI2000Viewer::cAutoDecimationMaxLinesPerPixel = 3;

BCI2000Viewer::ActionEntry BCI2000Viewer::sActions[] = {
    {&BCI2000Viewer::FileOpen, &BCI2000Viewer::FileOpen_Enabled, NULL, "actionOpen", NULL},
    {&BCI2000Viewer::FileClose, &BCI2000Viewer::FileClose_Enabled, NULL, "actionClose", NULL},
    {&BCI2000Viewer::FileOpenInfo, &BCI2000Viewer::FileOpenInfo_Enabled, NULL, "actionOpenInfo", NULL},
    {&BCI2000Viewer::FileExport, &BCI2000Viewer::FileExport_Enabled, NULL, "actionExport", NULL},
    {&BCI2000Viewer::FileQuit, &BCI2000Viewer::FileQuit_Enabled, NULL, "actionQuit", NULL},
    {&BCI2000Viewer::EditCopy, &BCI2000Viewer::EditCopy_Enabled, NULL, "actionCopy_to_Clipboard", NULL},
    {&BCI2000Viewer::HelpOpenHelp, NULL, NULL, "actionBCI2000_Help", NULL},
    {&BCI2000Viewer::HelpAbout, NULL, NULL, "actionAbout", NULL},

    {&BCI2000Viewer::ToggleNotes, &BCI2000Viewer::ToggleNotes_Enabled, &BCI2000Viewer::ToggleNotes_Checked,
    "actionToggle_Notes", nullptr},
    {&BCI2000Viewer::ToggleStatesChannels, &BCI2000Viewer::ToggleStatesChannels_Enabled, &BCI2000Viewer::ToggleStatesChannels_Checked,
    "actionToggle_StatesChannels", nullptr},
    {&BCI2000Viewer::ToggleFastScrolling, nullptr, &BCI2000Viewer::ToggleFastScrolling_Checked,
    "actionToggle_Fast_Scrolling", nullptr},
    {&BCI2000Viewer::ToggleAutoDecimation, nullptr, &BCI2000Viewer::ToggleAutoDecimation_Checked,
    "actionToggle_Auto_Decimation", nullptr},
    {&BCI2000Viewer::Invert, &BCI2000Viewer::Invert_Enabled, &BCI2000Viewer::Invert_Checked, "actionInvert", NULL},
    {&BCI2000Viewer::EnlargeSignal, &BCI2000Viewer::ChangeResolution_Enabled, NULL, "actionEnlarge_Signal", NULL},
    {&BCI2000Viewer::ReduceSignal, &BCI2000Viewer::ChangeResolution_Enabled, NULL, "actionReduce_Signal", NULL},
    {&BCI2000Viewer::MoreChannels, &BCI2000Viewer::MoreChannels_Enabled, NULL, "actionMore_Channels", NULL},
    {&BCI2000Viewer::FewerChannels, &BCI2000Viewer::FewerChannels_Enabled, NULL, "actionFewer_Channels", NULL},
    {&BCI2000Viewer::ChooseChannelColors, &BCI2000Viewer::ChooseChannelColors_Enabled, NULL,
     "actionChoose_Channel_Colors", NULL},
    {&BCI2000Viewer::ToggleBaselines, &BCI2000Viewer::ToggleBaselines_Enabled, &BCI2000Viewer::ToggleBaselines_Checked,
     "actionShow_Baselines", NULL},
    {&BCI2000Viewer::ToggleUnit, &BCI2000Viewer::ToggleUnit_Enabled, &BCI2000Viewer::ToggleUnit_Checked,
     "actionShow_Unit", NULL},

    {&BCI2000Viewer::HPOff, &BCI2000Viewer::Filter_Enabled, NULL, "actionHP_Off", "HP"},
    {&BCI2000Viewer::HPRemoveMean, &BCI2000Viewer::Filter_Enabled, NULL, "actionHP_Remove_Mean", "HP"},
    {&BCI2000Viewer::HP1Hz, &BCI2000Viewer::Filter_Enabled, NULL, "actionHP_1Hz", "HP"},
    {&BCI2000Viewer::HP5Hz, &BCI2000Viewer::Filter_Enabled, NULL, "actionHP_5Hz", "HP"},

    {&BCI2000Viewer::LPOff, &BCI2000Viewer::Filter_Enabled, NULL, "actionLP_Off", "LP"},
    {&BCI2000Viewer::LP30Hz, &BCI2000Viewer::Filter_Enabled, NULL, "actionLP_30Hz", "LP"},
    {&BCI2000Viewer::LP40Hz, &BCI2000Viewer::Filter_Enabled, NULL, "actionLP_40Hz", "LP"},
    {&BCI2000Viewer::LP70Hz, &BCI2000Viewer::Filter_Enabled, NULL, "actionLP_70Hz", "LP"},

    {&BCI2000Viewer::NotchOff, &BCI2000Viewer::Filter_Enabled, NULL, "actionNotch_Off", "Notch"},
    {&BCI2000Viewer::Notch50Hz, &BCI2000Viewer::Filter_Enabled, NULL, "actionNotch_50Hz", "Notch"},
    {&BCI2000Viewer::Notch60Hz, &BCI2000Viewer::Filter_Enabled, NULL, "actionNotch_60Hz", "Notch"},

    {&BCI2000Viewer::SampleZoomIn, &BCI2000Viewer::SampleZoomIn_Enabled, NULL, "actionZoom_In", NULL},
    {&BCI2000Viewer::SampleZoomOut, &BCI2000Viewer::SampleZoomOut_Enabled, NULL, "actionZoom_Out", NULL},
    {&BCI2000Viewer::ToFirstSample, &BCI2000Viewer::GoBack_Enabled, NULL, "toBeginBtn", NULL},
    {&BCI2000Viewer::ToLastSample, &BCI2000Viewer::GoForward_Enabled, NULL, "toEndBtn", NULL},
    {&BCI2000Viewer::ToPrevBlock, &BCI2000Viewer::GoBack_Enabled, NULL, "blockRewBtn", NULL},
    {&BCI2000Viewer::ToNextBlock, &BCI2000Viewer::GoForward_Enabled, NULL, "blockFwdBtn", NULL},
    {&BCI2000Viewer::ToPrevPage, &BCI2000Viewer::GoBack_Enabled, NULL, "pageRewBtn", NULL},
    {&BCI2000Viewer::ToNextPage, &BCI2000Viewer::GoForward_Enabled, NULL, "pageFwdBtn", NULL},

    {&BCI2000Viewer::SampleZoomIn, &BCI2000Viewer::SampleZoomIn_Enabled, NULL, "zoomInBtn", NULL},
    {&BCI2000Viewer::SampleZoomOut, &BCI2000Viewer::SampleZoomOut_Enabled, NULL, "zoomOutBtn", NULL},

    {&BCI2000Viewer::ShowSelectedChannels, &BCI2000Viewer::ChannelsSelected, NULL, "actionShow_Channel_s", NULL},
    {&BCI2000Viewer::HideSelectedChannels, &BCI2000Viewer::ChannelsSelected, NULL, "actionHide_Channel_s", NULL},
    {&BCI2000Viewer::HelpOnState, &BCI2000Viewer::StateSelected, NULL, "actionState_Help", NULL},
};

//---------------------------------------------------------------------------

BCI2000Viewer::BCI2000Viewer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::BCI2000Viewer), mSamplePos(0), 
      mNumSignalChannels(0), 
      mPositionEdited(false),
      mSamplingRate(0.0), mRemoveMean(false), 
      mpNotesWidget(nullptr), mpNotesState(nullptr),
      mpAVControl(nullptr), mpAudioFrameState(nullptr),
      mPrevReadSignalDuration(0),
      mpAudioSink(new AudioSink(bci::Rate::FromHertz(44100), 2, 441)),
      mpSecondaryViewer(nullptr),
      mIsPrimary(false), mIsSecondary(false)
{
    qRegisterMetaType<int64_t>("int64_t");

    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    setAcceptDrops(true);
    grabKeyboard();
    
    connect(QtApplication::instance(), &QtApplication::FileOpen, this, &BCI2000Viewer::onFileOpen);
    mpNotesWidget = new NotesWidget(this);
    mpNotesWidget->setVisible(false);
    ui->splitter->insertWidget(0, mpNotesWidget);
    connect(mpNotesWidget, &NotesWidget::userClickedNote, this, &BCI2000Viewer::SyncWithNote);

    ui->signalDisplay->SetBackgroundText("Drag and drop a BCI2000 data file\n"
                                         "into this window\nto display its contents.");
    ui->splitter->setCollapsible(0, false);
    ui->splitter->setCollapsible(1, false);
    ui->splitter->setCollapsible(2, false);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 1);
    ui->splitter->setStretchFactor(2, 0);
    ui->channelList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->menuWindow->setEnabled(false);

    SetupActions();
    ui->signalDisplay->Display()
        .SetValueUnitVisible(true)
        .SetAxisColor(cAxisColor)
        .SetChannelColors(cChannelColorsDefault)
        .SetFastScrolling(true)
        .SetAutoDecimation(cAutoDecimationMaxLinesPerPixel);
    mPlaybackTimer.setTimerType(Qt::PreciseTimer);
    mPlaybackTimer.setInterval(40); // 25 fps
    connect(&mPlaybackTimer, SIGNAL(timeout()), this, SLOT(PlaybackTimer()));

    QString filename;
    auto args = QApplication::arguments();
    for (int i = 1; i < args.size(); ++i)
    {
        if (args.at(i) == "--secondary")
            mIsSecondary = true;
        else if (args.at(i).startsWith("--"))
            /* ignore unknown option */;
        else
            filename = args.at(i);
    }
    if (mIsSecondary)
    {
        // disable close button on secondary viewer
        setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
        // make sure std streams are not redirected to bci streams
        BCIStream::ResetRedirection();
    }
    // defer start of IO thread until events are being processed
    QTimer::singleShot(0, this, &BCI2000Viewer::OnIOThreadTimer);
    QDir::setCurrent(QApplication::applicationDirPath() + "/../../data");
    ReadSettings();
    show();
    DoFileOpen(filename);
}

BCI2000Viewer::~BCI2000Viewer()
{
    delete ui;
    delete mpSecondaryViewer;
    if (mSecondaryViewerThread.joinable())
        mSecondaryViewerThread.join();
}

void BCI2000Viewer::SetupActions()
{
    // setup context menu for signal display
    ui->signalDisplay->setContextMenuPolicy(Qt::CustomContextMenu);
    // create context menu for channel list
    ui->channelList->addAction(ui->actionShow_Channel_s);
    ui->channelList->addAction(ui->actionHide_Channel_s);
    QAction *pSeparator = new QAction(NULL);
    pSeparator->setSeparator(true);
    ui->channelList->addAction(pSeparator);
    ui->channelList->addAction(ui->actionState_Help);
    ui->channelList->setContextMenuPolicy(Qt::ActionsContextMenu);

    // connect actions to slots and create action groups
    std::map<std::string, QActionGroup *> groups;
    for (size_t i = 0; i < sizeof(sActions) / sizeof(*sActions); ++i)
    {
        QAction *pAction = findChild<QAction *>(sActions[i].object);
        if (pAction)
        {
            connect(pAction, SIGNAL(triggered()), this, SLOT(ExecuteAction()));
            if (sActions[i].checked != NULL)
                pAction->setCheckable(true);
            else if (sActions[i].group != NULL)
            {
                pAction->setCheckable(true);
                std::string g = sActions[i].group;
                if (groups[g] == NULL)
                {
                    groups[g] = new QActionGroup(this);
                    pAction->setChecked(true);
                }
                groups[g]->addAction(pAction);
            }
        }
        QPushButton *pButton = findChild<QPushButton *>(sActions[i].object);
        if (pButton)
        {
            connect(pButton, SIGNAL(clicked()), this, SLOT(ExecuteAction()));
        }
    }
}

void BCI2000Viewer::ExecuteAction()
{
    QObject *pSender = sender();
    for (size_t i = 0; i < sizeof(sActions) / sizeof(*sActions); ++i)
        if (pSender->objectName() == sActions[i].object)
            (this->*sActions[i].action)();
    UpdateActions();
}

void BCI2000Viewer::UpdateActions()
{
    for (size_t i = 0; i < sizeof(sActions) / sizeof(*sActions); ++i)
    {
        QAction *pAction = findChild<QAction *>(sActions[i].object);
        if (pAction)
        {
            if (sActions[i].enabled)
                pAction->setEnabled((this->*sActions[i].enabled)());
            if (sActions[i].checked)
                pAction->setChecked((this->*sActions[i].checked)());
        }
        QPushButton *pButton = findChild<QPushButton *>(sActions[i].object);
        if (pButton)
        {
            if (sActions[i].enabled)
                pButton->setEnabled((this->*sActions[i].enabled)());
        }
    }
}

void BCI2000Viewer::closeEvent(QCloseEvent *pEvent)
{
    SaveSettings();
    CloseNotes();
    DestroyAVObjects();
    DestroySecondaryViewer();
    pEvent->accept();
}

void BCI2000Viewer::keyPressEvent(QKeyEvent *iopEvent)
{
    static int acc = 0;
    int wipe_acc = 1;
    bool shift = (iopEvent->modifiers() & Qt::ShiftModifier) || (iopEvent->modifiers() & Qt::ControlModifier) ||
                 (iopEvent->modifiers() & Qt::AltModifier);
    switch (iopEvent->key())
    {
    case Qt::Key_Up:
        if (ChannelUp_Enabled())
        {
            if (shift)
                ChannelPagePrev();
            else
                ChannelUp();
        }
        break;
    case Qt::Key_Down:
        if (ChannelDown_Enabled())
        {
            if (shift)
                ChannelPageNext();
            else
                ChannelDown();
        }
        break;

    case Qt::Key_PageUp:
    case Qt::Key_B:
        if (ChannelUp_Enabled())
            ChannelPagePrev();
        break;
    case Qt::Key_PageDown:
    case Qt::Key_Space:
        if (ChannelDown_Enabled())
            ChannelPageNext();
        break;

    case Qt::Key_Right:
        if (GoForward_Enabled())
        {
            if (shift)
                ToNextPage();
            else
                ToNextBlock();
        }
        break;
    case Qt::Key_Left:
        if (GoBack_Enabled())
        {
            if (shift)
                ToPrevPage();
            else
                ToPrevBlock();
        }
        break;
    case Qt::Key_Comma:
        if (FewerChannels_Enabled())
            FewerChannels();
        break;
    case Qt::Key_Period:
        if (MoreChannels_Enabled())
            MoreChannels();
        break;
    case Qt::Key_Minus:
        if (ChangeResolution_Enabled())
            ReduceSignal();
        break;
    case Qt::Key_Plus:
        if (ChangeResolution_Enabled())
            EnlargeSignal();
        break;
    case Qt::Key_Home:
        if (ChannelUp_Enabled())
            ChannelPageFirst();
        break;
    case Qt::Key_End:
        if (ChannelDown_Enabled())
            ChannelPageLast();
        break;
    case Qt::Key_Enter:
    case Qt::Key_G:
        ChannelPageFirst();
        for (int i = 1; i < acc; ++i)
            ChannelDown();
        break;
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
        acc = acc * 10 + (iopEvent->key() - Qt::Key_0);
        wipe_acc = 0;
        break;
    }
    if (wipe_acc)
        acc = 0;
}

void BCI2000Viewer::dragEnterEvent(QDragEnterEvent *iopEvent)
{
    if (mIsSecondary)
        return;
    QList<QUrl> urlList = iopEvent->mimeData()->urls();
    if (urlList.size() == 1)
    {
        QString file = urlList[0].toLocalFile();
        if (QFileInfo(file).isFile() && BCI2000FileReader(file.toLocal8Bit().constData()).IsOpen())
            iopEvent->acceptProposedAction();
    }
}

void BCI2000Viewer::dropEvent(QDropEvent *iopEvent)
{
    if (mIsSecondary)
        return;
    QList<QUrl> urlList = iopEvent->mimeData()->urls();
    if (urlList.size() == 1)
        DoFileOpen(urlList[0].toLocalFile());
    iopEvent->acceptProposedAction();
}

void BCI2000Viewer::onFileOpen(const QString& filename)
{
    if (mIsSecondary)
        return;
    if (!filename.isEmpty())
    {
        QDir::setCurrent(QFileInfo(filename).canonicalPath());
        DoFileOpen(filename);
    }
}

void BCI2000Viewer::on_editPosition_textEdited(QString)
{
    mPositionEdited = true;
}

void BCI2000Viewer::on_editPosition_editingFinished()
{
    if (mPositionEdited)
    {
        TimeValue t;
        std::istringstream iss(ui->editPosition->text().toLocal8Bit().constData());
        if (iss >> t)
            SetSamplePos(static_cast<int>(t * mFile.SamplingRate() - ui->signalDisplay->Display().NumSamples() / 2));
    }
    this->grabKeyboard();
}

void BCI2000Viewer::on_editPosition_returnPressed()
{
    on_editPosition_editingFinished();
}

void BCI2000Viewer::on_editPosition_selectionChanged()
{
    mPositionEdited = false;
    this->releaseKeyboard();
}

void BCI2000Viewer::on_channelList_itemChanged(QListWidgetItem *)
{
    UpdateChannelLabels();
    SetSamplePos(mSamplePos);
    UpdateVerticalScroller();
}

void BCI2000Viewer::on_channelList_itemSelectionChanged()
{
    UpdateActions();
}

void BCI2000Viewer::on_channelList_currentRowChanged(int inCurrentRow)
{
    UpdateActions();
}

void BCI2000Viewer::on_verticalScrollBar_valueChanged(int value)
{
    ui->signalDisplay->Display().SetTopGroup(ui->verticalScrollBar->sliderPosition());
}

void BCI2000Viewer::on_signalDisplay_customContextMenuRequested(QPoint inPos)
{
    QWidget *pWidget = dynamic_cast<QWidget *>(sender());
    if (pWidget != NULL)
        ui->menuView->popup(pWidget->mapToGlobal(inPos));
}

// User actions
//---------------------------------------------------------------------------
// Standard actions
void BCI2000Viewer::FileOpen()
{
    QString filename =
        QFileDialog::getOpenFileName(this, tr("Open Data File"), QDir::currentPath(), tr("BCI2000 Data Files (*.dat)"));
    if (!filename.isEmpty())
    {
        QDir::setCurrent(QFileInfo(filename).canonicalPath());
        DoFileOpen(filename);
    }
}
bool BCI2000Viewer::FileOpen_Enabled() const
{
    return !mIsSecondary;
}

void BCI2000Viewer::FileClose()
{
    DoFileOpen("");
}
bool BCI2000Viewer::FileClose_Enabled() const
{
    return mFile.IsOpen() && !mIsSecondary;
}

static std::string ConstructFileInfoPath()
{
    std::string dir = FileUtils::InstallationDirectory();
    dir = FileUtils::ParentDirectory(dir);
    dir += "/BCI2000FileInfo/";
    std::string executable = dir + "BCI2000FileInfo";
#if __APPLE__ && USE_MACOS_APPLICATION_BUNDLES
    executable += ".app/Contents/MacOS/BCI2000FileInfo";
#endif
    return executable;
}

void BCI2000Viewer::FileOpenInfo()
{
    std::string executable = ConstructFileInfoPath();
    int ignored = 0;
    ProcessUtils::ExecuteAsynchronously(executable, mFileName, ignored);
}
bool BCI2000Viewer::FileOpenInfo_Enabled() const
{
    std::string executable = ConstructFileInfoPath();
    return FileUtils::IsFile(executable) && mFile.IsOpen();
}

void BCI2000Viewer::FileExport()
{
    QString file = QString::fromLocal8Bit(
        (FileUtils::ExtractDirectory(mFileName) + FileUtils::ExtractBase(mFileName) + ".ascii").c_str());
    QString filename =
        QFileDialog::getSaveFileName(this, tr("Save currently displayed data as ..."), file,
                                     "Tab-separated ASCII file (*.ascii);;Octave-style ASCII file (*.oct)");
    if (!filename.isEmpty())
    {
        std::string file = filename.toLocal8Bit().constData();
        std::string ext = FileUtils::ExtractExtension(file);
        std::ofstream output(file);
        if (!output.is_open())
        {
            bcierr << "Could not open\n" << file << "\nfor writing.";
            return;
        }
        if (ext == ".ascii")
        {
            const LabelList &labels = ui->signalDisplay->Display().ChannelLabels();
            for (int ch = 0; ch < labels.Size(); ++ch)
            {
                output << labels[ch].Text();
                if (ch < labels.Size() - 1)
                    output << "\t";
                else
                    output << "\n";
            }
            GenericSignal signal = ConstructDisplaySignal(0, mFile.NumSamples());
            output << std::scientific << std::setprecision(std::numeric_limits<double>::max_digits10);
            for (int el = 0; output && el < signal.Elements(); ++el)
            {
                for (int ch = 0; ch < signal.Channels(); ++ch)
                {
                    output << signal(ch, el);
                    if (ch < signal.Channels() - 1)
                        output << "\t";
                    else
                        output << "\n";
                }
            }
        }
        else if (ext == ".oct")
        {
            output << std::setprecision(std::numeric_limits<double>::max_digits10);
            GenericSignal signal = ConstructDisplaySignal(0, mFile.NumSamples());
            const LabelList &labels = ui->signalDisplay->Display().ChannelLabels();
            int states = ui->signalDisplay->Display().NumMarkerChannels(), channels = signal.Channels() - states;
            if (channels > 0)
            {
                output << "# name: signal\n# type: matrix\n# rows: " << signal.Elements() << "\n# columns: " << channels
                       << "\n";
                for (int el = 0; el < signal.Elements(); ++el)
                {
                    for (int ch = 0; ch < channels; ++ch)
                        output << ' ' << signal(ch, el);
                    output << '\n';
                }
            }
            for (int ch = channels; ch < signal.Channels(); ++ch)
            {
                output << "# name: " << labels[ch].Text() << "\n# type: matrix\n# rows: " << signal.Elements()
                       << "\n# columns: 1\n";
                for (int el = 0; el < signal.Elements(); ++el)
                    output << ' ' << signal(ch, el) << '\n';
            }
        }
        else
        {
            bcierr << "Unknown file format: " << ext;
            return;
        }
        if (!(output << std::flush))
            bcierr << "Error when writing\n" << file;
    }
}
bool BCI2000Viewer::FileExport_Enabled() const
{
    return mFile.IsOpen();
}

void BCI2000Viewer::FileQuit()
{
    close();
}

bool BCI2000Viewer::FileQuit_Enabled() const
{
    return !mIsSecondary;
}

void BCI2000Viewer::EditCopy()
{
    QImage image(ui->signalDisplay->width(), ui->signalDisplay->height(), QImage::Format_RGB32);
    ;
    ui->signalDisplay->render(&image);
    QApplication::clipboard()->setImage(image);
}
bool BCI2000Viewer::EditCopy_Enabled() const
{
    return mFile.IsOpen();
}

void BCI2000Viewer::HelpOpenHelp()
{
    ExecutableHelp().Display();
}

void BCI2000Viewer::HelpAbout()
{
    AboutBox().SetApplicationName(QApplication::applicationName().toLocal8Bit().constData()).Display();
}

// Temporal movement
void BCI2000Viewer::ToFirstSample()
{
    SetSamplePos(0);
}
void BCI2000Viewer::ToLastSample()
{
    SetSamplePos(LastSample());
}
void BCI2000Viewer::ToNextBlock()
{
    ScrollSamplePosBy(mFile.SignalProperties().Elements());
}
void BCI2000Viewer::ToPrevBlock()
{
    ScrollSamplePosBy(-mFile.SignalProperties().Elements());
}
void BCI2000Viewer::ToPrevPage()
{
    SetSamplePos(std::max<int64_t>(0LL, mSamplePos - ui->signalDisplay->Display().NumSamples()));
}
void BCI2000Viewer::ToNextPage()
{
    SetSamplePos(std::min(mFile.NumSamples() - ui->signalDisplay->Display().NumSamples(),
                          mSamplePos + ui->signalDisplay->Display().NumSamples()));
}

bool BCI2000Viewer::GoBack_Enabled() const
{
    return mSamplePos > 0;
}
bool BCI2000Viewer::GoForward_Enabled() const
{
    return mSamplePos < mFile.NumSamples() - ui->signalDisplay->Display().NumSamples() - 1;
}

// Temporal resolution
void BCI2000Viewer::SampleZoomIn()
{
    int prevNumSamples = ui->signalDisplay->Display().NumSamples(), newNumSamples = prevNumSamples / 2;
    ui->signalDisplay->Display().SetNumSamples(newNumSamples);
    SetSamplePos(mSamplePos + (prevNumSamples - newNumSamples) / 2);
}
bool BCI2000Viewer::SampleZoomIn_Enabled() const
{
    return mFile.IsOpen() && ui->signalDisplay->Display().NumSamples() / 2 >= mFile.SignalProperties().Elements();
}

void BCI2000Viewer::SampleZoomOut()
{
    int prevNumSamples = ui->signalDisplay->Display().NumSamples(), newNumSamples = prevNumSamples * 2;
    if (newNumSamples > mFile.NumSamples())
        newNumSamples = mFile.NumSamples();
    ui->signalDisplay->Display().SetNumSamples(newNumSamples);
    SetSamplePos(mSamplePos + (prevNumSamples - newNumSamples) / 2);
}
bool BCI2000Viewer::SampleZoomOut_Enabled() const
{
    return ui->signalDisplay->Display().NumSamples() < mFile.NumSamples();
}

// Number of displayed channels
void BCI2000Viewer::FewerChannels()
{
    int newNumChannels = ui->signalDisplay->Display().DisplayGroups() / 2;
    if (newNumChannels < 1)
        newNumChannels = 1;
    ui->signalDisplay->Display().SetDisplayGroups(newNumChannels).SetTopGroup(ui->verticalScrollBar->sliderPosition());
    UpdateVerticalScroller();
}
bool BCI2000Viewer::FewerChannels_Enabled() const
{
    return ui->signalDisplay->Display().DisplayGroups() > 1;
}

void BCI2000Viewer::MoreChannels()
{
    int newNumChannels = ui->signalDisplay->Display().DisplayGroups() * 2;
    newNumChannels = std::min(newNumChannels, mNumSignalChannels);
    ui->signalDisplay->Display().SetDisplayGroups(newNumChannels).SetTopGroup(ui->verticalScrollBar->sliderPosition());
    UpdateVerticalScroller();
}
bool BCI2000Viewer::MoreChannels_Enabled() const
{
    return ui->signalDisplay->Display().DisplayGroups() < mNumSignalChannels;
}

// Channel scrolling
void BCI2000Viewer::ChannelUp()
{
    ui->verticalScrollBar->setSliderPosition(ui->verticalScrollBar->sliderPosition() - 1);
}
void BCI2000Viewer::ChannelDown()
{
    ui->verticalScrollBar->setSliderPosition(ui->verticalScrollBar->sliderPosition() + 1);
}
void BCI2000Viewer::ChannelPageNext()
{
    QScrollBar *s = ui->verticalScrollBar;
    s->setSliderPosition(std::min(s->maximum(), s->sliderPosition() + s->pageStep()));
}
void BCI2000Viewer::ChannelPagePrev()
{
    QScrollBar *s = ui->verticalScrollBar;
    s->setSliderPosition(std::max(s->minimum(), s->sliderPosition() - s->pageStep()));
}
void BCI2000Viewer::ChannelPageFirst()
{
    ui->verticalScrollBar->setSliderPosition(ui->verticalScrollBar->minimum());
}
void BCI2000Viewer::ChannelPageLast()
{
    ui->verticalScrollBar->setSliderPosition(ui->verticalScrollBar->maximum());
}

bool BCI2000Viewer::ChannelUp_Enabled()
{
    QScrollBar *s = ui->verticalScrollBar;
    return s->isEnabled() && s->sliderPosition() > s->minimum();
}
bool BCI2000Viewer::ChannelDown_Enabled()
{
    QScrollBar *s = ui->verticalScrollBar;
    return s->isEnabled() && s->sliderPosition() < s->maximum();
}

// Signal resolution
void BCI2000Viewer::EnlargeSignal()
{
    ui->signalDisplay->Display().SetMinValue(ui->signalDisplay->Display().MinValue() / 2);
    ui->signalDisplay->Display().SetMaxValue(ui->signalDisplay->Display().MaxValue() / 2);
}

void BCI2000Viewer::ReduceSignal()
{
    ui->signalDisplay->Display().SetMinValue(ui->signalDisplay->Display().MinValue() * 2);
    ui->signalDisplay->Display().SetMaxValue(ui->signalDisplay->Display().MaxValue() * 2);
}

bool BCI2000Viewer::ChangeResolution_Enabled() const
{
    return ui->signalDisplay->Display().NumSamples() > 0;
}

// Display attributes
void BCI2000Viewer::ChooseChannelColors()
{
    // The dialog's "custom colors" are used to hold channel colors.
    ColorList channelColors = ui->signalDisplay->Display().ChannelColors();
    ColorListChooser().SetPrompt(tr("Choose channel colors").toLocal8Bit().constData()).Display(channelColors);
    ui->signalDisplay->Display().SetChannelColors(channelColors);
}
bool BCI2000Viewer::ChooseChannelColors_Enabled() const
{
    return mFile.IsOpen();
}

void BCI2000Viewer::ToggleFastScrolling()
{
    ui->signalDisplay->Display().SetFastScrolling(!ui->signalDisplay->Display().FastScrolling());
}

bool BCI2000Viewer::ToggleFastScrolling_Checked() const
{
    return ui->signalDisplay->Display().FastScrolling();
}

void BCI2000Viewer::ToggleAutoDecimation()
{
  int maxLpp = ui->signalDisplay->Display().AutoDecimation();
  if (maxLpp)
    maxLpp = 0;
  else
    maxLpp = cAutoDecimationMaxLinesPerPixel;
  ui->signalDisplay->Display().SetAutoDecimation(maxLpp);
}

bool BCI2000Viewer::ToggleAutoDecimation_Checked() const
{
  return ui->signalDisplay->Display().AutoDecimation();
}

void BCI2000Viewer::Invert()
{
    ui->signalDisplay->Display().SetInverted(!ui->signalDisplay->Display().Inverted());
}
bool BCI2000Viewer::Invert_Checked() const
{
    return ui->signalDisplay->Display().Inverted();
}
bool BCI2000Viewer::Invert_Enabled() const
{
    return mNumSignalChannels > 0;
}

void BCI2000Viewer::ToggleBaselines()
{
    ui->signalDisplay->Display().SetBaselinesVisible(!ui->signalDisplay->Display().BaselinesVisible());
}
bool BCI2000Viewer::ToggleBaselines_Checked() const
{
    return ui->signalDisplay->Display().BaselinesVisible();
}
bool BCI2000Viewer::ToggleBaselines_Enabled() const
{
    return mNumSignalChannels > 0;
}

void BCI2000Viewer::ToggleUnit()
{
    ui->signalDisplay->Display().SetValueUnitVisible(!ui->signalDisplay->Display().ValueUnitVisible());
}
bool BCI2000Viewer::ToggleUnit_Checked() const
{
    return ui->signalDisplay->Display().ValueUnitVisible();
}
bool BCI2000Viewer::ToggleUnit_Enabled() const
{
    return mNumSignalChannels > 0;
}

void BCI2000Viewer::HPOff()
{
    mFilter.HPCorner(0.0);
    mRemoveMean = false;
    FilterChanged();
}
void BCI2000Viewer::HPRemoveMean()
{
    mFilter.HPCorner(0.0);
    mRemoveMean = true;
    FilterChanged();
}
void BCI2000Viewer::HP1Hz()
{
    mFilter.HPCorner(1.0 / mSamplingRate);
    mRemoveMean = false;
    FilterChanged();
}
void BCI2000Viewer::HP5Hz()
{
    mFilter.HPCorner(5.0 / mSamplingRate);
    mRemoveMean = false;
    FilterChanged();
}

void BCI2000Viewer::LPOff()
{
    mFilter.LPCorner(0.0);
    FilterChanged();
}
void BCI2000Viewer::LP30Hz()
{
    mFilter.LPCorner(30.0 / mSamplingRate);
    FilterChanged();
}
void BCI2000Viewer::LP40Hz()
{
    mFilter.LPCorner(40.0 / mSamplingRate);
    FilterChanged();
}
void BCI2000Viewer::LP70Hz()
{
    mFilter.LPCorner(70.0 / mSamplingRate);
    FilterChanged();
}

void BCI2000Viewer::NotchOff()
{
    mFilter.NotchCenter(0.0);
    FilterChanged();
}
void BCI2000Viewer::Notch50Hz()
{
    mFilter.NotchCenter(50.0 / mSamplingRate);
    FilterChanged();
}
void BCI2000Viewer::Notch60Hz()
{
    mFilter.NotchCenter(60.0 / mSamplingRate);
    FilterChanged();
}

bool BCI2000Viewer::Filter_Enabled() const
{
    return mSamplingRate > 0.0;
}

void BCI2000Viewer::ShowSelectedChannels()
{
    ui->channelList->blockSignals(true);
    QList<QListWidgetItem *> s = ui->channelList->selectedItems();
    for (QList<QListWidgetItem *>::iterator i = s.begin(); i != s.end(); ++i)
        (*i)->setCheckState(Qt::Checked);
    this->on_channelList_itemChanged(NULL);
    ui->channelList->blockSignals(false);
}
void BCI2000Viewer::HideSelectedChannels()
{
    ui->channelList->blockSignals(true);
    QList<QListWidgetItem *> s = ui->channelList->selectedItems();
    for (QList<QListWidgetItem *>::iterator i = s.begin(); i != s.end(); ++i)
        (*i)->setCheckState(Qt::Unchecked);
    this->on_channelList_itemChanged(NULL);
    ui->channelList->blockSignals(false);
}
bool BCI2000Viewer::ChannelsSelected() const
{
    return ui->channelList->selectedItems().size() > 0;
}

void BCI2000Viewer::HelpOnState()
{
    QListWidgetItem *pItem = ui->channelList->currentItem();
    if (pItem)
    {
        std::string name = pItem->text().toLocal8Bit().constData();
        if (ExecutableHelp().StateHelp().Exists(name))
            ExecutableHelp().StateHelp().Open(name);
        else
            QApplication::beep();
    }
}
bool BCI2000Viewer::StateSelected() const
{
    int currentRow = ui->channelList->currentRow();
    bool isStateEntry = false;
    if (currentRow > 0)
    {
        int i = currentRow;
        QListWidgetItem *p;
        do
        {
            p = ui->channelList->item(i++);
        } while (p->flags() & Qt::ItemIsUserCheckable && i < ui->channelList->count());
        isStateEntry = (i < ui->channelList->count());
    }
    if (isStateEntry)
    {
        ui->actionState_Help->setText(QString(tr("Help on %1")).arg(ui->channelList->currentItem()->text()));
        ui->actionState_Help->setEnabled(true);
    }
    else
    {
        ui->actionState_Help->setText(tr("BCI2000 Help"));
        ui->actionState_Help->setDisabled(true);
    }
    return isStateEntry;
}

void BCI2000Viewer::ToggleNotes()
{
    mpNotesWidget->setVisible(!mpNotesWidget->isVisible());
}

bool BCI2000Viewer::ToggleNotes_Enabled() const
{
    return mpNotesState != nullptr;
}

bool BCI2000Viewer::ToggleNotes_Checked() const
{
    return mpNotesWidget->isVisible();
}


void BCI2000Viewer::ToggleStatesChannels()
{
    ui->channelList->setVisible(!ui->channelList->isVisible());
}

bool BCI2000Viewer::ToggleStatesChannels_Enabled() const
{
    return true;
}

bool BCI2000Viewer::ToggleStatesChannels_Checked() const
{
    return ui->channelList->isVisible();
}

// Internal functions
//----------------------------------------------------------------------------
void BCI2000Viewer::DoFileOpen(const QString &inName)
{
    CloseNotes();
    DestroyAVObjects();
    DestroySecondaryViewer();
    if (mIsPrimary)
    {
        SaveSettings();
        mIsPrimary = false;
        ReadSettings();
    }
    std::string name = inName.toLocal8Bit().constData();
    mFile.Open(name.c_str());
    if (!mFile.IsOpen())
    {
        this->setWindowTitle(QApplication::applicationName());
        ui->signalDisplay->Display().SetNumSamples(0);
        mSamplingRate = 0.0;
        mFileName = "";
        UpdateVerticalScroller();
        if (!inName.isEmpty())
        {
            QString message = QString("Could not open \n\"") + inName + "\"\nas a BCI2000 file.\n";
            QMessageBox::critical(this, QApplication::applicationName(), message);
        }
    }
    else
    {
        mFileName = name;
        this->setWindowTitle(QString("%1 - %2").arg(inName, QApplication::applicationName()));
        ValueList<std::string> units;
        for (int ch = 0; ch < mFile.SignalProperties().Channels(); ++ch)
            units.push_back(mFile.SignalProperties().ValueUnit(ch).RawToPhysical(1));
        ui->signalDisplay->Display()
            .SetNumSamples(std::min<int>(static_cast<int>(mFile.SamplingRate() * 10), mFile.NumSamples()))
            .SetUnitsPerValue(1.0)
            .SetValueUnits(units)
            .SetMinValue(-100)
            .SetMaxValue(100)
            .SetUnitsPerSample(1.0 / mFile.SamplingRate())
            .SetSampleUnit(":s");
        mSamplingRate = mFile.SamplingRate();
    }
    FillChannelList();
    OpenNotes();
    CreateAVObjects();
    CreateSecondaryViewer();
    UpdateChannelLabels();
    SetSamplePos(0);
    ui->signalDisplay->Display().SetDisplayGroups(mFile.SignalProperties().Channels());
    UpdateVerticalScroller();
    UpdateActions();
}

void BCI2000Viewer::FillChannelList()
{
    ui->channelList->blockSignals(true);
    ui->channelList->clear();
    if (mFile.IsOpen())
    {
        QListWidgetItem *pItem = new QListWidgetItem(tr("States, Streams, Events"), ui->channelList);
        pItem->setBackground(cHeadingColor);
        pItem->setFlags(Qt::NoItemFlags);
        for (const auto &state : *mFile.States())
        {
            if (!state.Name().empty() && state.Name().front() != '_')
            {
                pItem = new QListWidgetItem(state.Name().c_str(), ui->channelList);
                pItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            }
        }

        pItem = new QListWidgetItem(tr("Channels"), ui->channelList);
        pItem->setBackground(cHeadingColor);
        pItem->setFlags(Qt::NoItemFlags);
        int base = ui->channelList->count(), channelsInFile = mFile.SignalProperties().Channels();
        if (mFile.Parameters()->Exists("ChannelNames"))
        {
            ParamRef ChannelNames = mFile.Parameter("ChannelNames");
            for (int i = 0; i < ChannelNames->NumValues() && i < channelsInFile; ++i)
            {
                pItem = new QListWidgetItem(ChannelNames(i).c_str(), ui->channelList);
                pItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            }
        }
        for (int i = ui->channelList->count() - base; i < channelsInFile; ++i)
        {
            pItem = new QListWidgetItem(QString(tr("Channel %1")).arg(i + 1), ui->channelList);
            pItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }

        int j = 0;
        for (++j; j < ui->channelList->count() && (ui->channelList->item(j)->flags() & Qt::ItemIsUserCheckable); ++j)
            ui->channelList->item(j)->setCheckState(Qt::Unchecked);
        for (++j; j < ui->channelList->count() && (ui->channelList->item(j)->flags() & Qt::ItemIsUserCheckable); ++j)
            ui->channelList->item(j)->setCheckState(Qt::Checked);
    }
    ui->channelList->blockSignals(false);
}

const char* BCI2000Viewer::SettingsPrefix() const
{
    Assert(!(mIsPrimary && mIsSecondary));
    if (mIsPrimary)
        return "Primary ";
    if (mIsSecondary)
        return "Secondary ";
    return "";
}

void BCI2000Viewer::SaveSettings() const
{
    Settings settings(SettingsPrefix());
    settings.beginGroup("Config");
    settings.setValue("Geometry", this->saveGeometry());
    settings.setValue("Splitter", ui->splitter->saveState());
    settings.setValue("WorkingDirectory", QDir::currentPath());
    std::ostringstream oss;
    if (oss << ui->signalDisplay->Display().ChannelColors())
        settings.setValue("ChannelColors", oss.str().c_str());
    settings.setValue("Invert", ui->signalDisplay->Display().Inverted() ? 1 : 0);
    settings.setValue("ShowStatesChannels", ui->channelList->isVisible() ? 1 : 0);
}

void BCI2000Viewer::ReadSettings()
{
    Settings settings(SettingsPrefix());
    settings.beginGroup("Config");
    auto geom = settings.value("Geometry");
    if (geom.isValid())
        this->restoreGeometry(settings.value("Geometry").toByteArray());
    else
        RestoreDefaultGeometry();
    ui->splitter->restoreState(settings.value("Splitter").toByteArray());
    QString wd = settings.value("WorkingDirectory").toString();
    if (!wd.isEmpty())
        QDir::setCurrent(wd);
    std::istringstream iss;
    iss.str(settings.value("ChannelColors").toString().toLocal8Bit().constData());
    ColorList colors;
    if (iss >> colors)
        ui->signalDisplay->Display().SetChannelColors(colors);
    ui->signalDisplay->Display().SetInverted(1 == settings.value("Invert", false).toInt());
    ui->channelList->setVisible(1 == settings.value("ShowStatesChannels", true).toInt());
}

void BCI2000Viewer::RestoreDefaultGeometry()
{
    this->show(); // make sure window frame geometry has been established
#if QT_VERSION < 0x060000
    auto pScreen = windowHandle()->screen();
#else
    auto pScreen = screen();
#endif
    QRect rect = pScreen->availableGeometry(),
          framed = frameGeometry(),
          inner = geometry();
    int titleHeight = inner.top() - framed.top();
    if (mIsPrimary)
    {
        this->move(rect.left(), rect.top());
        this->resize(rect.width(), rect.height() / 2 - titleHeight);
    }
    else if (mIsSecondary)
    {
        this->move(rect.left(), rect.top() + rect.height() / 2);
        this->resize(rect.width(), rect.height() / 2 - titleHeight);
    }
}

void BCI2000Viewer::UpdateSamplePos(bool inPropagate)
{
    ui->signalDisplay->Display().SetSampleOffset(mSamplePos);
    UpdateTimeField();
    UpdateNotes();
    UpdateAVObjects();
    if (inPropagate)
        UpdatePairedViewer();
    UpdateActions();
}

void BCI2000Viewer::UpdateTimeField()
{
    if (mFile.IsOpen())
    {
        TimeValue pos = static_cast<long>((2 * mSamplePos + ui->signalDisplay->Display().NumSamples()) / 2 /
                                          mFile.SamplingRate()),
                  length = static_cast<long>(mFile.NumSamples() / mFile.SamplingRate());
        std::ostringstream oss;
        oss << pos << "/" << length;
        ui->editPosition->setText(oss.str().c_str());
    }
    else
        ui->editPosition->setText("");
    ui->editPosition->setEnabled(mFile.IsOpen());
}

void BCI2000Viewer::UpdateChannelLabels()
{
    if (mFile.IsOpen())
    {
        std::vector<std::string> signalLabels;
        if (mFile.Parameters()->Exists("ChannelNames"))
        {
            ParamRef labelParam = mFile.Parameter("ChannelNames");
            for (int k = 0; k < labelParam->NumValues(); ++k)
                signalLabels.push_back(labelParam(k));
        }
        for (int i = static_cast<int>(signalLabels.size()); i < mFile.SignalProperties().Channels(); ++i)
        {
            std::ostringstream oss;
            oss << i + 1;
            signalLabels.push_back(oss.str());
        }

        LabelList channelLabels;
        int numMarkerChannels = 0;
        int j = 1;
        while (j < ui->channelList->count() && (ui->channelList->item(j)->flags() & Qt::ItemIsUserCheckable))
            ++j;
        int chBase = ++j;
        for (; j < ui->channelList->count() && (ui->channelList->item(j)->flags() & Qt::ItemIsUserCheckable); ++j)
            if (ui->channelList->item(j)->checkState() == Qt::Checked)
                channelLabels.push_back(Label(static_cast<int>(channelLabels.size()), signalLabels[j - chBase]));
        for (int i = 1; i < ui->channelList->count() && (ui->channelList->item(i)->flags() & Qt::ItemIsUserCheckable);
             ++i)
            if (ui->channelList->item(i)->checkState() == Qt::Checked)
            {
                channelLabels.push_back(Label(static_cast<int>(channelLabels.size()),
                                              ui->channelList->item(i)->text().toLocal8Bit().constData()));
                ++numMarkerChannels;
            }
        mNumSignalChannels = static_cast<int>(channelLabels.size()) - numMarkerChannels;
        ui->signalDisplay->Display()
            .SetNumMarkerChannels(numMarkerChannels)
            .SetChannelLabels(channelLabels)
            .SetChannelLabelsVisible(true);
    }
    else
    {
        mNumSignalChannels = 0;
        ui->signalDisplay->Display().SetNumMarkerChannels(0).SetChannelLabels(LabelList());
    }
}

void BCI2000Viewer::UpdateVerticalScroller()
{
    ui->verticalScrollBar->blockSignals(true);
    int numChannels = mNumSignalChannels, pageSize = ui->signalDisplay->Display().DisplayGroups();
    if (pageSize < 1)
        pageSize = 1;
    int scrollMax = numChannels - pageSize;
    if (scrollMax < 0)
        scrollMax = 0;
    if (pageSize > scrollMax)
    {
        ui->verticalScrollBar->setMinimum(0);
        ui->verticalScrollBar->setMaximum(0);
        ui->verticalScrollBar->setEnabled(false);
    }
    else
    {
        ui->verticalScrollBar->setPageStep(pageSize);
        ui->verticalScrollBar->setSingleStep(1);
        ui->verticalScrollBar->setMinimum(0);
        ui->verticalScrollBar->setMaximum(scrollMax);
        ui->verticalScrollBar->setSliderPosition(ui->signalDisplay->Display().TopGroup());
        ui->verticalScrollBar->setEnabled(true);
    }
    ui->verticalScrollBar->blockSignals(false);
}

GenericSignal BCI2000Viewer::ConstructDisplaySignal(int64_t inPos, int64_t inLength)
{
    GenericSignal result;
    if (mFile.IsOpen())
    {
        int64_t startTime = QDateTime::currentMSecsSinceEpoch();
        bool setCursor = mPrevReadSignalDuration > 100;
        if (setCursor)
            QApplication::setOverrideCursor(Qt::WaitCursor);
        int i = 1;
        std::vector<StateRef> states;
        for (; i < ui->channelList->count() && (ui->channelList->item(i)->flags() & Qt::ItemIsUserCheckable); ++i)
            if (ui->channelList->item(i)->checkState() == Qt::Checked)
                states.push_back(mFile.State(ui->channelList->item(i)->text().toLocal8Bit().constData()));
        std::vector<int> channels;
        int base = ++i;
        for (; i < ui->channelList->count() && (ui->channelList->item(i)->flags() & Qt::ItemIsUserCheckable); ++i)
            if (ui->channelList->item(i)->checkState() == Qt::Checked)
                channels.push_back(i - base);

        GenericSignal signal(channels.size() + states.size(), inLength), statevalues(states.size(), inLength);
        GenericSignal::ValueType* pSignal = signal.MutableData();
        GenericSignal::ValueType* pStatevalues = statevalues.MutableData();
        int64_t sampleInFile = inPos;
        for (int64_t sample = 0; sample < signal.Elements() && sampleInFile < mFile.NumSamples();
             ++sample, ++sampleInFile)
        {
            for (int channelIdx = 0; channelIdx < static_cast<int>(channels.size()); ++channelIdx)
                pSignal[signal.LinearIndex(channelIdx, sample)] = mFile.CalibratedValue(channels[channelIdx], sampleInFile);

            mFile.ReadStateVector(sampleInFile);
            for (size_t i = 0; i < states.size(); ++i)
                pStatevalues[statevalues.LinearIndex(i, sample)] = states[i].AsUnsigned();
        }

        if (FilterActive())
        {
            mFilter.Reset();
            result = GenericSignal(signal.Properties());
            // run the filter twice to avoid transient artifacts
            mFilter.Process(signal, result);
            mFilter.Process(signal, result);
        }
        else
        {
            result = signal;
        }

        GenericSignal::ValueType* pResult = result.MutableData();
        if (mRemoveMean)
        {
            for (size_t ch = 0; ch < channels.size(); ++ch)
            {
                double mean = 0.0;
                for (int sample = 0; sample < result.Elements(); ++sample)
                    mean += result(ch, sample);
                mean /= result.Elements();
                for (int sample = 0; sample < result.Elements(); ++sample)
                    pResult[result.LinearIndex(ch, sample)] -= mean;
            }
        }

        for (int ch = 0; ch < statevalues.Channels(); ++ch)
            for (int sample = 0; sample < statevalues.Elements(); ++sample)
                pResult[result.LinearIndex(ch + channels.size(), sample)] = pStatevalues[statevalues.LinearIndex(ch, sample)];

        mFile.ReadStateVector(inPos + inLength - 1);
        mPrevReadSignalDuration = QDateTime::currentMSecsSinceEpoch() - startTime;
        if (setCursor)
            QApplication::restoreOverrideCursor();
    }
    else
    {
        result = GenericSignal(0, 0);
    }
    return result;
}

int64_t BCI2000Viewer::NumSamples() const
{
    return mFile.NumSamples();
}

int64_t BCI2000Viewer::NumDisplaySamples() const
{
    return ui->signalDisplay->Display().NumSamples();
}

void BCI2000Viewer::SetSamplePos(int64_t inPos, bool inPropagate)
{
    if (mFile.IsOpen())
    {
        mSamplePos = inPos;
        if (mSamplePos >= mFile.NumSamples() - NumDisplaySamples())
            mSamplePos = mFile.NumSamples() - NumDisplaySamples() - 1;
        if (mSamplePos < 0)
            mSamplePos = 0;
        GenericSignal signal = ConstructDisplaySignal(mSamplePos, NumDisplaySamples());
        ui->signalDisplay->Display().WrapForward(signal);
    }
    else
    {
        ui->signalDisplay->Display().SetNumSamples(0).WrapForward(GenericSignal(0, 0));
        mSamplePos = 0;
    }
    UpdateSamplePos(inPropagate);
}

void BCI2000Viewer::ScrollSamplePosBy(int64_t inPosDiff, bool inPropagate)
{
    if (mSamplePos + inPosDiff < 0)
    {
        SetSamplePos(0, inPropagate);
        return;
    }
    if (mSamplePos + inPosDiff >= mFile.NumSamples() - NumDisplaySamples())
    {
        SetSamplePos(LastSample(), inPropagate);
        return;
    }
    if (inPosDiff != 0 && mFile.IsOpen())
    {
        if (FilterActive() || mRemoveMean)
        {
            SetSamplePos(mSamplePos + inPosDiff, inPropagate);
        }
        else
        {
            int64_t readCursor = inPosDiff > 0 ? mSamplePos + NumDisplaySamples() : mSamplePos + inPosDiff;
            int64_t length = std::min(::abs(inPosDiff), NumDisplaySamples());
            GenericSignal signal = ConstructDisplaySignal(readCursor, length);
            mSamplePos += inPosDiff;
            if (inPosDiff > 0)
                ui->signalDisplay->Display().ScrollForward(signal);
            else
                ui->signalDisplay->Display().ScrollBack(signal);
            UpdateSamplePos(inPropagate);
        }
    }
}

void BCI2000Viewer::ScrollToSamplePos(int64_t inPos, bool inPropagate)
{
    int64_t delta = inPos - mSamplePos;
    if (delta != 0)
        ScrollSamplePosBy(delta, inPropagate);
}

void BCI2000Viewer::FilterChanged()
{
    SetSamplePos(mSamplePos, false);
}

bool BCI2000Viewer::FilterActive()
{
    return (mFilter.HPCorner() != 0.0) || (mFilter.LPCorner() != 0.0) || (mFilter.NotchCenter() != 0.0);
}

void BCI2000Viewer::OpenNotes()
{
    // Look for a notes file and open it if present
    std::string base = FileUtils::StripExtension(mFileName),
                extpart = FileUtils::ExtractExtension(base); // second extension if present
    base = FileUtils::StripExtension(base); // remove second extension if present
    std::string notesFile = base + "_notes" + extpart + ".txt";
    if (FileUtils::IsFile(notesFile) && mFile.States()->Exists("Notes"))
    {
        mpNotesState = new StateRef(mFile.State("Notes"));
        mpNotesWidget->setNotesFile(notesFile);
        mpNotesWidget->setVisible(true);
    }
    else
    {
        CloseNotes();
    }
}

void BCI2000Viewer::UpdateNotes()
{
    if (mFile.IsOpen() && mpNotesState)
    {
        mFile.ReadStateVector(mSamplePos + NumDisplaySamples() - 1);
        mpNotesWidget->setLinePosition(*mpNotesState);
    }
}

void BCI2000Viewer::CloseNotes()
{
    mpNotesWidget->setNotesFile("");
    mpNotesWidget->setVisible(false);
    delete mpNotesState;
    mpNotesState = nullptr;
}

void BCI2000Viewer::SyncWithNote(int note)
{
    if (mpNotesState && mFile.IsOpen())
    {
        int64_t pos = mSamplePos + NumDisplaySamples() - 1;
        while (pos > NumDisplaySamples() && *mpNotesState >= note + 1)
            mFile.ReadStateVector(pos--);
        while (pos < NumSamples() && *mpNotesState < note + 1)
            mFile.ReadStateVector(pos++);
        SetSamplePos(pos + 1 - NumDisplaySamples());
    }
}

void BCI2000Viewer::Play()
{
    mLastTimerStamp = QDateTime::currentMSecsSinceEpoch();
    mPlaybackTimer.start();
    for (auto p : mAudioPlayers)
        p->Play();
}

void BCI2000Viewer::Pause()
{
    mPlaybackTimer.stop();
    for (auto p : mAudioPlayers)
        p->Pause();
}

void BCI2000Viewer::SetVolume(float volume)
{
    for (auto p : mAudioPlayers)
        p->SetVolume(volume);
}

void BCI2000Viewer::SetPlaybackRatio(float f)
{
    if (f != mPlaybackRatio)
    {
        mPlaybackRatio = f;
        for (auto p : mAudioPlayers)
        {
            Rate r = p->NativeFrameRate();
            p->SetNominalFrameRate(r * Ratio::FromDouble(f));
        }
    }
}

bool BCI2000Viewer::Playing() const
{
    return mPlaybackTimer.isActive();
}

void BCI2000Viewer::PlaybackTimer()
{
    int64_t samples = 0;
    if (mAudioPlayers.empty())
    {
        int64_t now = QDateTime::currentMSecsSinceEpoch();
        samples = mPlaybackRatio * ((now - mLastTimerStamp) * mFile.SamplingRate() / 1000);
        mLastTimerStamp = now;
    }
    else if (mAudioPlayers.front()->IsPlaying())
    {
        int64_t targetFrame = mAudioPlayers.front()->FramePosition();
        int64_t currentFrame = AudioFramePosition();
        double delta_t = (targetFrame - currentFrame) / cAudioExtensionSamplingRate_Hz;
        samples = delta_t * mFile.SamplingRate() + 0.5;
    }
    ScrollSamplePosBy(samples);
    if (!GoForward_Enabled())
        Pause();
    UpdateActions();
}

void BCI2000Viewer::CreateAVObjects()
{
    if (mIsSecondary)
        return;

    Settings settings;
    settings.beginGroup("Config");

    mpAVControl = new AVControl(this);
    mpAVControl->move(10, 60);
    mpAVControl->restoreGeometry(settings.value("AVControl").toByteArray());
    ui->menuWindow->setEnabled(true);
    QAction *pAction = ui->menuWindow->addAction("AV Control");
    connect(pAction, &QAction::triggered, this, &BCI2000Viewer::BringAVControlToFront);

    std::string base = FileUtils::StripExtension(mFileName),
                extpart = FileUtils::ExtractExtension(base); // second extension if present
    base = FileUtils::StripExtension(base); // remove second extension if present

    // first, look for webcam-related states and files
    for (int i = 0; i < cMaxWebcams; ++i)
    {
        std::string name = "WebcamFrame" + std::to_string(i);
        if (mFile.States()->Exists(name))
        {
            std::string videoName = base + "_" + std::to_string(i) + "_vid" + extpart + ".mp4";
            if (FileUtils::IsFile(videoName))
            {
                VideoWidget *pWidget = new VideoWidget(this);
                pWidget->setVideoFile(videoName);
                pWidget->setWindowTitle(QString("Webcam %1").arg(i));
                VideoLink link = { mFile.State(name), pWidget };
                mVideoWidgets.push_back(link);
            }
        }
    }

    // then, look for recorded visualizations and their states
    const std::string stateMark = "VisFrame_";
    for (const auto& state : *mFile.States())
    {
        if (state.Name().find(stateMark) == 0)
        {
            std::string visID = state.Name().substr(stateMark.length());
            std::string videoName = base + "_" + visID + extpart + ".mp4";
            if (FileUtils::IsFile(videoName))
            {
                VideoWidget* pWidget = new VideoWidget(this);
                pWidget->setVideoFile(videoName);
                pWidget->setWindowTitle(QString::fromLocal8Bit(visID.c_str()));
                VideoLink link = { mFile.State(state.Name()), pWidget };
                mVideoWidgets.push_back(link);
            }
        }
    }

    int i = 0;
    for (const auto &w : mVideoWidgets)
    {
        QAction *pAction = ui->menuWindow->addAction(w.widget->windowTitle());
        pAction->setProperty("Window", i);
        connect(pAction, SIGNAL(triggered()), this, SLOT(BringVideoWindowToFront()));
        w.widget->setWindowFlags(Qt::Tool);
        w.widget->resize(w.widget->width() / 2, w.widget->height() / 2);
        w.widget->restoreGeometry(settings.value(QString("Webcam%1").arg(i)).toByteArray());
        w.widget->show();
        ++i;
    }

    if (mFile.States()->Exists("AudioFrame"))
    {
        int audioBufferSize = cAudioExtensionDefaultBufferSize;
        if (mFile.Parameters()->Exists("AudioBufferSize"))
            audioBufferSize = mFile.Parameter("AudioBufferSize");
        double audioBufferDelayMs = audioBufferSize * 1000.0 / cAudioExtensionSamplingRate_Hz;

        mpAudioFrameState = new StateRef(mFile.State("AudioFrame"));
        mFile.ReadStateVector(0);
        int64_t initialValue = *mpAudioFrameState;
        int64_t i = 1;
        while (i < mFile.NumSamples() && *mpAudioFrameState == initialValue)
            mFile.ReadStateVector(i++);
        double delta_t = i / mFile.SamplingRate();
        double delta_audioFrames = delta_t * cAudioExtensionSamplingRate_Hz;
        mAudioFrameOffset = *mpAudioFrameState - delta_audioFrames;
        mFile.ReadStateVector(0);
        std::string audioInName = base + "_in" + extpart,
                    audioOutName = base + "_out" + extpart;
        for (const char *ext : {".wav", ".flac", ".ogg"})
        {
            if (FileUtils::IsFile(audioInName + ext))
            {
                mAudioPlayers.push_back(new AudioPlayer(mpAudioSink));
                mAudioPlayers.back()->SetFile(audioInName + ext);
                mAudioPlayers.back()->SetExternalPlaybackLatency(Time::FromMilliseconds(audioBufferDelayMs));
            }
            if (FileUtils::IsFile(audioOutName + ext))
            {
                mAudioPlayers.push_back(new AudioPlayer(mpAudioSink));
                mAudioPlayers.back()->SetFile(audioOutName + ext);
                mAudioPlayers.back()->SetExternalPlaybackLatency(Time::FromMilliseconds(audioBufferDelayMs));
            }
        }
    }

    if (!mVideoWidgets.empty() || !mAudioPlayers.empty())
        mpAVControl->show();
}

void BCI2000Viewer::UpdateAVObjects()
{
    if (mFile.IsOpen())
        mFile.ReadStateVector(mSamplePos + NumDisplaySamples() - 1);
    for (const auto &entry : mVideoWidgets)
    {
        int64_t position = entry.state;
        if (mSamplePos == 0)
            position = 0;
        entry.widget->setFramePosition(position);
    }
    for (auto p : mAudioPlayers)
        if (!p->IsPlaying())
            p->SetFramePosition(AudioFramePosition());
    if (mpAVControl)
        mpAVControl->updatePosition(mSamplePos);
}

void BCI2000Viewer::DestroyAVObjects()
{
    Settings settings;
    settings.beginGroup("Config");
    int i = 0;
    for (auto &w : mVideoWidgets)
    {
        settings.setValue(QString("Webcam%1").arg(i++), w.widget->saveGeometry());
        delete w.widget;
    }
    mVideoWidgets.clear();
    ui->menuWindow->clear();
    ui->menuWindow->setEnabled(false);

    if (mpAVControl)
    {
        settings.setValue(QString("AVControl"), mpAVControl->saveGeometry());
        delete mpAVControl;
        mpAVControl = nullptr;
    }
    for (auto p : mAudioPlayers)
        delete p;
    mAudioPlayers.clear();

    delete mpAudioFrameState;
    mpAudioFrameState = nullptr;
}

int64_t BCI2000Viewer::AudioFramePosition()
{
    mFile.ReadStateVector(mSamplePos + NumDisplaySamples() - 1);
    return *mpAudioFrameState ? *mpAudioFrameState - mAudioFrameOffset : 0;
}

void BCI2000Viewer::BringAVControlToFront()
{
    if (mpAVControl)
    {
        mpAVControl->show();
        mpAVControl->raise();
        mpAVControl->activateWindow();
    }
}

void BCI2000Viewer::BringVideoWindowToFront()
{
    auto property = sender()->property("Window");
    if (property.isValid())
    {
        bool ok = false;
        int i = property.toInt(&ok);
        if (ok && i >= 0 && i < mVideoWidgets.size())
        {
            QWidget *w = mVideoWidgets[i].widget;
            w->show();
            w->raise();
            w->activateWindow();
        }
    }
}

std::string BCI2000Viewer::GetSecondaryFile(const std::string& inPrimaryFile)
{
    std::string base = FileUtils::StripExtension(inPrimaryFile),
                ext1 = FileUtils::ExtractExtension(inPrimaryFile),
                ext2 = FileUtils::ExtractExtension(base);
    base = FileUtils::StripExtension(base);
    std::string secondaryFile = base + "_1" + ext2 + ext1;
    return secondaryFile;
}

void BCI2000Viewer::CreateSecondaryViewer()
{
    Assert(!mpSecondaryViewer);
    std::string secondaryFile = GetSecondaryFile(mFileName);
    if (FileUtils::IsFile(secondaryFile))
    {
        if (!mIsPrimary)
        {
            SaveSettings();
            mIsPrimary = true;
            ReadSettings();
        }
        secondaryFile = "\"" + secondaryFile + "\"";
        mpSecondaryViewer = new ChildProcess(FileUtils::ExecutablePath(), "--secondary " + secondaryFile);
        mpSecondaryViewer->run();
    }
}

void BCI2000Viewer::UpdatePairedViewer()
{
    std::ostream output(mpSecondaryViewer ? &mpSecondaryViewer->standardIO() : std::cout.rdbuf());
    output.precision(std::numeric_limits<double>::max_digits10);
    output << "pos " << (mSamplePos / mSamplingRate) << std::endl;
}

void BCI2000Viewer::DestroySecondaryViewer()
{
    if (mpSecondaryViewer && mpSecondaryViewer->running())
    {
        Tiny::OStream output(mpSecondaryViewer->standardIO());
        output << "quit" << std::endl;
        mpSecondaryViewer->Wait();
    }
    delete mpSecondaryViewer;
    mpSecondaryViewer = nullptr;
}

void BCI2000Viewer::OnIOThreadTimer()
{
#if _WIN32
    if (mIsPrimary)
    {
        this->showMinimized();
        this->showNormal();
    }
#endif
    mSecondaryViewerThread = std::thread([this] {this->IOThread();});
}

void BCI2000Viewer::IOThread()
{
    std::cout << "ready" << std::endl;

    std::istream input(mpSecondaryViewer ? &mpSecondaryViewer->standardIO() : std::cin.rdbuf());
    std::string line;
    while (std::getline(input, line))
    {
        if (!line.empty() && line.back() == '\r') // Windows text mode
            line.pop_back();

        if (line.find("pos ") == 0)
        {
            double pos = std::strtod(line.c_str() + 4, nullptr);
            int64_t samplePos = pos * mSamplingRate;
            QMetaObject::invokeMethod(this, "ScrollToSamplePos", Qt::QueuedConnection, Q_ARG(int64_t, samplePos), Q_ARG(bool, false));
        }
        else if (line == "ready")
        {
            ; // ignore it
        }
        else if (line == "quit")
        {
            if (mIsSecondary)
            {
                std::cout << "quit" << std::endl;
                QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
            }
            return;
        }
        else
        {
            SuggestDebugging << "unexpected input: " << line;
        }
    }
}

int64_t BCI2000Viewer::LastSample() const
{
    return std::max<int64_t>(0LL, mFile.NumSamples() - ui->signalDisplay->Display().NumSamples());
}
