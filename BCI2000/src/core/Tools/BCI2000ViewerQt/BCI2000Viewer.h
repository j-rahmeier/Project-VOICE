////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000Viewer.h 7925 2024-03-13 12:27:20Z mellinger $
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
#ifndef BCI2000VIEWER_H
#define BCI2000VIEWER_H

#include <QMainWindow>
#include <QtWidgets>

#include "AudioSink.h"
#include "AVControl.h"
#include "BCI2000FileReader.h"
#include "Color.h"
#include "DisplayFilter.h"
#include "GenericSignal.h"
#include "VideoWidget.h"
#include "NotesWidget.h"
#include "ChildProcess.h"

class AudioPlayer;

namespace Ui
{
class BCI2000Viewer;
}

class BCI2000Viewer : public QMainWindow
{
    Q_OBJECT

  public:
    BCI2000Viewer(QWidget *parent = 0);
    ~BCI2000Viewer();

  private:
    Ui::BCI2000Viewer *ui;

  protected:
    // Qt event handlers
    void closeEvent(QCloseEvent *) override;
    void dragEnterEvent(QDragEnterEvent *) override;
    void dropEvent(QDropEvent *) override;
    void keyPressEvent(QKeyEvent *) override;

  private slots:
    void onFileOpen(const QString&);
    void on_signalDisplay_customContextMenuRequested(QPoint pos);
    void on_editPosition_textEdited(QString);
    void on_editPosition_editingFinished();
    void on_editPosition_returnPressed();
    void on_editPosition_selectionChanged();
    void on_channelList_itemChanged(QListWidgetItem *);
    void on_channelList_itemSelectionChanged();
    void on_channelList_currentRowChanged(int currentRow);
    void on_verticalScrollBar_valueChanged(int value);

  private:
    static struct ActionEntry
    {
        typedef void (BCI2000Viewer::*Action)();
        typedef bool (BCI2000Viewer::*StateGetter)() const;
        Action action;
        StateGetter enabled, checked;
        const char *object, *group;
    } sActions[];
    void SetupActions();
    void UpdateActions();
  private slots:
    void ExecuteAction();

  public:
    // User actions
    // Standard actions
    void FileOpen();
    bool FileOpen_Enabled() const;
    void FileOpenInfo();
    bool FileOpenInfo_Enabled() const;
    void FileClose();
    bool FileClose_Enabled() const;
    void FileExport();
    bool FileExport_Enabled() const;
    void FileQuit();
    bool FileQuit_Enabled() const;
    void EditCopy();
    bool EditCopy_Enabled() const;
    void HelpOpenHelp();
    void HelpAbout();
    // Temporal movement
    void ToFirstSample();
    void ToLastSample();
    void ToPrevBlock();
    void ToNextBlock();
    void ToPrevPage();
    void ToNextPage();
    bool GoBack_Enabled() const;
    bool GoForward_Enabled() const;
    // Temporal resolution
    void SampleZoomIn();
    bool SampleZoomIn_Enabled() const;
    void SampleZoomOut();
    bool SampleZoomOut_Enabled() const;
    // Number of displayed channels
    void FewerChannels();
    bool FewerChannels_Enabled() const;
    void MoreChannels();
    bool MoreChannels_Enabled() const;
    // Channel scrolling
    void ChannelUp();
    void ChannelDown();
    void ChannelPageNext();
    void ChannelPagePrev();
    void ChannelPageFirst();
    void ChannelPageLast();
    bool ChannelUp_Enabled();
    bool ChannelDown_Enabled();
    // Signal resolution
    void EnlargeSignal();
    void ReduceSignal();
    bool ChangeResolution_Enabled() const;
    // Display attributes
    void ChooseChannelColors();
    bool ChooseChannelColors_Enabled() const;
    void Invert();
    bool Invert_Checked() const;
    bool Invert_Enabled() const;
    void ToggleBaselines();
    bool ToggleBaselines_Checked() const;
    bool ToggleBaselines_Enabled() const;
    void ToggleUnit();
    bool ToggleUnit_Checked() const;
    bool ToggleUnit_Enabled() const;
    // Filters
    void HPOff();
    void HPRemoveMean();
    void HP1Hz();
    void HP5Hz();
    void LPOff();
    void LP30Hz();
    void LP40Hz();
    void LP70Hz();
    void NotchOff();
    void Notch50Hz();
    void Notch60Hz();
    bool Filter_Enabled() const;
    // Channel List
    void ShowSelectedChannels();
    void HideSelectedChannels();
    bool ChannelsSelected() const;
    void HelpOnState();
    bool StateSelected() const;
    // Notes
    void ToggleNotes();
    bool ToggleNotes_Enabled() const;
    bool ToggleNotes_Checked() const;
    // States/Channels
    void ToggleStatesChannels();
    bool ToggleStatesChannels_Enabled() const;
    bool ToggleStatesChannels_Checked() const;
    // Scrolling
    void ToggleFastScrolling();
    bool ToggleFastScrolling_Checked() const;
    // Decimation
    void ToggleAutoDecimation();
    bool ToggleAutoDecimation_Checked() const;

