////////////////////////////////////////////////////////////////////////////////
// $Id: AnalysisGui.cpp 7681 2023-10-27 14:19:26Z mellinger $
// Authors: Adam Wilson, mellinger@neurotechcenter.org
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
#include "AnalysisGui.h"
#include "FileUtils.h"
#include "ProcessUtils.h"
#include "Resources.h"
#include "ResultsTreeModel.h"
#include "ScriptEditor.h"
#include "BCI2000FileReader.h"

namespace
{
QString toQString(const std::string &s)
{
    return QString::fromLocal8Bit(s.c_str());
}
std::string toString(const QString &s)
{
    return std::string(s.toLocal8Bit().constData());
}
} // namespace

struct AnalysisGui::FilesListField : CollapsibleGroup, Broadcaster
{
    enum
    {
        onListItemsAdded,
        onListItemsRemoved
    };
    struct MessageArg
    {
        std::string userSelectedDirectory;
    };
    static void listDirectory(QString dirname, const QStringList &filters, QStringList &files)
    {
        QDir dir(dirname);
        for (const auto &p : dir.entryInfoList(filters))
            files.append(p.absoluteFilePath());
        for (const auto &p : dir.entryInfoList(QStringList(), QDir::AllDirs | QDir::NoDotAndDotDot))
            listDirectory(p.absoluteFilePath(), filters, files);
    }
    FilesListField(QString title, QWidget* parent) : CollapsibleGroup(title, parent)
    {
        CollapsibleGroup::setFlat(true);
        auto pLayout = new QVBoxLayout;
        mpFilesList = new QListWidget;
        mpFilesList->setSelectionMode(QAbstractItemView::ExtendedSelection);
        pLayout->addWidget(mpFilesList);
        auto buttons = new QHBoxLayout;
        mpAddFilesButton = new QPushButton("+");
        mpRemoveFilesButton = new QPushButton("-");
        mpRemoveFilesButton->setEnabled(false);
        buttons->addWidget(mpAddFilesButton);
        buttons->addWidget(mpRemoveFilesButton);
        buttons->addStretch(1);
        pLayout->addItem(buttons);
        setContentLayout(pLayout);
        connect(mpAddFilesButton, &QPushButton::clicked, this, &FilesListField::onAddFilesButtonClicked);
        connect(mpRemoveFilesButton, &QPushButton::clicked, this, &FilesListField::onRemoveFilesButtonClicked);
        connect(mpFilesList, &QListWidget::itemSelectionChanged, this, &FilesListField::onFilesListItemSelectionChanged);
        mpFilesList->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(mpFilesList, &QWidget::customContextMenuRequested, this, &FilesListField::onFilesListCustomContextMenuRequested);
    }
    void onAddFilesButtonClicked()
    {
        QString dir = QFileDialog::getExistingDirectory(this, "Select data directory",
                                                        QString::fromLocal8Bit(FileUtils::AbsolutePath(".").c_str()));
        if (dir.size() == 0)
            return;

        bool wasBlocked = mpFilesList->blockSignals(true);
        QStringList files;
        listDirectory(dir, QStringList() << "*.dat", files);
        bool added = false;
        for (const auto &file : qAsConst(files))
        {
            bool found = false;
            for (int row = 0; !found && row < mpFilesList->count(); ++row)
                if (mpFilesList->item(row)->text() == file)
                    found = true;
            if (!found) {
                QListWidgetItem* pItem = new QListWidgetItem(file, mpFilesList);
                BCI2000FileReader reader;
                reader.Open(file.toLocal8Bit().constData(), 0);
                if (!reader.IsOpen()) {
                    pItem->setForeground(Qt::red);
                    pItem->setToolTip("Not a valid BCI2000 data file");
                }
                pItem->setData(Qt::UserRole, reader.IsOpen());
            }
            added = added || !found;
        }
        mpFilesList->blockSignals(wasBlocked);
        if (added)
        {
            MessageArg arg = {dir.toLocal8Bit().constData()};
            Emit(onListItemsAdded, &arg);
        }
        mpRemoveFilesButton->setEnabled(!mpFilesList->selectedItems().empty());
    }
    void onRemoveFilesButtonClicked()
    {
        bool wasBlocked = mpFilesList->blockSignals(true);
        auto selected = mpFilesList->selectedItems();
        for (auto p : qAsConst(selected))
            delete p;
        mpFilesList->blockSignals(wasBlocked);
        if (!selected.empty())
            Emit(onListItemsRemoved);
        mpRemoveFilesButton->setEnabled(false);
    }
    void onFilesListItemSelectionChanged()
    {
        mpRemoveFilesButton->setEnabled(!mpFilesList->selectedItems().empty());
    }
    void onFilesListCustomContextMenuRequested()
    {
        QPoint where = QCursor::pos();
        auto pItem = mpFilesList->itemAt(mpFilesList->mapFromGlobal(where));
        if (pItem)
        {
            QMenu menu;
            auto pFileInfo = menu.addAction("Open with BCI2000FileInfo");
            auto pViewer = menu.addAction("Open with BCI2000Viewer");
            menu.addSeparator();
            auto pRemove = menu.addAction("Remove from list");
            auto pResult = menu.exec(where);
            if (pResult == pFileInfo || pResult == pViewer)
            {
              std::string executable;
              if (pResult == pFileInfo)
              {
                executable = FileUtils::InstallationDirectory() + "../BCI2000FileInfo/BCI2000FileInfo";
              }
              else if (pResult == pViewer)
              {
                executable = FileUtils::InstallationDirectory() + "../BCI2000Viewer/BCI2000Viewer";
              }
              std::string args;
              auto items = mpFilesList->selectedItems();
              for (const auto& item : items)
              {
                args = "\"";
                args += item->text().toLocal8Bit().constData();
                args += "\"";
                if (!executable.empty())
                {
                  int ignored;
                  ProcessUtils::ExecuteAsynchronously(executable, args, ignored);
                }
              }
            }
            else if (pResult == pRemove)
            {
                qDeleteAll(mpFilesList->selectedItems());
            }
        }
    }

