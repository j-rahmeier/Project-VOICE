////////////////////////////////////////////////////////////////////////////////
// $Id: FilesWidget.cpp 8268 2024-07-23 16:38:37Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A window containing a BCI2000 data file list.
//   Data files are tested for consistency.
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
#include "FilesWidget.h"

#include "BCI2000FileReader.h"
#include "FileDialog.h"
#include "FileUtils.h"
#include "ProcessUtils.h"

#include <QBoxLayout>
#include <QFileInfo>
#include <QListWidget>
#include <QPushButton>
#include <QMenu>
#include <QMessageBox>

namespace
{
struct FileItem : QListWidgetItem
{
    std::string mFile;
    BCI2000FileReader mReader;
    bool mConsistent;

    FileItem(const QString &file)
        : QListWidgetItem(QFileInfo(file).fileName()), mFile(file.toUtf8().constData()), mReader(mFile.c_str()),
          mConsistent(false)
    {
        if (!mReader.IsOpen())
        {
            setForeground(Qt::red);
            setToolTip("Not a BCI2000 data file");
            mConsistent = false;
        }
        else
        {
            FilesWidget::FileClassification fileClass = FilesWidget::classifyFile(&mReader);
            switch (fileClass)
            {
            case FilesWidget::Unknown:
                QListWidgetItem::setText(QListWidgetItem::text() + " (Unidentified Experiment)");
                break;
            case FilesWidget::SMR:
                QListWidgetItem::setText(QListWidgetItem::text() + " (SMR/ERD Experiment)");
                break;
            case FilesWidget::ERP:
                QListWidgetItem::setText(QListWidgetItem::text() + " (P300/ERP Experiment)");
                break;
            default:
                ;
            }
        }
    }
};
} // namespace

struct FilesWidget::Private : QObject
{
    FilesWidget *mpSelf;
    QListWidget *mpFilesList;
    QPushButton *mpPlusButton, *mpMinusButton, *mpUpButton, *mpDownButton;
    const BCI2000FileReader *mpReferenceFile;

    Private(FilesWidget *);

    void itemSelectionChanged();

    void onContextMenu(const QPoint&);
    void onOpenInBCI2000FileInfo();
    void onOpenInBCI2000Viewer();
    void openSelectionWith(const char*);

    void buttonPlus();
    void buttonMinus();
    void buttonUp();
    void buttonDown();

    void checkConsistency();
    static bool filesConsistent(const BCI2000FileReader *, const BCI2000FileReader *);
};

FilesWidget::Private::Private(FilesWidget *parent) : QObject(parent), mpSelf(parent), mpReferenceFile(nullptr)
{
}

void FilesWidget::Private::itemSelectionChanged()
{
    if (mpFilesList->selectedItems().empty())
    {
        mpMinusButton->setEnabled(false);
        mpUpButton->setEnabled(false);
        mpDownButton->setEnabled(false);
    }
    else
    {
        mpMinusButton->setEnabled(true);
        mpUpButton->setEnabled(true);
        mpDownButton->setEnabled(true);
    }
}

void FilesWidget::Private::onContextMenu(const QPoint& pos)
{
    if (mpFilesList->selectedItems().empty())
        return;

    QPoint globalPos = mpFilesList->mapToGlobal(pos);
    QMenu menu;
    menu.addAction("Open in BCI2000FileInfo ...", this, &Private::onOpenInBCI2000FileInfo);
    menu.addAction("Open in BCI2000Viewer ...", this, &Private::onOpenInBCI2000Viewer);
    menu.exec(globalPos);
}

void FilesWidget::Private::onOpenInBCI2000FileInfo()
{
    openSelectionWith("BCI2000FileInfo");
}

void FilesWidget::Private::onOpenInBCI2000Viewer()
{
    openSelectionWith("BCI2000Viewer");
}

