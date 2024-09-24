////////////////////////////////////////////////////////////////////////////////
// $Id: LDAClassifier.cpp 7721 2023-11-21 16:49:50Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: An implementation of the LDA classifier training algorithm.
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
#include "LDAClassifier.h"
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

struct LDAClassifier::Private
{
    double mCount1, mCount2;
    Eigen::VectorXd mSum1, mSum2;
    Eigen::MatrixXd mSqSum1, mSqSum2;
};

LDAClassifier::LDAClassifier() : p(new Private)
{
    onClear();
}

LDAClassifier::~LDAClassifier()
{
    delete p;
}

const char *LDAClassifier::method() const
{
    return "LDA";
}

void LDAClassifier::onClear()
{
    p->mCount1 = 0;
    p->mCount2 = 0;
    p->mSum1 = Eigen::VectorXd();
    p->mSum2 = p->mSum1;
    p->mSqSum1 = Eigen::MatrixXd();
    p->mSqSum2 = p->mSqSum1;
}

void LDAClassifier::onInitialize(int, int)
{
}

void LDAClassifier::onObserve(double label, const std::vector<double> &data)
{
    if (p->mCount1 == 0 && p->mCount2 == 0)
    {
        p->mSum1 = Eigen::VectorXd::Zero(data.size());
        p->mSum2 = p->mSum1;
        p->mSqSum1 = Eigen::MatrixXd::Zero(data.size(), data.size());
        p->mSqSum2 = p->mSqSum1;
    }
    double &count = (label < 0) ? p->mCount1 : p->mCount2;
    ++count;
    Eigen::VectorXd &sum = (label < 0) ? p->mSum1 : p->mSum2;
    const Eigen::Map<const Eigen::VectorXd> v(data.data(), data.size());
    sum += v;
    Eigen::MatrixXd &sqSum = (label < 0) ? p->mSqSum1 : p->mSqSum2;
    sqSum += v * v.adjoint();
}

void LDAClassifier::onFinalize(std::vector<double> &weights)
{
    Eigen::VectorXd mean1 = p->mSum1 / p->mCount1, mean2 = p->mSum2 / p->mCount2;
    Eigen::MatrixXd cov1 = (p->mSqSum1 - (mean1 * p->mSum1.adjoint())) / p->mCount1;
    Eigen::MatrixXd cov2 = (p->mSqSum2 - (mean2 * p->mSum2.adjoint())) / p->mCount2;
    Eigen::MatrixXd cov = (cov1 * p->mCount1 + cov2 * p->mCount2) / (p->mCount1 + p->mCount2);
    auto qr = cov.colPivHouseholderQr();
    Eigen::VectorXd w = qr.solve(mean2 - mean1);
    weights = std::vector<double>(w.data(), w.data() + w.size());
}