    QListWidget *mpFilesList;
    QPushButton *mpAddFilesButton, *mpRemoveFilesButton;
};

namespace {
struct FileSelectorField : CollapsibleGroup
{
    FileSelectorField(const QString &title, QWidget* parent) : CollapsibleGroup(title, parent)
    {
        CollapsibleGroup::setFlat(true);
        setContentLayout(new QHBoxLayout);
        mpEdit = new QLineEdit(parent);
        contentLayout()->addWidget(mpEdit);
        mpButton = new QPushButton("...", parent);
        contentLayout()->addWidget(mpButton);
    }
    QLineEdit *mpEdit;
    QAbstractButton *mpButton;
};
struct ScriptField : CollapsibleGroup
{
    ScriptField(const QString &title, QWidget* parent) : CollapsibleGroup(title, parent)
    {
        CollapsibleGroup::setFlat(true);
        auto pLayout = new QVBoxLayout;
        mpEditor = new ScriptEditor(parent);
        mpEditor->setRows(6);
        mpEditor->setCols(50);
        mpEditor->setEditable(true);
        pLayout->addWidget(mpEditor);
        mpLoadButton = new QPushButton("Load...", parent);
        mpSaveButton = new QPushButton("Save as...", parent);
        auto pButtons = new QHBoxLayout;
        pButtons->addWidget(mpLoadButton);
        pButtons->addWidget(mpSaveButton);
        pButtons->addStretch(1);
        pLayout->addLayout(pButtons);
        connect(mpSaveButton, &QPushButton::clicked, mpEditor, &ScriptEditor::saveAs);
        connect(mpLoadButton, &QPushButton::clicked, mpEditor, &ScriptEditor::reopen);
        setContentLayout(pLayout);
    }
    void setScriptUtf8(const char *name, const char *content = nullptr)
    {
        if (name && !content)
        {
            std::string s;
            std::getline(std::ifstream(name), s, '\0');
            setScriptUtf8(name, s.c_str());
            return;
        }
        else if (content)
        {
            mpEditor->setContentUtf8(content);
        }
        mpEditor->setFileNameUtf8(name ? name : "");
    }
    QAbstractButton *mpLoadButton, *mpSaveButton;
    ScriptEditor *mpEditor;
    std::string mName;
};
struct ProtocolWindow : QWidget
{
    ProtocolWindow(const Tiny::String &filename, int cols = 80, int rows = 25)
    {
        setAttribute(Qt::WA_DeleteOnClose, true);
        setAttribute(Qt::WA_QuitOnClose, false);
        auto pLayout = new QHBoxLayout;
        pLayout->setContentsMargins(0, 0, 0, 0);
        setLayout(pLayout);
        QPlainTextEdit *pEdit = new QPlainTextEdit;
        pEdit->setFont(QFont("Courier", 10));
        pEdit->setTextInteractionFlags(Qt::TextInteractionFlag::TextSelectableByKeyboard |
                                       Qt::TextInteractionFlag::TextSelectableByMouse);
        std::string text;
        if (std::getline(std::ifstream(filename), text, '\0'))
            pEdit->setPlainText(QString::fromUtf8(text.c_str()));
        layout()->addWidget(pEdit);
        setWindowTitle(toQString(filename));
        resize(cols * pEdit->fontMetrics().averageCharWidth(), rows * pEdit->fontMetrics().lineSpacing());
        show();
    }
};
std::string NormalizeFileName(std::string name, const std::string &replacement = "")
{
    size_t pos;
    while ((pos = name.find_first_of(":+-*'#^\"")) != std::string::npos)
        name = name.substr(0, pos) + replacement + name.substr(pos + 1);
    return name;
}
} // namespace