void FilesWidget::Private::openSelectionWith(const char* toolName)
{
    std::string toolsDir = FileUtils::ParentDirectory(FileUtils::InstallationDirectory());
    std::string toolPath = toolsDir + "/" + toolName + "/" + toolName;
#if __APPLE__ && USE_MACOS_APPLICATION_BUNDLES
    toolPath += ".app/Contents/MacOS/";
    toolPath += toolName;
#endif
    auto items = mpFilesList->selectedItems();
    for (auto* pItem : items)
    {
        auto pFileItem = dynamic_cast<FileItem*>(pItem);
        const std::string& file = pFileItem->mFile;
        int err = 0;
        if (!ProcessUtils::ExecuteAsynchronously(toolPath, file, err))
        {
            QString msg("Could not execute \"%1\"");
            msg = msg.arg(QString::fromLocal8Bit(toolPath.c_str()));
            QMessageBox::warning(mpSelf, "Warning", msg);
        }
    }
}

void FilesWidget::Private::buttonPlus()
{
    auto files = FileDialog::getOpenFileNames(mpSelf, "Select BCI2000 data file(s)", "BCI2000 data files (*.dat *.bcidat)");
    mpSelf->addFiles(files);
}

void FilesWidget::Private::buttonMinus()
{
    auto selection = mpFilesList->selectedItems();
    for (auto item : selection)
        delete item;
    checkConsistency();
    mpSelf->dataChanged();
}

void FilesWidget::Private::buttonUp()
{
    auto selection = mpFilesList->selectedItems();
    if (!selection.empty())
    {
        int row = mpFilesList->row(selection.front());
        if (row > 0)
        {
            mpFilesList->insertItem(row - 1, mpFilesList->takeItem(row));
            mpFilesList->item(row - 1)->setSelected(true);
        }
        if (row == 1)
            checkConsistency();
        mpSelf->dataChanged();
    }
}

void FilesWidget::Private::buttonDown()
{
    auto selection = mpFilesList->selectedItems();
    if (!selection.empty())
    {
        int row = mpFilesList->row(selection.front());
        if (row < mpFilesList->count() - 1)
        {
            mpFilesList->insertItem(row + 1, mpFilesList->takeItem(row));
            mpFilesList->item(row + 1)->setSelected(true);
        }
        if (row == 0)
            checkConsistency();
        mpSelf->dataChanged();
    }
}

void FilesWidget::Private::checkConsistency()
{
    const BCI2000FileReader *pReference = mpReferenceFile;
    if (!mpReferenceFile && mpFilesList->count() > 0)
    {
        auto pFirst = dynamic_cast<FileItem *>(mpFilesList->item(0));
        pReference = &pFirst->mReader;
    }
    if (pReference)
    {
        for (int i = 0; i < mpFilesList->count(); ++i)
        {
            auto pItem = dynamic_cast<FileItem *>(mpFilesList->item(i));
            if (filesConsistent(pReference, &pItem->mReader))
            {
                pItem->mConsistent = true;
                pItem->setForeground(Qt::black);
                pItem->setToolTip(QString::fromUtf8(pItem->mFile.c_str()));
            }
            else
            {
                pItem->mConsistent = false;
                pItem->setForeground(Qt::red);
                pItem->setToolTip("The file's data properties are inconsistent with the first file in the list");
            }
        }
    }
}

bool FilesWidget::Private::filesConsistent(const BCI2000FileReader *pItem1, const BCI2000FileReader *pItem2)
{
    if (!pItem1->IsOpen())
        return false;
    if (!pItem2->IsOpen())
        return false;
    if (pItem1->SamplingRate() != pItem2->SamplingRate())
        return false;
    if (pItem1->SourceCh() != pItem2->SourceCh())
        return false;
    if (pItem1->SignalProperties().Elements() != pItem2->SignalProperties().Elements())
        return false;
    return true;
}

