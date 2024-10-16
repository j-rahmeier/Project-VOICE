//////////////////////////////////////////////////////////////////////
// $Id: ParamDisplay.cpp 8117 2024-05-07 14:17:44Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A ParamDisplay class that handles a single parameter's
//       associated user interface, consisting of a group of user
//       interface widgets arranged inside a parent widget.
//       ParamDisplay is the interface to the outside world;
//       actual behavior is implemented by a class hierarchy
//       descending from a "DisplayBase" class.
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
#include "ParamDisplay.h"

#include "BCIStream.h"
#include "Blob.h"
#include "Color.h"
#include "EditMatrix.h"
#include "ExecutableHelp.h"
#include "OperatorUtils.h"
#include "Param.h"
#include "ParsedComment.h"
#include "Preferences.h"
#include "StringUtils.h"
#include "FileUtils.h"

#include <QColorDialog>
#include <QDesktopServices>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QHelpEvent>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QWhatsThis>

#include <sstream>
#include <string>

#define LOAD_MATRIX_FILTER                                                                                             \
    tr("Matrix file (*.txt *." MATRIX_EXTENSION ")"                                                                    \
       ";;All files (*.*)")
#define SAVE_MATRIX_FILTER                                                                                             \
    tr("BCI2000 matrix parameter (*." MATRIX_EXTENSION ")"                                                             \
       ";;Tab delimited matrix file (*.txt)"                                                                           \
       ";;All files (*.*)")
#define ALLFILES_FILTER tr("All files (*.*)")

namespace {
    QString AppRelativeToAbsolutePath(const QString& rel)
    {
        std::string abs = FileUtils::AbsolutePath(
            rel.toLocal8Bit().constData(), 
            FileUtils::InstallationDirectory()
        );
        return QString::fromLocal8Bit(abs.c_str());
    }
}

////////////////////////////////////////////////////////////////////////////////
// ParamDisplay definitions
////////////////////////////////////////////////////////////////////////////////
ParamDisplay::ParamDisplay() : mpDisplay(nullptr)
{
}

ParamDisplay::ParamDisplay(const Param &inParam) : mpDisplay(nullptr)
{
    ParsedComment parsedComment(inParam);
    switch (parsedComment.Kind() & (ParsedComment::shapeMask | ParsedComment::typeMask))
    {
    case ParsedComment::singleEntry | ParsedComment::enumeration:
        mpDisplay = new SingleEntryEnum(parsedComment);
        break;
    case ParsedComment::singleEntry | ParsedComment::boolean:
        mpDisplay = new SingleEntryBoolean(parsedComment);
        break;
    case ParsedComment::singleEntry | ParsedComment::inputfile:
        mpDisplay = new SingleEntryInputFile(parsedComment);
        break;
    case ParsedComment::singleEntry | ParsedComment::outputfile:
        mpDisplay = new SingleEntryOutputFile(parsedComment);
        break;
    case ParsedComment::singleEntry | ParsedComment::directory:
        mpDisplay = new SingleEntryDirectory(parsedComment);
        break;
    case ParsedComment::singleEntry | ParsedComment::color:
        mpDisplay = new SingleEntryColor(parsedComment);
        break;
    case ParsedComment::singleEntry | ParsedComment::blob:
        mpDisplay = new SingleEntryBlob(parsedComment);
        break;
    default:
        switch (parsedComment.Kind() & ParsedComment::shapeMask)
        {
        case ParsedComment::singleEntry:
            mpDisplay = new SingleEntryEdit(parsedComment);
            break;
        case ParsedComment::list:
            mpDisplay = new List(parsedComment);
            break;
        case ParsedComment::matrix:
            mpDisplay = new Matrix(parsedComment);
            break;
        default:
            Assert(false);
        }
    }
}

ParamDisplay::~ParamDisplay()
{
}

QWidget* ParamDisplay::GetWidget()
{
    return mpDisplay;
}

void ParamDisplay::WriteValuesTo(Param &p) const
{
    if (mpDisplay)
        mpDisplay->WriteValuesTo(p);
}

void ParamDisplay::ReadValuesFrom(const Param &p)
{
    if (mpDisplay)
        mpDisplay->ReadValuesFrom(p);
}

bool ParamDisplay::Modified() const
{
    return mpDisplay && mpDisplay->Modified();
}

