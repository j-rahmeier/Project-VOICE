//////////////////////////////////////////////////////////////////////
// $Id: ConfigWindow.h 7850 2024-02-21 20:13:45Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The Operator module's parameter configuration dialog.
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
#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include "ParamDisplay.h"
#include <QDialog>
#include <map>
#include <set>
#include <string>

class Param;
class ParamList;
class Preferences;
class QScrollArea;

namespace Ui
{
class ConfigWindow;
}

class ConfigWindow : public QDialog
{
    Q_OBJECT

  public:
    ConfigWindow(QWidget *parent = 0);
    ~ConfigWindow();
    static ConfigWindow* Instance();

    int Initialize(ParamList *);
    void RenderParameter(Param *);

  private:
    void DeleteAllTabs();
    void RenderParameters(const std::string &section);
    void UpdateParameters();

  public slots:
    void Move(int x, int y) { QWidget::move(x, y); }
    void Resize(int w, int h) { QWidget::resize(w, h); }

  private slots:
    void OnSubsectionCollapsed(bool);
    void OnCfgTabControlChange();
    void OnClose();
    void OnSaveParametersClick();
    void OnLoadParametersClick();
    void OnConfigureSaveFilterClick();
    void OnConfigureLoadFilterClick();
    void OnHelpClick();

  protected:
    void changeEvent(QEvent *e);

  private:
    bool LoadParameters(const QString &inFileName);
    void DisposeWidgets();

  private:
    Ui::ConfigWindow *m_ui;

  private:
    ParamList *mpParameters;

    typedef std::map<std::string, ParamDisplay*> DisplayContainer;
    DisplayContainer mParamDisplays;
    std::string mCurTab;
    bool mUserSwitchedTabs;
    QString mOriginalTitle;
    QScrollArea *mpScrollArea;
    int mScrollPos;
};

extern ConfigWindow *gpConfig;

#endif // CONFIGWINDOW_H