FilesWidget::FilesWidget(QWidget *parent) : QWidget(parent), p(new Private(this))
{
    QLayout *pLayout = new QVBoxLayout;
    pLayout->setContentsMargins(2, 0, 2, 0);
    p->mpFilesList = new QListWidget(this);
    p->mpFilesList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(p->mpFilesList, &QListWidget::itemSelectionChanged, p, &Private::itemSelectionChanged);
    p->mpFilesList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(p->mpFilesList, &QListWidget::customContextMenuRequested, p, &Private::onContextMenu);
    pLayout->addWidget(p->mpFilesList);
    QLayout *pSubLayout = new QHBoxLayout;
    pLayout->addItem(pSubLayout);
    p->mpPlusButton = new QPushButton("+", this);
    connect(p->mpPlusButton, &QPushButton::pressed, p, &Private::buttonPlus);
    pSubLayout->addWidget(p->mpPlusButton);
    p->mpMinusButton = new QPushButton("-", this);
    p->mpMinusButton->setEnabled(false);
    connect(p->mpMinusButton, &QPushButton::pressed, p, &Private::buttonMinus);
    pSubLayout->addWidget(p->mpMinusButton);
    p->mpUpButton = new QPushButton("up", this);
    p->mpUpButton->setEnabled(false);
    connect(p->mpUpButton, &QPushButton::pressed, p, &Private::buttonUp);
    pSubLayout->addWidget(p->mpUpButton);
    p->mpDownButton = new QPushButton("down", this);
    p->mpDownButton->setEnabled(false);
    connect(p->mpDownButton, &QPushButton::pressed, p, &Private::buttonDown);
    pSubLayout->addWidget(p->mpDownButton);
    setLayout(pLayout);
}

FilesWidget::~FilesWidget()
{
}

std::vector<BCI2000FileReader *> FilesWidget::data() const
{
    std::vector<BCI2000FileReader *> readers;
    for (int i = 0; i < p->mpFilesList->count(); ++i)
    {
        auto pItem = dynamic_cast<FileItem *>(p->mpFilesList->item(i));
        if (pItem->mConsistent && pItem->mReader.IsOpen())
            readers.push_back(&pItem->mReader);
    }
    return readers;
}

FilesWidget::FilesList FilesWidget::files() const
{
    FilesList files;
    for (int i = 0; i < p->mpFilesList->count(); ++i)
    {
        auto pItem = dynamic_cast<FileItem *>(p->mpFilesList->item(i));
        if (pItem->mConsistent && pItem->mReader.IsOpen())
        {
            std::string file = pItem->mFile;
            std::string info = pItem->text().toUtf8().constData();
            files.push_back(std::make_pair(file, info));
        }
    }
    return files;
}

void FilesWidget::setFiles(const FilesList &files)
{
    p->mpFilesList->clear();
    for (const auto &file : files)
    {
        auto pItem = new FileItem(QString::fromLocal8Bit(file.first.c_str()));
        p->mpFilesList->addItem(pItem);
    }
    p->checkConsistency();
    p->mpSelf->dataChanged();
}

void FilesWidget::setReferenceFile(const BCI2000FileReader *pRef)
{
    if (pRef != p->mpReferenceFile)
    {
        p->mpReferenceFile = pRef;
        p->checkConsistency();
    }
}

const BCI2000FileReader *FilesWidget::referenceFile() const
{
    return p->mpReferenceFile;
}

FilesWidget::FileClassification FilesWidget::classifyFile(const BCI2000FileReader *pFile)
{
    if (pFile->States()->Exists("Feedback"))
        return SMR;
    if (pFile->States()->Exists("TargetCode"))
        return SMR;
    if (pFile->States()->Exists("StimulusCode"))
        return ERP;
    return Unknown;
}

void FilesWidget::addFile()
{
    p->buttonPlus();
}

void FilesWidget::addFiles(const QStringList& files)
{
    bool changed = false;
    for (const auto& file : files)
    {
        bool found = false;
        for (int i = 0; i < p->mpFilesList->count(); ++i)
        {
            auto pItemInList = dynamic_cast<FileItem*>(p->mpFilesList->item(i));
            QString fileInList = QString::fromLocal8Bit(pItemInList->mFile.c_str());
            if (fileInList == file)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            auto pItem = new FileItem(file);
            p->mpFilesList->addItem(pItem);
            changed = true;
        }
    }
    p->checkConsistency();
    if (changed)
        dataChanged();
}