AnalysisGui::AnalysisGui()
    : mProgress(this), mOnFilesListItemsRemoved(&AnalysisGui::onFilesListItemsRemoved, this),
      mOnFilesListItemsAdded(&AnalysisGui::onFilesListItemsAdded, this),
      mOnAnalyzerMessage(&AnalysisGui::onAnalyzerMessage, this)
{
    mProgress.setRange(0, 1);
    mProgress.setValue(1);
    static const int messages[] = {
        Analyzer::onBegin,
        Analyzer::onFileProgress,
        Analyzer::onDataProgress,
        Analyzer::onDone,
    };
    for (auto msg : messages)
        mAnalyzer.AddListener(msg, &mOnAnalyzerMessage);
    connect(&mProgress, SIGNAL(canceled()), this, SLOT(onProgressCanceled()));

    setMinimumWidth(500);
    setStyleSheet("QSplitter::handle{background:lightgray;margin-top:8px;margin-bottom:8px}");
    mpSplitter = new QSplitter(Qt::Vertical);
    mpSplitter->setHandleWidth(1);

    auto pTopArea = new QWidget;
    auto pTopLayout = new QVBoxLayout;
    pTopArea->setLayout(pTopLayout);
    pTopLayout->setContentsMargins(0, 0, 0, 0);

    mpFilesList = new FilesListField("Input files", this);
    mpFilesList->setCollapsed(false);
    connect(mpFilesList, &FilesListField::toggled, this, &AnalysisGui::onGroupCollapsed);
    pTopLayout->addWidget(mpFilesList);

    if (!mAnalyzer.init())
    {
        Tiny::String text = mAnalyzer.error();
        auto pLabel = new QLabel(toQString(text));
        pLabel->setForegroundRole(QPalette::ColorRole::Highlight);
        pTopLayout->addWidget(pLabel);
    }
    else if (!mAnalyzer.preferences().empty())
    {
        auto prefsArea = new CollapsibleGroup("Analysis preferences", this);
        prefsArea->setFlat(true);
        auto pPrefsLayout = new QFormLayout;
        int row = 0;
        for (const auto &pref : mAnalyzer.preferences())
        {
            Tiny::String desc = pref.description;
            if (desc.Empty())
                desc = pref.name;
            Tiny::String tooltip = pref.comment;
            if (tooltip.Empty())
                tooltip = desc;
            Tiny::String value;
            value << pref.value;
            auto pEdit = new QLineEdit(toQString(value));
            pEdit->setToolTip(toQString(tooltip));
            pEdit->setValidator(new QDoubleValidator(pEdit));
            pEdit->setProperty("preference", QVariant::fromValue((void *)&pref.value));
            pPrefsLayout->addRow(toQString(desc), pEdit);
        }
        prefsArea->setContentLayout(pPrefsLayout);
        pTopLayout->addWidget(prefsArea);
        prefsArea->setCollapsed(true);
        connect(prefsArea, &CollapsibleGroup::toggled, this, &AnalysisGui::onGroupCollapsed);
    }

    auto pReqs = new ScriptField("Override requirements", this);
    pReqs->setUsingCheckBox(true);
    pReqs->setCollapsed(true);
    connect(pReqs, &ScriptField::toggled, this, &AnalysisGui::onGroupCollapsed);
    mpRequirementsGroup = pReqs;
    mpRequirements = pReqs->mpEditor;
    pTopLayout->addWidget(pReqs);

    auto pInputScript = new ScriptField("Override input script", this);
    pInputScript->setUsingCheckBox(true);
    pInputScript->setCollapsed(true);
    connect(pInputScript, &ScriptField::toggled, this, &AnalysisGui::onGroupCollapsed);
    mpInputScriptGroup = pInputScript;
    mpInputScript = pInputScript->mpEditor;
    pTopLayout->addWidget(pInputScript);

    auto pAnalysisScript = new ScriptField("Override analysis script", this);
    pAnalysisScript->setUsingCheckBox(true);
    pAnalysisScript->setCollapsed(true);
    connect(pAnalysisScript, &ScriptField::toggled, this, &AnalysisGui::onGroupCollapsed);
    mpAnalysisScriptGroup = pAnalysisScript;
    mpAnalysisScript = pAnalysisScript->mpEditor;
    pTopLayout->addWidget(pAnalysisScript);

    pTopLayout->addStretch();

    mpRunButton = new QPushButton("Run");
    mpRunButton->setEnabled(false);
    pTopLayout->addWidget(mpRunButton, 0, Qt::AlignRight);

    mpSplitter->addWidget(pTopArea);

    QWidget *pBottomArea = new QWidget;
    auto pBottomLayout = new QVBoxLayout;
    pBottomArea->setLayout(pBottomLayout);
    pBottomLayout->setContentsMargins(0, 0, 0, 0);

    auto resultsArea = new CollapsibleGroup("Results", this);
    resultsArea->setFlat(true);
    resultsArea->setCollapsed(false);
    connect(resultsArea, &CollapsibleGroup::toggled, this, &AnalysisGui::onGroupCollapsed);
    auto resultsAreaLayout = new QVBoxLayout;
    resultsArea->setContentLayout(resultsAreaLayout);
    mpGlobalResultText = new QLabel;
    resultsAreaLayout->addWidget(mpGlobalResultText);
    mpResultsView = new QTreeView;
    mpResultsView->setContextMenuPolicy(Qt::CustomContextMenu);
    mpResultsView->header()->setStretchLastSection(false);
    mpResultsView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mpResultsView->setHeaderHidden(true);
    connect(mpResultsView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onResultsContextMenu(QPoint)));
    resultsAreaLayout->addWidget(mpResultsView);
    pBottomLayout->addWidget(resultsArea);

    auto pOutput = new FileSelectorField("Output directory", this);
    pOutput->setCollapsed(false);
    connect(pOutput, &FileSelectorField::toggled, this, &AnalysisGui::onGroupCollapsed);
    mpOutputDirLineEdit = pOutput->mpEdit;
    pBottomLayout->addWidget(pOutput);

    pBottomLayout->addStretch();

    mpSaveButton = new QPushButton("Save and view");
    mpSaveButton->setEnabled(false);
    pBottomLayout->addWidget(mpSaveButton, 0, Qt::AlignRight);

    setLayout(new QVBoxLayout);
    layout()->addWidget(mpSplitter);
    mpSplitter->addWidget(pBottomArea);

    connect(pReqs->mpEditor, SIGNAL(textChanged()), this, SLOT(onRequirementsChanged()));
    connect(pOutput->mpButton, SIGNAL(clicked()), this, SLOT(onSetOutputDirButtonClicked()));
    connect(mpRunButton, SIGNAL(clicked()), this, SLOT(onRunButtonClicked()));
    connect(mpSaveButton, SIGNAL(clicked()), this, SLOT(onSaveButtonClicked()));

    QString dataDir, reqFile;
    QStringList args = QApplication::arguments();
    int idx = args.indexOf("--data");
    if (idx > 0 && idx < args.size() - 1)
        dataDir = args[idx + 1];
    idx = args.indexOf("--requirements");
    if (idx > 0 && idx < args.size() - 1)
        reqFile = args[idx + 1];
    idx = args.indexOf("--output");
    if (idx > 0 && idx < args.size() - 1)
        mOutputDir = args[idx + 1];

    if (dataDir.isEmpty())
    {
        dataDir = QApplication::applicationDirPath() + "/../../data";
    }
    else
    {
        QStringList files;
        FilesListField::listDirectory(dataDir, QStringList() << "*.dat", files);
        mpFilesList->mpFilesList->addItems(files);
    }
    mpFilesList->AddListener(FilesListField::onListItemsRemoved, &mOnFilesListItemsRemoved);
    mpFilesList->AddListener(FilesListField::onListItemsAdded, &mOnFilesListItemsAdded);
    FileUtils::ChangeDirectory(dataDir.toLocal8Bit().constData());

    if (reqFile.isEmpty())
    {
        std::string reqScript(Resources::LatencyRequirements.data, Resources::LatencyRequirements.length);
        std::string reqName("./LatencyRequirements.");
        reqName += Resources::LatencyRequirements.type;
        pReqs->setScriptUtf8(reqName.c_str(), reqScript.c_str());
    }
    else
    {
        mpRequirements->open(reqFile.toUtf8().constData());
    }
    pInputScript->setScriptUtf8(
        "./Input.bciscript",
        std::string(Resources::DefaultInputScript.data, Resources::DefaultInputScript.length).c_str());
    pAnalysisScript->setScriptUtf8(
        "./LatencyAnalysis.bciscript",
        std::string(Resources::LatencyAnalysisScript.data, Resources::LatencyAnalysisScript.length).c_str());

    mpRunButton->setEnabled(mpFilesList->mpFilesList->count() > 0);
    mpSplitter->setSizes(QList<int>() << 1 << 0);

    if (!mOutputDir.isEmpty())
        mpOutputDirLineEdit->setPlaceholderText(mOutputDir);
    clearResults();
}

