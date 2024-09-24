////////////////////////////////////////////////////////////////////////////////
// $Id: Classifier.h 7720 2023-11-20 18:50:11Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A base class for linear classifier training algorithms.
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
#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "GenericSignal.h"
#include "Param.h"
#include <vector>

class Classifier
{
  public:
    Classifier();
    virtual ~Classifier()
    {
    }
    Classifier(const Classifier &) = delete;
    Classifier &operator=(const Classifier &) = delete;

    virtual const char *method() const = 0;

    typedef void (*Callback)(int count, void *);
    struct CallbackDef
    {
        Callback callback;
        void *data;
    };
    void setTrainingCallback(Callback, void *);
    void setTrainingCallback(const CallbackDef &);
    const CallbackDef &trainingCallback() const;

    void setDownsamplingFactor(int);
    int downsamplingFactor() const;

    void setOutputDimension(int);
    int outputDimension() const;

    void clear();
    void train(const std::vector<GenericSignal> &data1, const std::vector<GenericSignal> &data2);
    double classify(const GenericSignal &) const;
    const std::vector<double> &weights() const;
    const GenericSignal &weightsAsSignal() const;
    double offset() const;
    double meanDiff() const;
    double variance() const;
    void normalizeVariance();

    void downsampleData(const GenericSignal&, std::vector<double>&) const;
    void upsampleData(const std::vector<double>&, GenericSignal&) const;

protected:
    virtual void onClear() = 0;
    virtual void onInitialize(int count, int size) = 0;
    virtual void onObserve(double label, const std::vector<double> &data) = 0;
    virtual void onFinalize(std::vector<double> &weights) = 0;

  private:
    void apply(const std::vector<GenericSignal> &data1, const std::vector<GenericSignal> &data2);

    CallbackDef mCallback;
    int mDownsamplingFactor, mOutputDimension;
    GenericSignal mWeightsAsSignal;
    std::vector<double> mWeights;
    double mOffset, mMeanDiff, mVariance;
};

#endif // CLASSIFIER_H
