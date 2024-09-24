////////////////////////////////////////////////////////////////////////////////
// $Id: SWLDAClassifier.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: An implementation of the SWLDA classifier training algorithm.
//   (Matlab's stepwisefit() function translated into C++ by Cristhian Potes).
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
#include "SWLDAClassifier.h"
#include "Exception.h"
#include <stepwisefit.h>

struct SWLDAClassifier::Private
{
    double mPenter, mPremove;
    int mMaxIterations;

    size_t mRow;
    size_t mCount, mDimensions;
    ap::real_2d_array mX;
    ap::real_1d_array my;
};

SWLDAClassifier::SWLDAClassifier() : p(new Private)
{
    onClear();
    p->mMaxIterations = Inf<double>();
    p->mPenter = 0.05;
    p->mPremove = 0.10;
}

SWLDAClassifier::~SWLDAClassifier()
{
    delete p;
}

const char *SWLDAClassifier::method() const
{
    return "SWLDA";
}

void SWLDAClassifier::setPenter(double d)
{
    p->mPenter = d;
}

double SWLDAClassifier::penter() const
{
    return p->mPenter;
}

void SWLDAClassifier::setPremove(double d)
{
    p->mPremove = d;
}

double SWLDAClassifier::premove() const
{
    return p->mPremove;
}

void SWLDAClassifier::setMaxIterations(double d)
{
    p->mMaxIterations = d;
}

double SWLDAClassifier::maxIterations() const
{
    return p->mMaxIterations;
}

void SWLDAClassifier::onClear()
{
    p->mRow = 0;
    p->mCount = 0;
    p->mDimensions = 0;
    p->mX = ap::real_2d_array();
    p->my = ap::real_1d_array();
}

void SWLDAClassifier::onInitialize(int count, int dimensions)
{
    if (count < dimensions)
        throw std_runtime_error << "SWLDA: Number of features (" << dimensions
                                << ") exceeds number of training examples (" << count << ")";

    p->mRow = 0;
    p->mCount = count;
    p->mDimensions = dimensions;
    p->mX.setlength(count, dimensions);
    p->my.setlength(count);
}

void SWLDAClassifier::onObserve(double label, const std::vector<double> &data)
{
    p->my(p->mRow) = label;
    ::memcpy(&p->mX(p->mRow, 0), data.data(), data.size() * sizeof(double));
    ++p->mRow;
}

void SWLDAClassifier::onFinalize(std::vector<double> &weights)
{
    ap::real_1d_array B, SE, PVAL;
    B.setlength(p->mDimensions);
    SE.setlength(p->mDimensions);
    PVAL.setlength(p->mDimensions);
    ap::boolean_1d_array in;
    in.setlength(p->mDimensions);
    for (int i = 0; i < p->mDimensions; ++i)
        in(i) = false;
    try
    {
        stepwisefit(p->mX, p->my, p->mPenter, p->mPremove, p->mMaxIterations, B, SE, PVAL, in);
    }
    catch (const ap::ap_error & err)
    {
        throw std_runtime_error << "Internal SWLDA error: " << err.msg;
    }
    weights = std::vector<double>(B.getcontent(), B.getcontent() + p->mDimensions);
    double absmax = 0;
    for (int i = 0; i < p->mDimensions; ++i)
    {
        if (!in(i))
            weights[i] = 0;
        else
            absmax = std::max(::fabs(weights[i]), absmax);
    }
    for (auto& w : weights)
        w *= 10.0 / absmax; // P300Classifier scaling
}