QString AnalysisGui::defaultDir(const QString &path)
{
    QFileInfo info(path);
    if (info.isDir())
        return path;
    if (info.isFile())
        return info.path();
    return QFileInfo(QApplication::applicationFilePath()).path();
}

void AnalysisGui::onGroupCollapsed(bool collapsed)
{
}

void AnalysisGui::onFilesListItemsAdded()
{
    onFilesListItemsRemoved();
    const std::string &dir = mpFilesList->Message().Argument<FilesListField::MessageArg *>()->userSelectedDirectory;
    FileUtils::ChangeDirectory(dir);
    mpOutputDirLineEdit->setPlaceholderText(QString::fromLocal8Bit(dir.c_str()));
}

void AnalysisGui::onFilesListItemsRemoved()
{
    mpRunButton->setEnabled(mpFilesList->mpFilesList->count() > 0);
}

void AnalysisGui::onRunButtonClicked()
{
    std::vector<Tiny::String> files;
    for (int i = 0; i < mpFilesList->mpFilesList->count(); ++i) {
        if (mpFilesList->mpFilesList->item(i)->data(Qt::UserRole).toBool())
            files.push_back(toString(mpFilesList->mpFilesList->item(i)->text()));
    }
    auto fields = findChildren<QLineEdit *>();
    for (auto field : qAsConst(fields))
    {
        auto p = field->property("preference");
        if (p.isValid())
        {
            auto ptr = static_cast<double *>(p.value<void *>());
            if (ptr)
                *ptr = field->text().toDouble();
        }
    }
    mAnalyzer.setInputFiles(files);
    if (mpInputScriptGroup->isCollapsed())
        mAnalyzer.setOverrideInputScript("");
    else
        mAnalyzer.setOverrideInputScript(mpInputScript->contentUtf8());
    if (mpAnalysisScriptGroup->isCollapsed())
        mAnalyzer.setOverrideAnalysisScript("");
    else
        mAnalyzer.setOverrideAnalysisScript(mpAnalysisScript->contentUtf8());
    if (mpRequirementsGroup->isCollapsed())
        mAnalyzer.setOverrideRequirementsScript("");
    else
        mAnalyzer.setOverrideRequirementsScript(mpRequirements->contentUtf8());

    clearResults();
    mpRunButton->setEnabled(false);
    mAnalyzer.run();
}

