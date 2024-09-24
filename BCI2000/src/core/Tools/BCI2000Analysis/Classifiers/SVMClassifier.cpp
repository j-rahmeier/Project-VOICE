////////////////////////////////////////////////////////////////////////////////
// $Id: SVMClassifier.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A wrapper to the libsvm library.
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
#include "SVMClassifier.h"
#include "Exception.h"
#include "svm.h"

namespace
{

double computeScalarProduct(const svm_node *n1, const svm_node *n2)
{
    // This will only work for feature vectors that contain all dimensions in growing order.
    double r = 0;
    while (n1->index >= 0 && n2->index >= 0)
    {
        Assert(n1->index == n2->index);
        r += n1->value * n2->value;
        ++n1;
        ++n2;
    }
    return r;
}

void findHyperparams(const svm_problem *problem, svm_parameter *params)
{
    // Chapelle and Zien, 2005 suggest to base initial selection of C on empirical variance in feature space.
    double sSq = 0;
    for (int i = 0; i < problem->l; ++i)
    {
        sSq += computeScalarProduct(problem->x[i], problem->x[i]) / problem->l;
        for (int j = i + 1; j < problem->l; ++j)
            sSq -= 2 * computeScalarProduct(problem->x[i], problem->x[j]) / problem->l / problem->l;
    }
    std::vector<std::pair<int, svm_parameter>> results;
    for (auto k : {-3, -2, -1, 0, 1, 2, 3})
    {
        params->C = ::pow(2, k) / sSq;
        std::vector<double> target(problem->l);
        svm_cross_validation(problem, params, 10, target.data());
        int correct = 0;
        for (int i = 0; i < problem->l; ++i)
        {
            if (target[i] == problem->y[i])
                ++correct;
        }
        results.push_back(std::make_pair(correct, *params));
    }
    size_t maxIdx = 0;
    for (size_t i = 1; i < results.size(); ++i)
        if (results[i].first > results[maxIdx].first)
            maxIdx = i;
    *params = results[maxIdx].second;
}

} // namespace

struct SVMClassifier::Private
{
    int mCount, mDimensions;
    std::vector<double> mLabels;
    std::vector<svm_node> mVectors;
};

SVMClassifier::SVMClassifier() : p(new Private)
{
    onClear();
}

SVMClassifier::~SVMClassifier()
{
    delete p;
}

const char *SVMClassifier::method() const
{
    return "SVM";
}

void SVMClassifier::onClear()
{
    p->mLabels.clear();
    p->mVectors.clear();
}

void SVMClassifier::onInitialize(int count, int dimensions)
{
    p->mCount = count;
    p->mDimensions = dimensions;
    p->mLabels.clear();
    p->mLabels.reserve(count);
    p->mVectors.clear();
    p->mVectors.reserve(count * (dimensions + 1));
}

void SVMClassifier::onObserve(double label, const std::vector<double> &data)
{
    p->mLabels.push_back(label);
    for (int i = 0; i < data.size(); ++i)
    {
        svm_node node = {i + 1, data[i]};
        p->mVectors.push_back(node);
    }
    svm_node cork = {-1, 0};
    p->mVectors.push_back(cork);
}

void SVMClassifier::onFinalize(std::vector<double> &weights)
{
    std::vector<svm_node *> x(p->mCount);
    for (int i = 0; i < p->mCount; ++i)
        x[i] = &p->mVectors[i * (p->mDimensions + 1)];
    svm_problem problem = {p->mCount, p->mLabels.data(), x.data()};
    svm_parameter params = {0};
    params.svm_type = C_SVC;
    params.kernel_type = LINEAR;
    params.cache_size = std::max(1.0, 10.0 * p->mCount * p->mCount * sizeof(double) / 1024 / 1024);
    params.eps = 0.001;
    params.C = 1;
    findHyperparams(&problem, &params);
    svm_model *pModel = svm_train(&problem, &params);
    weights.clear();
    weights.resize(p->mDimensions, 0);
    for (int i = 0; i < pModel->l; ++i)
    {
        svm_node *pNode = pModel->SV[i];
        while (pNode->index > 0)
        {
            weights[pNode->index - 1] += pNode->value * pModel->sv_coef[0][i];
            ++pNode;
        }
    }
    if (pModel->label[0] == -1)
    {
        for (auto &w : weights)
            w = -w;
    }
}