////////////////////////////////////////////////////////////////////////////////
// ParamLabel definitions
////////////////////////////////////////////////////////////////////////////////
ParamLabel::ParamLabel(const ParsedComment &inParsedComment)
    : mParamName(inParsedComment.Name()), mHelpContext(inParsedComment.HelpContext())
{
    setWhatsThis("dummy");
}

bool ParamLabel::event(QEvent *inpEvent)
{
    bool result = false;
    switch (inpEvent->type())
    {
    case QEvent::WhatsThis: {
        QWhatsThis::leaveWhatsThisMode();
        if (ExecutableHelp().ParamHelp().Exists(mParamName))
            ExecutableHelp().ParamHelp().Open(mParamName, mHelpContext);
        result = true;
    }
    break;
    default:
        result = QLabel::event(inpEvent);
        break;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// DisplayBase definitions
////////////////////////////////////////////////////////////////////////////////
DisplayBase::DisplayBase(const ParsedComment &inParam)
    : mModified(false), mpUserLevel(nullptr), mpCentralWidget(nullptr)
{
    QLayout* pLayout = new QHBoxLayout;
    pLayout->setContentsMargins(0, 0, 0, 0);

    // add the parameter's name
    ParamLabel *pLabel = new ParamLabel(inParam);
    pLabel->setText(QString::fromUtf8(inParam.Name().c_str()));
    QFont font = pLabel->font();
    font.setWeight(QFont::Bold);
    pLabel->setFont(font);
    pLabel->setMinimumWidth(Measure(ParamName));
    pLabel->setToolTip(QString::fromUtf8(inParam.Path().c_str()));
    pLayout->addWidget(pLabel);

    mpCentralWidget = new QWidget;
    mpCentralWidget->setLayout(new QVBoxLayout);
    int margin = Measure(Margin);
    mpCentralWidget->layout()->setContentsMargins(0, margin, margin, margin);
    pLayout->addWidget(mpCentralWidget);

    // render the parameter's User Level track bar
    // _only_ if the current user level is "advanced"
    if (OperatorUtils::UserLevel() == Preferences::Advanced)
    {
        mpUserLevel = new QSlider;
        mpUserLevel->setOrientation(Qt::Horizontal);
        mpUserLevel->setRange(1, 3);
        mpUserLevel->setSingleStep(1);
        mpUserLevel->setPageStep(1);
        mpUserLevel->setTickInterval(1);
        mpUserLevel->setTickPosition(QSlider::TicksBelow);
        mpUserLevel->setTracking(false);
        mpUserLevel->setFixedWidth(Measure(UserLevel));
        connect(mpUserLevel, SIGNAL(valueChanged(int)), this, SLOT(OnContentChange()));

        pLayout->addWidget(mpUserLevel);
    }
    setLayout(pLayout);
}

DisplayBase::~DisplayBase()
{
}

void DisplayBase::WriteValuesTo(Param &inParam) const
{
    if (mpUserLevel)
        OperatorUtils::SetUserLevel(inParam.Path().c_str(), mpUserLevel->value());
}

void DisplayBase::ReadValuesFrom(const Param &inParam)
{
    if (mpUserLevel)
        mpUserLevel->setValue(OperatorUtils::GetUserLevel(inParam.Path().c_str()));
    mModified = false;
}

int DisplayBase::Measure(int object)
{
    // We need to define object width in terms of text width
    // to make it work on high DPI displays.
    QString s;
    switch (object)
    {
    case ParamName:
        s = "VisualizeSourceDecimation ";
        break;
    case ParamValue:
        s = "auto";
        break;
    case UserLevel:
        s = "_|___|___|_";
        break;
    case Comment:
        s = QString(40, 'x');
        break;
    case EllipsisButton:
        s = "  ...  ";
        break;
    case Margin:
        s = "x";
        break;
    default:
        ;
    }
    return fontMetrics().horizontalAdvance(s);
}

////////////////////////////////////////////////////////////////////////////////
// SeparateComment definitions
////////////////////////////////////////////////////////////////////////////////
SeparateComment::SeparateComment(const ParsedComment &inParam) : DisplayBase(inParam)
{
    // render the parameter's comment
    QLabel *pComment = new QLabel;
    pComment->setText(QString::fromUtf8(inParam.Comment().c_str()));
    QFont font = pComment->font();
    font.setItalic(true);
    pComment->setFont(font);
    pComment->setToolTip(QString::fromUtf8(inParam.Comment().c_str()));
    pComment->setWordWrap(true);
    pComment->setMinimumWidth(Measure(Comment));

    CentralWidget()->layout()->addWidget(pComment);
    QWidget* pWidget = new QWidget;
    CentralWidget()->layout()->addWidget(pWidget);
    SetCentralWidget(pWidget);
    pWidget->setLayout(new QHBoxLayout);
    pWidget->layout()->setContentsMargins(0, 0, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////
// SingleEntryEdit definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryEdit::SingleEntryEdit(const ParsedComment &inParam)
    : SeparateComment(inParam), mpEdit(nullptr)
{
    mpEdit = new QLineEdit;
    mpEdit->setReadOnly(inParam.Kind() & ParsedComment::noedit);
    mpEdit->setToolTip("");
    mpEdit->setMinimumWidth(Measure(ParamValue));
    connect(mpEdit, SIGNAL(textChanged(QString)), this, SLOT(OnEditChange()));
    CentralWidget()->layout()->addWidget(mpEdit);
}

void SingleEntryEdit::WriteValuesTo(Param &outParam) const
{
    outParam.Value() = mpEdit->text().toLocal8Bit();
    DisplayBase::WriteValuesTo(outParam);
}

void SingleEntryEdit::ReadValuesFrom(const Param &inParam)
{
    mpEdit->setText(QString::fromLocal8Bit(inParam.Value().c_str()));
    DisplayBase::ReadValuesFrom(inParam);
}

void SingleEntryEdit::OnEditChange()
{
    mpEdit->setToolTip(mpEdit->text());
    DisplayBase::OnContentChange();
}

////////////////////////////////////////////////////////////////////////////////
// List definitions
////////////////////////////////////////////////////////////////////////////////
List::List(const ParsedComment &inParam)
    : SingleEntryButton(inParam), mName(inParam.Name()), mKind(inParam.Kind())
{
}

void List::ButtonClick()
{
    Param param(mName, "", "list");
    TextToParam(param);
    EditMatrix editor;
    editor.SetColumnKind(0, mKind);
    editor.SetDisplayedParam(&param, mKind);
    editor.exec();
    ParamToText(param);
}

void List::ParamToText(const Param &inParam)
{
    std::ostringstream oss;
    if (inParam.NumValues() > 0)
    {
        oss << EncodedString(inParam.Value(0));
        for (int i = 1; i < inParam.NumValues(); ++i)
            oss << ' ' << EncodedString(inParam.Value(i));
    }
    mpEdit->setText(QString::fromLocal8Bit(oss.str().c_str()));
}

void List::TextToParam(Param &outParam) const
{
    std::istringstream is(mpEdit->text().toLocal8Bit().constData());
    EncodedString value;
    int index = 0;
    outParam.SetNumValues(0);
    while (is >> value)
    {
        outParam.SetNumValues(index + 1);
        Param param;
        std::istringstream iss(value);
        if (iss >> param)
          outParam.Value(index++) = param;
        else
          outParam.Value(index++) = value;
    }
}

void List::WriteValuesTo(Param &outParam) const
{
    if (!(mKind & ParsedComment::accessMask))
    {
        TextToParam(outParam);
        DisplayBase::WriteValuesTo(outParam);
    }
}

void List::ReadValuesFrom(const Param &inParam)
{
    ParamToText(inParam);
    DisplayBase::ReadValuesFrom(inParam);
}

////////////////////////////////////////////////////////////////////////////////
// Matrix definitions
////////////////////////////////////////////////////////////////////////////////
Matrix::Matrix(const ParsedComment &inParam)
    : SeparateComment(inParam), mKind(inParam.Kind()), mMatrixWindowOpen(false)
{
    bool noedit = (inParam.Kind() & ParsedComment::accessMask);
    QPushButton *pEditButton = new QPushButton;
    pEditButton->setText(noedit ? "View..." : "Edit...");
    connect(pEditButton, SIGNAL(clicked()), this, SLOT(OnEditButtonClick()));
    CentralWidget()->layout()->addWidget(pEditButton);

    QPushButton *pLoadButton = new QPushButton;
    pLoadButton->setText(tr("Load..."));
    pLoadButton->setEnabled(!noedit);
    connect(pLoadButton, SIGNAL(clicked()), this, SLOT(OnLoadButtonClick()));
    CentralWidget()->layout()->addWidget(pLoadButton);

    QPushButton *pSaveButton = new QPushButton;
    pSaveButton->setText(tr("Save..."));
    connect(pSaveButton, SIGNAL(clicked()), this, SLOT(OnSaveButtonClick()));
    CentralWidget()->layout()->addWidget(pSaveButton);

    auto pLayout = dynamic_cast<QBoxLayout*>(CentralWidget()->layout());
    if (pLayout)
        pLayout->addStretch();
}

void Matrix::WriteValuesTo(Param &outParam) const
{
    outParam = mParam;
    DisplayBase::WriteValuesTo(outParam);
}

void Matrix::ReadValuesFrom(const Param &inParam)
{
    mParam = inParam;
    if (mMatrixWindowOpen)
        gpEditMatrix->SetDisplayedParam(&mParam, mKind);
    DisplayBase::ReadValuesFrom(inParam);
}

void Matrix::OnEditButtonClick()
{
    mMatrixWindowOpen = true;
    if (gpEditMatrix == nullptr)
        gpEditMatrix = new EditMatrix;
    gpEditMatrix->SetDisplayedParam(&mParam, mKind);
    gpEditMatrix->exec();
    mMatrixWindowOpen = false;
    DisplayBase::OnContentChange();
}

void Matrix::OnLoadButtonClick()
{
    QDir &curDir = OperatorUtils::CurrentDir();
    QString fileName =
        QFileDialog::getOpenFileName(NULL, tr("Open Matrix File"), curDir.canonicalPath(), LOAD_MATRIX_FILTER);
    if (!fileName.isEmpty())
    {
        curDir.setPath(QFileInfo(fileName).canonicalPath());
        int result = OperatorUtils::LoadMatrix(fileName, mParam);
        switch (result)
        {
        case OperatorUtils::NoError:
            break;
        case OperatorUtils::MatLoadColsDiff:
            QMessageBox::critical(NULL, tr("Error"), tr("Number of columns differs across rows."));
            break;
        case OperatorUtils::MatNotFound:
            QMessageBox::critical(NULL, tr("Error"), tr("Could not open \"%1\" as a matrix data file."));
            break;
        default:
            QMessageBox::critical(NULL, tr("Error"), tr("Error loading \"%1\" as a matrix file."));
        }
        DisplayBase::OnContentChange();
    }
}

void Matrix::OnSaveButtonClick()
{
    QDir &curDir = OperatorUtils::CurrentDir();
    QString fileName =
        QFileDialog::getSaveFileName(NULL, tr("Save Matrix File"), curDir.canonicalPath(), SAVE_MATRIX_FILTER);
    if (!fileName.isEmpty())
    {
        curDir.setPath(QFileInfo(fileName).canonicalPath());
        int result = OperatorUtils::SaveMatrix(fileName, mParam);
        switch (result)
        {
        case OperatorUtils::NoError:
            break;
        case OperatorUtils::CannotWriteNestedMatrixAsText:
            QMessageBox::critical(
                NULL, tr("Error"),
                tr("You are saving a \"nested\" matrix. Nested matrices cannot be written in text format.\n\n"
                   "Please choose \"BCI2000 matrix parameter\" as a file type.\n"));
            break;
        case OperatorUtils::CouldNotWrite:
        default:
            QMessageBox::critical(NULL, tr("Error"), tr("Could not write to file \"%1\".").arg(fileName));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// SingleEntryBlob definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryBlob::SingleEntryBlob(const ParsedComment &inParam)
    : SeparateComment(inParam), mpData(nullptr), mpParam(nullptr), mKind(inParam.Kind())
{
    QPushButton *p = new QPushButton;
    p->setText("View...");
    connect(p, SIGNAL(clicked()), this, SLOT(OnViewButtonClick()));
    CentralWidget()->layout()->addWidget(p);
    mpViewButton = p;

    p = new QPushButton;
    p->setText("Load...");
    p->setEnabled((inParam.Kind() & ParsedComment::accessMask) == 0);
    connect(p, SIGNAL(clicked()), this, SLOT(OnLoadButtonClick()));
    CentralWidget()->layout()->addWidget(p);
    mpLoadButton = p;

    p = new QPushButton;
    p->setText("Save...");
    connect(p, SIGNAL(clicked()), this, SLOT(OnSaveButtonClick()));
    CentralWidget()->layout()->addWidget(p);
    mpSaveButton = p;
}

SingleEntryBlob::~SingleEntryBlob()
{
    delete mpData;
}

void SingleEntryBlob::WriteValuesTo(Param &outParam) const
{
    if ((mKind & ParsedComment::noedit) == 0)
    {
        if (mpData)
        {
            std::ostringstream oss;
            mpData->InsertInto(oss);
            outParam.Value() = oss.str();
        }
        bool notEmpty = *outParam.Value().c_str();
        mpViewButton->setEnabled(notEmpty);
        mpSaveButton->setEnabled(notEmpty);
        DisplayBase::WriteValuesTo(outParam);
    }
}

void SingleEntryBlob::ReadValuesFrom(const Param &inParam)
{
    mpParam = &inParam;
    delete mpData;
    mpData = 0;
    bool notEmpty = *inParam.Value().c_str();
    mpViewButton->setEnabled(notEmpty);
    mpSaveButton->setEnabled(notEmpty);
    DisplayBase::ReadValuesFrom(inParam);
}

void SingleEntryBlob::ReadData()
{
    Assert(mpParam);
    if (!mpData)
    {
        mpData = new Blob;
        std::istringstream iss(mpParam->Value().AsString() + " ");
        mpData->ExtractFrom(iss);
    }
}

void SingleEntryBlob::OnViewButtonClick()
{
    ReadData();
    if (!mpData->SaveAsTemp())
        bcierr__ << "Could not write temporary file";
    else
    {
        std::string file = FileUtils::NormalizedPath(mpData->FileName());
        if (!QDesktopServices::openUrl(QUrl(("file:///" + file).c_str(), QUrl::TolerantMode)))
            bcierr__ << "Could not open file \"" << file << "\" in viewer, check OS configuration";
    }
}

void SingleEntryBlob::OnLoadButtonClick()
{
    QDir &curDir = OperatorUtils::CurrentDir();
    QString fileName = QFileDialog::getOpenFileName(0, "Open file", curDir.canonicalPath());
    if (!fileName.isEmpty())
    {
        curDir.setPath(QFileInfo(fileName).canonicalPath());
        delete mpData;
        mpData = new Blob(fileName.toLocal8Bit().constData());
        DisplayBase::OnContentChange();
    }
}

void SingleEntryBlob::OnSaveButtonClick()
{
    QDir &curDir = OperatorUtils::CurrentDir();
    QString fileName = QFileDialog::getSaveFileName(0, "Save as:", curDir.canonicalPath());
    if (!fileName.isEmpty())
    {
        curDir.setPath(QFileInfo(fileName).canonicalPath());
        ReadData();
        if (!mpData->SaveAs(fileName.toLocal8Bit().constData()))
            QMessageBox::critical(0, "Error", tr("Could not write to file \"%1\".").arg(fileName));
    }
}

////////////////////////////////////////////////////////////////////////////////
// SingleEntryButton definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryButton::SingleEntryButton(const ParsedComment &inParam)
    : SingleEntryEdit(inParam)
{
    QPushButton *pButton = new QPushButton;
    pButton->setText("...");
    pButton->setFixedWidth(Measure(EllipsisButton));
    pButton->setEnabled((inParam.Kind() & ParsedComment::accessMask) == 0);
    connect(pButton, SIGNAL(clicked()), this, SLOT(OnButtonClick()));
    CentralWidget()->layout()->addWidget(pButton);
    mComment = inParam.Comment();
}

void SingleEntryButton::OnButtonClick()
{
    ButtonClick();
}

////////////////////////////////////////////////////////////////////////////////
// SingleEntryInputFile definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryInputFile::SingleEntryInputFile(const ParsedComment &inParam)
    : SingleEntryButton(inParam)
{
}

void SingleEntryInputFile::ButtonClick()
{
    QString fileName = QFileDialog::getOpenFileName(
        mpEdit->parentWidget(), tr("Choosing %1").arg(QString::fromUtf8(mComment.c_str())),
        AppRelativeToAbsolutePath(mpEdit->text()), ALLFILES_FILTER);
    if (!fileName.isEmpty())
        mpEdit->setText(fileName);
}

////////////////////////////////////////////////////////////////////////////////
// SingleEntryOutputFile definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryOutputFile::SingleEntryOutputFile(const ParsedComment &inParam)
    : SingleEntryButton(inParam)
{
}

void SingleEntryOutputFile::ButtonClick()
{
    QString fileName = QFileDialog::getSaveFileName(
        mpEdit->parentWidget(), tr("Choosing %1").arg(QString::fromUtf8(mComment.c_str())),
        AppRelativeToAbsolutePath(mpEdit->text()), ALLFILES_FILTER);
    if (!fileName.isEmpty())
        mpEdit->setText(fileName);
}

////////////////////////////////////////////////////////////////////////////////
// SingleEntryDirectory definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryDirectory::SingleEntryDirectory(const ParsedComment &inParam)
    : SingleEntryButton(inParam)
{
}

void SingleEntryDirectory::ButtonClick()
{
    QString dirName = QFileDialog::getExistingDirectory(
        mpEdit->parentWidget(), tr("Choosing %1").arg(QString::fromUtf8(mComment.c_str())),
        AppRelativeToAbsolutePath(mpEdit->text()));
    if (!dirName.isEmpty())
        mpEdit->setText(dirName);
}

////////////////////////////////////////////////////////////////////////////////
// SingleEntryColor definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryColor::SingleEntryColor(const ParsedComment &inParam)
    : SingleEntryButton(inParam)
{
}

void SingleEntryColor::ButtonClick()
{
    RGBColor color;
    std::istringstream iss(mpEdit->text().toLocal8Bit().constData());
    if (!(iss >> color))
        color = RGBColor::Black;
    QColor c = QRgb(color);
    c = QColorDialog::getColor(c, NULL, tr("Choosing %1").arg(QString::fromUtf8(mComment.c_str())));
    if (c.isValid())
    {
        color = c.rgb();
        std::ostringstream oss;
        oss << color;
        mpEdit->setText(QString::fromLocal8Bit(oss.str().c_str()));
    }
}
////////////////////////////////////////////////////////////////////////////////
// SingleEntryEnum definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryEnum::SingleEntryEnum(const ParsedComment &inParam)
    : SeparateComment(inParam), mpComboBox(nullptr), mIndexBase(inParam.IndexBase())
{
    mpComboBox = new QComboBox;
    for (size_t i = 0; i < inParam.Values().size(); ++i)
        mpComboBox->addItem(QString::fromUtf8(inParam.Values()[i].c_str()));
    mpComboBox->setToolTip(QString::fromUtf8(inParam.Comment().c_str()));
    mpComboBox->setEnabled(!(inParam.Kind() & ParsedComment::accessMask));
    connect(mpComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnContentChange()));
    auto pLayout = dynamic_cast<QHBoxLayout*>(CentralWidget()->layout());
    if (pLayout)
    {
        pLayout->addWidget(mpComboBox);
        pLayout->addStretch();
    }
}

void SingleEntryEnum::WriteValuesTo(Param &outParam) const
{
    if (mpComboBox->currentText() == "auto")
        outParam.Value() = "auto";
    else
        outParam.Value() = StringUtils::String() << mpComboBox->currentIndex() + mIndexBase;
    SeparateComment::WriteValuesTo(outParam);
}

void SingleEntryEnum::ReadValuesFrom(const Param &inParam)
{
    mpComboBox->setCurrentIndex(::atoi(inParam.Value().c_str()) - mIndexBase);
    SeparateComment::ReadValuesFrom(inParam);
}

////////////////////////////////////////////////////////////////////////////////
// SingleEntryBoolean definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryBoolean::SingleEntryBoolean(const ParsedComment &inParam)
    : DisplayBase(inParam), mpCheckBox(nullptr)
{
    mpCheckBox = new QCheckBox;
    mpCheckBox->setToolTip(QString::fromUtf8(inParam.Comment().c_str()));
    mpCheckBox->setText(QString::fromUtf8(inParam.Comment().c_str()));
    mpCheckBox->setEnabled(!(inParam.Kind() & ParsedComment::accessMask));
    connect(mpCheckBox, SIGNAL(stateChanged(int)), this, SLOT(OnContentChange()));
    QBoxLayout* pLayout = new QHBoxLayout;
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->addWidget(mpCheckBox);
    pLayout->addStretch();
    QWidget* pWidget = new QWidget;
    pWidget->setLayout(pLayout);
    CentralWidget()->layout()->addWidget(pWidget);
}

void SingleEntryBoolean::WriteValuesTo(Param &outParam) const
{
    outParam.Value() = (mpCheckBox->isChecked() ? "1" : "0");
    DisplayBase::WriteValuesTo(outParam);
}

void SingleEntryBoolean::ReadValuesFrom(const Param &inParam)
{
    mpCheckBox->setChecked(::atoi(inParam.Value().c_str()));
    DisplayBase::ReadValuesFrom(inParam);
}