void AnalysisGui::clearResults()
{
    mpResultsView->setModel(nullptr);
    mpRunButton->setEnabled(mpFilesList->mpFilesList->count() > 0);
    mpSaveButton->setEnabled(false);
    setGlobalResult("n/a");
}

void AnalysisGui::setGlobalResult(const QString &text)
{
    mpGlobalResultText->setText(QString("Overall result: <b>%1</b>").arg(text));
}

void AnalysisGui::onSaveButtonClicked()
{
    Tiny::String filename = toString(mpOutputDirLineEdit->text());
    if (filename.Empty())
        filename = toString(mpOutputDirLineEdit->placeholderText());
    filename += "/" + Tiny::String(mOutputFileName);
    if (!mAnalyzer.saveProtocol(filename))
        QMessageBox::critical(this, "Save Error", toQString(mAnalyzer.error()));
    else
        new ProtocolWindow(filename, 95, 30);
}

void AnalysisGui::onSetOutputDirButtonClicked()
{
    QString dir = QFileDialog::getExistingDirectory(
        this, "Select protocol output directory",
        defaultDir(mOutputDir.isEmpty() ? mpOutputDirLineEdit->placeholderText() : mOutputDir));
    if (dir.isEmpty())
        return;
    mOutputDir = dir;
    mpOutputDirLineEdit->setText(mOutputDir);
}

