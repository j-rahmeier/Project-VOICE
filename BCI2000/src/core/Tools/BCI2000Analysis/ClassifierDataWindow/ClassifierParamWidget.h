////////////////////////////////////////////////////////////////////////////////
// $Id: ClassifierParamWidget.h 7726 2023-11-22 18:04:27Z mellinger $
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
#ifndef CLASSIFIER_PARAM_WIDGET_H
#define CLASSIFIER_PARAM_WIDGET_H
#include <QWidget>

struct AnalysisData;

class ClassifierParamWidget : public QWidget
{
    Q_OBJECT
  public:
    struct ClassifierParams
    {
        int downsampling, outputDimension;
        bool normalize;
        double penter, premove, maxIterations;
        static ClassifierParams createFromData(const AnalysisData &);
    };

    ClassifierParamWidget(QWidget * = nullptr);
    ~ClassifierParamWidget();
    ClassifierParamWidget(ClassifierParamWidget &) = delete;
    ClassifierParamWidget &operator=(ClassifierParamWidget &) = delete;
    void setParams(const ClassifierParams &);
    ClassifierParams params() const;

  signals:
    void edited();

  public slots:
    void enableSWLDAParams(bool);

  private:
    struct Private;
    Private *p;
};

#endif // CLASSIFIER_PARAM_WIDGET_H
