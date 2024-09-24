////////////////////////////////////////////////////////////////////////////////
// $Id: AnalysisParamWidget.h 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A widget for analysis parameters.
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
#ifndef ANALYSIS_PARM_WIDGET_H
#define ANALYSIS_PARM_WIDGET_H
#include <QWidget>
#include <string>

class BCI2000FileReader;

class AnalysisParamWidget : public QWidget
{
    Q_OBJECT
  public:
    struct AnalysisParams
    {
        std::string condition1, condition2;
        static AnalysisParams createFromFile(const BCI2000FileReader *);
        void serialize(std::ostream &) const;
        static AnalysisParams unserialize(std::istream &);
    };

    AnalysisParamWidget(QWidget * = nullptr);
    ~AnalysisParamWidget();
    AnalysisParamWidget(AnalysisParamWidget &) = delete;
    AnalysisParamWidget &operator=(AnalysisParamWidget &) = delete;
    void setParams(const AnalysisParams &);
    AnalysisParams params() const;

  signals:
    void conditionsEdited();
    void editFilterParamsClicked();
    void resetFilterParamsClicked();

  private:
    struct Private;
    Private *p;
};

#endif // ANALYSIS_PARM_WIDGET_H