void AnalysisGui::onAnalyzerMessage()
{
    auto arg = mAnalyzer.Message().Argument<Analyzer::MessageArg *>();
    switch (mAnalyzer.Message().Id())
    {
    case Analyzer::onBegin:
        QMetaObject::invokeMethod(this, "onAnalyzerBegin");
        break;
    case Analyzer::onFileProgress:
        QMetaObject::invokeMethod(this, "onFileProgress", Q_ARG(int, arg->cur), Q_ARG(int, arg->total),
                                  Q_ARG(QString, toQString(arg->info)));
        break;
    case Analyzer::onDataProgress:
        QMetaObject::invokeMethod(this, "onDataProgress", Q_ARG(int, arg->cur), Q_ARG(int, arg->total));
        break;
    case Analyzer::onDone:
        QMetaObject::invokeMethod(this, "onAnalyzerDone");
        break;
    }
}

void AnalysisGui::onAnalyzerBegin()
{
    mProgress.setWindowTitle("Analyzing ...");
    mProgress.setCancelButtonText("Cancel");
    mProgress.setMinimumDuration(0);
    mProgress.setMinimum(0);
    mProgress.setMaximum(1);
    mProgress.setValue(0);
    mProgress.show();
}

void AnalysisGui::onFileProgress(int cur, int total, QString info)
{
    mProgress.setLabelText(QString("%3 (%1 of %2)").arg(cur + 1).arg(total).arg(info));
    mFileCur = cur;
    mFileTotal = total;
}

void AnalysisGui::onDataProgress(int cur, int total)
{
    mProgress.setMaximum(total);
    mProgress.setValue(cur);
}

void AnalysisGui::onAnalyzerDone()
{
    mOutputFileName.clear();
    if (!mProgress.wasCanceled())
    {
        auto results = mAnalyzer.results();
        mpResultsView->setModel(new ResultsTreeModel(results, this));
        mpResultsView->expandToDepth(0);
        if (mAnalyzer.finished())
        {
            int total = 0, passed = 0, failed = 0, missing = 0;
            for (auto p : results)
                if (p->description == "global")
                    mOutputFileName = "analysis_protocol_" + p->data_timestamp + ".txt";
            mOutputFileName = NormalizeFileName(mOutputFileName);
            setGlobalResult(QString::fromUtf8(mAnalyzer.globalResult().c_str()));
        }
    }
    mProgress.setValue(mProgress.maximum());
    if (mpSplitter->sizes().back() == 0)
        mpSplitter->setSizes(QList<int>() << 0 << 1);
    mpRunButton->setEnabled(mpFilesList->mpFilesList->count() > 0);
    mpSaveButton->setEnabled(!mOutputFileName.empty());
}

void AnalysisGui::onRequirementsChanged()
{
    clearResults();
}

void AnalysisGui::onProgressCanceled()
{
    mAnalyzer.cancel();
}

void AnalysisGui::onResultsContextMenu(const QPoint &where)
{
    QMenu *menu = nullptr;
    auto model = dynamic_cast<ResultsTreeModel *>(mpResultsView->model());
    if (model)
        menu = model->contextMenu(mpResultsView->indexAt(where));
    if (menu)
        menu->exec(mpResultsView->mapToGlobal(where));
}

void AnalysisGui::closeEvent(QCloseEvent *)
{
    mAnalyzer.cancel();
}
