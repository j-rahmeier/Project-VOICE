//////////////////////////////////////////////////////////////////////
// $Id: PrefDialog.cpp 7207 2023-02-20 16:58:22Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The Operator module's preference dialog.
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
#include "PrefDialog.h"
#include "ui_PrefDialog.h"
#include "Preferences.h"

PrefDialog::PrefDialog(QWidget *parent) : QDialog(parent), m_ui(new Ui::PrefDialog)
{
    m_ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);
    this->setWindowTitle(Preferences::Instance()->mInstancePrefix + this->windowTitle());
    GetValues();
}

PrefDialog::~PrefDialog()
{
    delete m_ui;
}

void PrefDialog::GetValues()
{
    m_ui->slider_UserLevel->setValue(Preferences::Instance()->mUserLevel);

    m_ui->edit_CmdBtn1->setText(Preferences::Instance()->mButtons[0].Cmd);
    m_ui->edit_NameBtn1->setText(Preferences::Instance()->mButtons[0].Name);

    m_ui->edit_CmdBtn2->setText(Preferences::Instance()->mButtons[1].Cmd);
    m_ui->edit_NameBtn2->setText(Preferences::Instance()->mButtons[1].Name);

    m_ui->edit_CmdBtn3->setText(Preferences::Instance()->mButtons[2].Cmd);
    m_ui->edit_NameBtn3->setText(Preferences::Instance()->mButtons[2].Name);

    m_ui->edit_CmdBtn4->setText(Preferences::Instance()->mButtons[3].Cmd);
    m_ui->edit_NameBtn4->setText(Preferences::Instance()->mButtons[3].Name);

    m_ui->edit_OnConnect->setText(Preferences::Instance()->mScript[Preferences::AfterModulesConnected]);
    m_ui->edit_OnConnect->setReadOnly(Preferences::Instance()->mCmdlineSpecified[Preferences::AfterModulesConnected]);

    m_ui->edit_OnSetConfig->setText(Preferences::Instance()->mScript[Preferences::OnSetConfig]);
    m_ui->edit_OnSetConfig->setReadOnly(Preferences::Instance()->mCmdlineSpecified[Preferences::OnSetConfig]);

    m_ui->edit_OnStart->setText(Preferences::Instance()->mScript[Preferences::OnStart]);
    m_ui->edit_OnStart->setReadOnly(Preferences::Instance()->mCmdlineSpecified[Preferences::OnStart]);

    m_ui->edit_OnStartRun->setText(Preferences::Instance()->mScript[Preferences::OnStartRun]);
    m_ui->edit_OnStartRun->setReadOnly(Preferences::Instance()->mCmdlineSpecified[Preferences::OnStartRun]);

    m_ui->edit_OnSuspend->setText(Preferences::Instance()->mScript[Preferences::OnSuspend]);
    m_ui->edit_OnSuspend->setReadOnly(Preferences::Instance()->mCmdlineSpecified[Preferences::OnSuspend]);

    m_ui->edit_OnResume->setText(Preferences::Instance()->mScript[Preferences::OnResume]);
    m_ui->edit_OnResume->setReadOnly(Preferences::Instance()->mCmdlineSpecified[Preferences::OnResume]);

    m_ui->edit_OnExit->setText(Preferences::Instance()->mScript[Preferences::OnExit]);
    m_ui->edit_OnExit->setReadOnly(Preferences::Instance()->mCmdlineSpecified[Preferences::OnExit]);
}

void PrefDialog::PutValues() const
{
    Preferences::Instance()->mUserLevel = m_ui->slider_UserLevel->value();

    Preferences::Instance()->mButtons[0].Cmd = m_ui->edit_CmdBtn1->text();
    Preferences::Instance()->mButtons[0].Name = m_ui->edit_NameBtn1->text();

    Preferences::Instance()->mButtons[1].Cmd = m_ui->edit_CmdBtn2->text();
    Preferences::Instance()->mButtons[1].Name = m_ui->edit_NameBtn2->text();

    Preferences::Instance()->mButtons[2].Cmd = m_ui->edit_CmdBtn3->text();
    Preferences::Instance()->mButtons[2].Name = m_ui->edit_NameBtn3->text();

    Preferences::Instance()->mButtons[3].Cmd = m_ui->edit_CmdBtn4->text();
    Preferences::Instance()->mButtons[3].Name = m_ui->edit_NameBtn4->text();

    if (!Preferences::Instance()->mCmdlineSpecified[Preferences::AfterModulesConnected])
        Preferences::Instance()->mScript[Preferences::AfterModulesConnected] = m_ui->edit_OnConnect->text();

    if (!Preferences::Instance()->mCmdlineSpecified[Preferences::OnSetConfig])
        Preferences::Instance()->mScript[Preferences::OnSetConfig] = m_ui->edit_OnSetConfig->text();

    if (!Preferences::Instance()->mCmdlineSpecified[Preferences::OnStart])
        Preferences::Instance()->mScript[Preferences::OnStart] = m_ui->edit_OnStart->text();

    if (!Preferences::Instance()->mCmdlineSpecified[Preferences::OnSuspend])
        Preferences::Instance()->mScript[Preferences::OnSuspend] = m_ui->edit_OnSuspend->text();

    if (!Preferences::Instance()->mCmdlineSpecified[Preferences::OnResume])
        Preferences::Instance()->mScript[Preferences::OnResume] = m_ui->edit_OnResume->text();

    if (!Preferences::Instance()->mCmdlineSpecified[Preferences::OnStartRun])
        Preferences::Instance()->mScript[Preferences::OnStartRun] = m_ui->edit_OnStartRun->text();

    if (!Preferences::Instance()->mCmdlineSpecified[Preferences::OnExit])
        Preferences::Instance()->mScript[Preferences::OnExit] = m_ui->edit_OnExit->text();
}

void PrefDialog::on_okButton_clicked()
{
    PutValues();
    //  close();
    QDialog::accept();
}

void PrefDialog::on_cancelButton_clicked()
{
    // close();
    QDialog::reject();
}

void PrefDialog::on_slider_UserLevel_valueChanged(int value)
{
    switch (value)
    {
    case Preferences::Beginner:
        m_ui->label_UserLevel->setText("Beginner");
        break;
    case Preferences::Intermediate:
        m_ui->label_UserLevel->setText("Intermediate");
        break;
    case Preferences::Advanced:
        m_ui->label_UserLevel->setText("Advanced");
        break;
    default:;
    }
}