    int64_t NumSamples() const;
    int64_t NumDisplaySamples() const;
    void SetSamplePos(int64_t sampleIndex, bool propagate = true);
    void ScrollSamplePosBy(int64_t sampleIndexDiff, bool propagate = true);
  public slots:
    void ScrollToSamplePos(int64_t sampleIndex, bool propagate = true);

  public slots:
    void Play();
    void Pause();
    void SetVolume(float);
    void SetPlaybackRatio(float);

  public:
    bool Playing() const;
  private slots:
    void PlaybackTimer();

  private:
    // Internal functions
    void DoFileOpen(const QString &name);
    static std::string GetSecondaryFile(const std::string& primaryFile);
    void FillChannelList();

    const char* SettingsPrefix() const;
    void SaveSettings() const;
    void ReadSettings();
    void RestoreDefaultGeometry();

    void UpdateSamplePos(bool inPropagate);
    void UpdateTimeField();
    void UpdateChannelLabels();
    void UpdateVerticalScroller();
    GenericSignal ConstructDisplaySignal(int64_t samplePos, int64_t length);
    void FilterChanged();
    bool FilterActive();

    void OpenNotes();
    void UpdateNotes();
    void CloseNotes();
  private slots:
    void SyncWithNote(int);

  private:
    void CreateAVObjects();
    void UpdateAVObjects();
    void DestroyAVObjects();
    int64_t AudioFramePosition();
  private slots:
    void BringAVControlToFront();
    void BringVideoWindowToFront();

  private:
    void CreateSecondaryViewer();
    void UpdatePairedViewer();
    void DestroySecondaryViewer();
    void IOThread();
  private slots:
    void OnIOThreadTimer();

  private:
    int64_t LastSample() const;

  private:
    static const RGBColor cAxisColor;
    static const RGBColor cChannelColorsDefault[];
    static const QColor cHeadingColor;
    static const int cAutoDecimationMaxLinesPerPixel;

    BCI2000FileReader mFile;
    int64_t mSamplePos;
    int mNumSignalChannels;
    bool mPositionEdited;
    float mSamplingRate;
    DisplayFilter mFilter;
    bool mRemoveMean;
    std::string mFileName;

    NotesWidget* mpNotesWidget;
    StateRef* mpNotesState;

    AudioSinkRef mpAudioSink;

    AVControl *mpAVControl;
    struct VideoLink
    {
        StateRef state;
        VideoWidget *widget;
    };
    std::vector<VideoLink> mVideoWidgets;
    StateRef *mpAudioFrameState;
    int64_t mAudioFrameOffset;
    std::vector<AudioPlayer *> mAudioPlayers;
    QTimer mPlaybackTimer;
    float mPlaybackRatio;
    int64_t mLastTimerStamp;
    int64_t mPrevReadSignalDuration;

    ChildProcess* mpSecondaryViewer;
    std::thread mSecondaryViewerThread;
    bool mIsPrimary, mIsSecondary;
};

#endif // BCI2000VIEWER_H
