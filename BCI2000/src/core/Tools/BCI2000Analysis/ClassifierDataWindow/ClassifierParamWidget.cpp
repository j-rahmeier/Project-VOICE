////////////////////////////////////////////////////////////////////////////////
// $Id: ClassifierParamWidget.cpp 7726 2023-11-22 18:04:27Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A widget for classifier parameters.
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
#include "ClassifierParamWidget.h"
#include "AnalysisData.h"
#include <QBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QValidator>
#include <QCheckBox>

ClassifierParamWidget::ClassifierParams ClassifierParamWidget::ClassifierParams::createFromData(
    const AnalysisData &data)
{
    const auto &data1 = data.trainingData1, &data2 = data.trainingData2;
    ClassifierParams params;
    params.downsampling = 1;
    int N1 = data1.size(), N2 = data2.size();
    int dim = 0;
    if (!data1.empty())
        dim = data1.front().Channels() * data1.front().Elements();
    else if (!data2.empty())
        dim = data2.front().Channels() * data2.front().Elements();
    params.downsampling = 2 * dim / (N1 + N2 + 1);
    if (params.downsampling == 0)
        params.downsampling = 1;

    params.outputDimension = 1;
    params.normalize = true;

    params.penter = 0.1; // SWLDA parameters taken from Krusienski et al, 2006
    params.premove = 0.15;
    params.maxIterations = 60;
    return params;
}

struct ClassifierParamWidget::Private : QObject
{
    QLineEdit *mpDownsampling, *mpOutputDimension, *mpPenter, *mpPremove, *mpMaxIterations;
    QCheckBox *mpNormalize;
    Private(QObject *parent) : QObject(parent)
    {
    }
};

ClassifierParamWidget::ClassifierParamWidget(QWidget *parent) : QWidget(parent), p(new Private(this))
{
    auto pLayout = new QFormLayout;
    setLayout(pLayout);
    p->mpDownsampling = new QLineEdit(this);
    p->mpDownsampling->setMinimumWidth(10 * 5);
    p->mpDownsampling->setValidator(new QIntValidator(1, 9000, this));
    pLayout->addRow("Downsampling factor", p->mpDownsampling);
    p->mpOutputDimension = new QLineEdit(this);
    p->mpOutputDimension->setMinimumWidth(10*1);
    p->mpOutputDimension->setValidator(new QIntValidator(1, 3, this));
    pLayout->addRow("Output dimension", p->mpOutputDimension);
    p->mpNormalize = new QCheckBox("Normalize by within-class variance", this);
    pLayout->addRow(p->mpNormalize);
    auto pGroupBox = new QGroupBox("SWLDA Parameters", this);
    pLayout->addRow(pGroupBox);
    auto pLayout2 = new QFormLayout;
    p->mpPenter = new QLineEdit(this);
    p->mpPenter->setValidator(new QDoubleValidator(0, 1, 4, this));
    pLayout2->addRow("p_enter", p->mpPenter);
    p->mpPremove = new QLineEdit(this);
    p->mpPremove->setValidator(new QDoubleValidator(0, 1, 4, this));
    pLayout2->addRow("p_remove", p->mpPremove);
    p->mpMaxIterations = new QLineEdit(this);
    pLayout2->addRow("max iterations", p->mpMaxIterations);
    pGroupBox->setLayout(pLayout2);
    connect(p->mpDownsampling, &QLineEdit::textEdited, this, &ClassifierParamWidget::edited);
    connect(p->mpOutputDimension, &QLineEdit::textEdited, this, &ClassifierParamWidget::edited);
    connect(p->mpNormalize, &QCheckBox::toggled, this, &ClassifierParamWidget::edited);
    connect(p->mpPenter, &QLineEdit::textEdited, this, &ClassifierParamWidget::edited);
    connect(p->mpPremove, &QLineEdit::textEdited, this, &ClassifierParamWidget::edited);
    connect(p->mpMaxIterations, &QLineEdit::textEdited, this, &ClassifierParamWidget::edited);
}

ClassifierParamWidget::~ClassifierParamWidget()
{
}

void ClassifierParamWidget::setParams(const ClassifierParams &params)
{
    p->mpDownsampling->setText(QString::number(params.downsampling));
    p->mpOutputDimension->setText(QString::number(params.outputDimension));
    p->mpNormalize->setChecked(params.normalize);
    p->mpPenter->setText(QString::number(params.penter));
    p->mpPremove->setText(QString::number(params.premove));
    if (params.maxIterations == Inf<double>())
        p->mpMaxIterations->setText("inf");
    else
        p->mpMaxIterations->setText(QString::number(params.maxIterations));
}

ClassifierParamWidget::ClassifierParams ClassifierParamWidget::params() const
{
    ClassifierParams params;
    params.downsampling = p->mpDownsampling->text().toDouble();
    params.outputDimension = p->mpOutputDimension->text().toDouble();
    params.normalize = p->mpNormalize->isChecked();
    params.penter = p->mpPenter->text().toDouble();
    params.premove = p->mpPremove->text().toDouble();
    if (p->mpMaxIterations->text() == "inf")
        params.maxIterations = Inf<double>();
    else
        params.maxIterations = p->mpMaxIterations->text().toDouble();
    return params;
}

void ClassifierParamWidget::enableSWLDAParams(bool state)
{
    p->mpPenter->setEnabled(state);
    p->mpPremove->setEnabled(state);
    p->mpMaxIterations->setEnabled(state);
}
