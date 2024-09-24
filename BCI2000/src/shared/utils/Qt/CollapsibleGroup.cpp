////////////////////////////////////////////////////////////////////////////////
// $Id: CollapsibleGroup.cpp 7681 2023-10-27 14:19:26Z mellinger $
// Authors: mellinger@neurotechcenter.org
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
#include "CollapsibleGroup.h"
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QToolButton>
#include <QFrame>

struct CollapsibleGroup::ClickableLabel : QLabel
{
    QAbstractButton *mpButton;
    ClickableLabel(QString title, QWidget* parent) : QLabel(title, parent), mpButton(nullptr)
    {
    }
    void mousePressEvent(QMouseEvent *) override
    {
        if (mpButton)
            mpButton->toggle();
    }
};

CollapsibleGroup::CollapsibleGroup(const QString &title, QWidget* parent)
    : QWidget(parent), mpToolButton(nullptr), mpCheckBox(nullptr), mpLayout(new QGridLayout), 
      mpLabel(new ClickableLabel(title, this)),
      mpContent(new QFrame(this)), mpHeader(new QWidget(this))
{
    auto pMainLayout = new QVBoxLayout;
    pMainLayout->addWidget(mpHeader);
    pMainLayout->addWidget(mpContent);
    mpLayout->setVerticalSpacing(0);
    mpLayout->setHorizontalSpacing(0);
    mpLayout->setContentsMargins(0, 0, 0, 0);
    mpLayout->addWidget(mpLabel, 0, 1, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    mpLayout->addWidget(mpContent, 1, 1, 1, 1, Qt::AlignTop);
    setLayout(mpLayout);
    setFlat(false);
    setUsingCheckBox(false);
}

void CollapsibleGroup::setContentLayout(QLayout *pLayout)
{
    delete mpContent->layout();
    mpContent->setLayout(pLayout);
}

QLayout *CollapsibleGroup::contentLayout() const
{
    return mpContent->layout();
}

void CollapsibleGroup::setTitle(const QString& title)
{
    mpLabel->setText(title);
}

QString CollapsibleGroup::title() const
{
    return mpLabel->text();
}

void CollapsibleGroup::setTitleFont(const QFont& font)
{
    mpLabel->setFont(font);
}

const QFont& CollapsibleGroup::titleFont() const
{
    return mpLabel->font();
}

void CollapsibleGroup::setFlat(bool flat)
{
    if (flat)
        mpContent->setFrameShape(QFrame::NoFrame);
    else
        mpContent->setFrameShape(QFrame::StyledPanel);
}

bool CollapsibleGroup::isFlat() const
{
    return mpContent->frameShape() == QFrame::NoFrame;
}

bool CollapsibleGroup::isCollapsed() const
{
    if (mpCheckBox)
        return !mpCheckBox->isChecked();
    if (mpToolButton)
        return !mpToolButton->isChecked();
    return true;
}

void CollapsibleGroup::setCollapsed(bool b)
{
    if (mpCheckBox)
        mpCheckBox->setChecked(!b);
    if (mpToolButton)
        mpToolButton->setChecked(!b);
}

void CollapsibleGroup::onCollapsedToggled()
{
    if (mpToolButton)
        mpToolButton->setArrowType(isCollapsed() ? Qt::ArrowType::RightArrow : Qt::ArrowType::DownArrow);
    if (mpCheckBox)
        mpCheckBox->setChecked(!isCollapsed());
    mpContent->setVisible(!isCollapsed());
    emit toggled(isCollapsed());
}

void CollapsibleGroup::setUsingCheckBox(bool b)
{
    bool collapsed = isCollapsed();
    if (b && !mpCheckBox)
    {
        delete mpToolButton;
        mpToolButton = nullptr;
        mpCheckBox = new QCheckBox(this);
        mpCheckBox->setChecked(!collapsed);
        mpCheckBox->setFixedHeight(mpCheckBox->fontMetrics().lineSpacing());
        mpCheckBox->setFixedWidth(3 * mpCheckBox->minimumSizeHint().width() / 2);
        mpLabel->mpButton = mpCheckBox;
        mpLayout->addWidget(mpCheckBox, 0, 0, 1, 1, Qt::AlignTop);
        connect(mpCheckBox, &QCheckBox::toggled, this, &CollapsibleGroup::onCollapsedToggled);
        onCollapsedToggled();
    }
    else if (!b && !mpToolButton)
    {
        delete mpCheckBox;
        mpCheckBox = nullptr;
        mpToolButton = new QToolButton(this);
        mpToolButton->setStyleSheet("QToolButton { border: none; }");
        mpToolButton->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
        mpToolButton->setArrowType(Qt::ArrowType::DownArrow);
        mpToolButton->setCheckable(true);
        mpToolButton->setChecked(!collapsed);
        mpToolButton->setFixedHeight(mpToolButton->fontMetrics().lineSpacing());
        mpLabel->mpButton = mpToolButton;
        mpLayout->addWidget(mpToolButton, 0, 0, 1, 1, Qt::AlignTop);
        connect(mpToolButton, &QToolButton::toggled, this, &CollapsibleGroup::onCollapsedToggled);
        onCollapsedToggled();
    }
}

bool CollapsibleGroup::isUsingCheckBox() const
{
    return mpCheckBox != nullptr;
}
