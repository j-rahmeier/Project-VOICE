////////////////////////////////////////////////////////////////////////////////
// $Id: CollapsibleGroup.h 7681 2023-10-27 14:19:26Z mellinger $
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
#ifndef COLLAPSIBLE_GROUP_H
#define COLLAPSIBLE_GROUP_H

#include <QWidget>

class QToolButton;
class QCheckBox;
class QGridLayout;
class QFrame;

class CollapsibleGroup : public QWidget
{
    Q_OBJECT

  public:
    CollapsibleGroup(const QString &title, QWidget* parent);
    void setContentLayout(QLayout *pLayout);
    QLayout *contentLayout() const;
    void setTitle(const QString&);
    QString title() const;
    void setTitleFont(const QFont&);
    const QFont& titleFont() const;
    void setFlat(bool);
    bool isFlat() const;
    bool isCollapsed() const;
    void setCollapsed(bool);
    void setUsingCheckBox(bool);
    bool isUsingCheckBox() const;

  signals:
    void toggled(bool);

  private:
    void onCollapsedToggled();
    using QWidget::layout;
    using QWidget::setLayout;

    QToolButton* mpToolButton;
    QCheckBox* mpCheckBox;
    QGridLayout* mpLayout;
    QWidget* mpHeader;
    QFrame* mpContent;
    struct ClickableLabel;
    ClickableLabel* mpLabel;
};

#endif // COLLAPSIBLE_GROUP_H
