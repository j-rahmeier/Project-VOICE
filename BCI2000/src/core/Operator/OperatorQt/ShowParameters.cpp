//////////////////////////////////////////////////////////////////////
// $Id: ShowParameters.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A dialog displaying parameters with check boxes.
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
#include "ShowParameters.h"
#include "ui_ShowParameters.h"

#include "OperatorUtils.h"
#include "ParamList.h"
#include <QtWidgets>

static const int cTextMargin = 2;
static const size_t cMaxTreeDepth = 3;

ShowParameters::ShowParameters(QWidget *parent, const ParamList &paramlist, int filtertype)
    : QDialog(parent), m_ui(new Ui::ShowParameters), mrParamList(paramlist), mFilterType(filtertype), mChanged(false)
{
    m_ui->setupUi(this);

    QDialogButtonBox *pButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    this->layout()->addWidget(pButtonBox);
    connect(pButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(pButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

    connect(this, SIGNAL(accepted()), this, SLOT(OnAccepted()));
    connect(this, SIGNAL(rejected()), this, SLOT(OnRejected()));
    connect(m_ui->treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(OnExpandCollapse()));
    connect(m_ui->treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(OnExpandCollapse()));
    connect(m_ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(OnItemChanged()));

    switch (mFilterType)
    {
    case OperatorUtils::loadFilter:
        this->setWindowTitle(tr("Load Filter"));
        m_ui->label->setText(tr("Select parameters for reading:"));
        break;
    case OperatorUtils::saveFilter:
        this->setWindowTitle(tr("Save Filter"));
        m_ui->label->setText(tr("Select parameters for writing:"));
        break;
    }
    OnShow();
}

ShowParameters::~ShowParameters()
{
    delete m_ui;
}

void ShowParameters::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ShowParameters::OnShow()
{
    m_ui->treeWidget->clear();
    for (int i = 0; i < mrParamList.Size(); ++i)
    {
        if (!mrParamList.ByIndex(i).Readonly() || (mFilterType != OperatorUtils::loadFilter))
        {
            const HierarchicalLabel &path = mrParamList.ByIndex(i).Sections();
            QTreeWidgetItem *pParent = m_ui->treeWidget->invisibleRootItem();
            for (size_t j = 0; j < std::min(cMaxTreeDepth - 1, path.size()); ++j)
            {
                int c = 0;
                while (c < pParent->childCount())
                {
                    if (pParent->child(c)->text(0) == path[j].c_str())
                        break;
                    else
                        ++c;
                }
                if (c < pParent->childCount())
                {
                    pParent = pParent->child(c);
                }
                else
                {
                    pParent = new QTreeWidgetItem(pParent, QStringList(path[j].c_str()));
                    pParent->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate | Qt::ItemIsEnabled);
                    pParent->setCheckState(0, Qt::PartiallyChecked);
                    AdjustSize(pParent, cTextMargin);
                }
            }
            const Param &param = mrParamList.ByIndex(i);
            const char *pName = param.Name().c_str();
            QTreeWidgetItem *pItem = new QTreeWidgetItem(pParent, QStringList(QString::fromLocal8Bit(pName)));
            pItem->setToolTip(0, QString::fromLocal8Bit(mrParamList.ByIndex(i).Comment().c_str()));
            pItem->setWhatsThis(0, pItem->toolTip(0));
            pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            std::string paramPath = param.Path();
            pItem->setData(0, Qt::UserRole, QString::fromLocal8Bit(paramPath.c_str()));
            pItem->setCheckState(0, !OperatorUtils::GetFilterStatus(paramPath.c_str(), mFilterType) ? Qt::Checked
                                                                                                    : Qt::Unchecked);
            AdjustSize(pItem, cTextMargin);
        }
    }
    m_ui->treeWidget->resizeColumnToContents(0);
    mChanged = false;
}

void ShowParameters::OnAccepted()
{
    for (QTreeWidgetItemIterator i(m_ui->treeWidget, QTreeWidgetItemIterator::NoChildren); *i != NULL; ++i)
    {
        QString path = (*i)->data(0, Qt::UserRole).toString();
        OperatorUtils::SetFilterStatus(path.toLocal8Bit(), mFilterType, (*i)->checkState(0) == Qt::Unchecked);
    }
}

void ShowParameters::OnRejected()
{
    if (mChanged)
    {
        int result =
            QMessageBox::question(this, tr("BCI2000 Operator Module"), tr("Discard changes to Parameter selection?"),
                                  QMessageBox::Save | QMessageBox::Discard, QMessageBox::Discard);
        if (result == QMessageBox::Save)
            OnAccepted();
    }
}

void ShowParameters::OnExpandCollapse()
{
    m_ui->treeWidget->resizeColumnToContents(0);
}

void ShowParameters::OnItemChanged()
{
    mChanged = true;
}

void ShowParameters::AdjustSize(QTreeWidgetItem *inpItem, int inMargin)
{
    for (int i = 0; i < inpItem->columnCount(); ++i)
    {
        QRect r = QFontMetrics(inpItem->font(i)).boundingRect(inpItem->text(i));
        r.adjust(-inMargin, -inMargin, inMargin, inMargin);
        inpItem->setSizeHint(i, r.size());
    }
}
