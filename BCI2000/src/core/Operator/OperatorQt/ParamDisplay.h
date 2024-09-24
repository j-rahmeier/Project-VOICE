//////////////////////////////////////////////////////////////////////
// $Id: ParamDisplay.h 8014 2024-04-09 19:28:15Z mellinger $
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
#ifndef PARAM_DISPLAY_H
#define PARAM_DISPLAY_H

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QSlider>
#include <QWidget>

#include <map>
#include <set>
#include <string>

#include "Param.h"
class ParsedComment;
class DisplayBase;

class ParamDisplay // This class is the interface to the outside world.
{
  public:
    ParamDisplay();
    ParamDisplay(const Param &);
    ParamDisplay(const ParamDisplay &) = delete;
    ~ParamDisplay();
    const ParamDisplay &operator=(const ParamDisplay &) = delete;

    QWidget* GetWidget();

    void WriteValuesTo(Param &) const;
    void ReadValuesFrom(const Param &);
    bool Modified() const;

  private:
    DisplayBase *mpDisplay;
};

// The ParamLabel class exists to allow for custom
// "WhatsThis" behavior by overriding QLabel::event().
class ParamLabel : public QLabel
{
    Q_OBJECT

  public:
    ParamLabel(const ParsedComment &);
    virtual ~ParamLabel()
    {
    }

  protected:
    virtual bool event(QEvent *);

  private:
    std::string mParamName, mHelpContext;
};

// The class hierarchy descending from DisplayBase
// contains implementations for various
// kinds of parameters.
class DisplayBase : public QWidget
{
    Q_OBJECT

  protected:
    DisplayBase(const ParsedComment &);

  public:
    virtual ~DisplayBase();

    virtual void WriteValuesTo(Param &) const;
    virtual void ReadValuesFrom(const Param &);
    bool Modified() const
    {
        return mModified;
    }

  protected:
    QWidget* CentralWidget()
    {
        return mpCentralWidget;
    }
    void SetCentralWidget(QWidget* pWidget)
    {
        mpCentralWidget = pWidget;
    }

    enum {
        ParamName,
        ParamValue,
        UserLevel,
        Comment,
        EllipsisButton,
        Margin,
    };
    int Measure(int);

  public slots:
    void OnContentChange()
    {
        mModified = true;
    }

  private:
    bool mModified;
    QWidget *mpCentralWidget;
    QSlider *mpUserLevel;
};

// This is the base class for all displays where there is a separate label
// holding the "comment" part of the parameter.
class SeparateComment : public DisplayBase
{
    Q_OBJECT
  protected:
    SeparateComment(const ParsedComment &);
};

// This is the base class for all displays that contain an edit field
// holding parameter values.
class SingleEntryEdit : public SeparateComment
{
    Q_OBJECT
  public:
    SingleEntryEdit(const ParsedComment &);
    virtual void WriteValuesTo(Param &) const;
    virtual void ReadValuesFrom(const Param &);
  private slots:
    void OnEditChange();

  protected:
    QLineEdit *mpEdit;
};

// This is the base class for all displays that contain a button beside
// the edit field.
class SingleEntryButton : public SingleEntryEdit
{
    Q_OBJECT
  public:
    SingleEntryButton(const ParsedComment &);
  private slots:
    void OnButtonClick();

  private:
    virtual void ButtonClick() = 0;

  protected:
    std::string mComment;
};

class SingleEntryInputFile : public SingleEntryButton
{
    Q_OBJECT
  public:
    SingleEntryInputFile(const ParsedComment &);

  private:
    void ButtonClick() override;
};

class SingleEntryOutputFile : public SingleEntryButton
{
    Q_OBJECT
  public:
    SingleEntryOutputFile(const ParsedComment &);

  private:
    void ButtonClick() override;
};

class SingleEntryDirectory : public SingleEntryButton
{
    Q_OBJECT
  public:
    SingleEntryDirectory(const ParsedComment &);

  private:
    void ButtonClick() override;
};

class SingleEntryColor : public SingleEntryButton
{
    Q_OBJECT
  public:
    SingleEntryColor(const ParsedComment &);

  private:
    void ButtonClick() override;
};

class SingleEntryBlob : public SeparateComment
{
    Q_OBJECT
  public:
    SingleEntryBlob(const ParsedComment &);
    ~SingleEntryBlob();
    void WriteValuesTo(Param &) const;
    void ReadValuesFrom(const Param &);
  private slots:
    void OnViewButtonClick();
    void OnLoadButtonClick();
    void OnSaveButtonClick();

  private:
    void ReadData();
    QWidget *mpViewButton, *mpSaveButton, *mpLoadButton;
    std::string mName;
    class Blob *mpData;
    const Param *mpParam;
    int mKind;
};

class List : public SingleEntryButton
{
    Q_OBJECT
  public:
    List(const ParsedComment &);
    void WriteValuesTo(Param &) const override;
    void ReadValuesFrom(const Param &) override;

  private:
    void ButtonClick() override;
    void ParamToText(const Param &);
    void TextToParam(Param &) const;
    std::string mName;
    int mKind;
};

class Matrix : public SeparateComment
{
    Q_OBJECT
  public:
    Matrix(const ParsedComment &);
    void WriteValuesTo(Param &) const override;
    void ReadValuesFrom(const Param &) override;
  private slots:
    void OnEditButtonClick();
    void OnLoadButtonClick();
    void OnSaveButtonClick();

  private:
    bool mMatrixWindowOpen;
    Param mParam;
    int mKind;
};

class SingleEntryEnum : public SeparateComment
{
    Q_OBJECT
  public:
    SingleEntryEnum(const ParsedComment &);
    void WriteValuesTo(Param &) const override;
    void ReadValuesFrom(const Param &) override;

  private:
    QComboBox *mpComboBox;
    int mIndexBase;
};

class SingleEntryBoolean : public DisplayBase
{
    Q_OBJECT
  public:
    SingleEntryBoolean(const ParsedComment &);
    void WriteValuesTo(Param &) const override;
    void ReadValuesFrom(const Param &) override;

  private:
    QCheckBox *mpCheckBox;
};

#endif // PARAM_DISPLAY_H
