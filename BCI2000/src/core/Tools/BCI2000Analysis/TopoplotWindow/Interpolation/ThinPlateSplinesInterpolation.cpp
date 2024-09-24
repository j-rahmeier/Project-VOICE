////////////////////////////////////////////////////////////////////////////////
// $Id: ThinPlateSplinesInterpolation.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Interpolate between data points on a nonuniform grid using
//   thin plate splines:
//   https://www.geometrictools.com/Documentation/ThinPlateSplines.pdf
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
#include "ThinPlateSplinesInterpolation.h"

#include "Debugging.h"
#include "Numeric.h"
#include <Eigen/Dense>
#include <fastlog.h>
#include <vector>

using Eigen::MatrixXf;
using Eigen::VectorXf;

namespace
{
const float eps = Eps(eps);
const float normconst = 1.0f / (2.f * 8.f * Pi<float>());

inline float sqr(float a)
{
    return a * a;
}

inline float Green(const GUI::Point &p1, const GUI::Point &p2)
{
    float r2 = sqr(p2.x - p1.x) + sqr(p2.y - p1.y);
    if (r2 < eps)
        return 0;
    return r2 * fastlog(r2) * normconst;
}
} // namespace

struct ThinPlateSplinesInterpolation::Private
{
    MatrixXf invM, N;
    VectorXf a, b;
    std::vector<GUI::Point> mPoints;

    inline float computeAt(const GUI::Point &point) const
    {
        float value = 0;
        for (int i = 0; i < a.size(); ++i)
            value += a(i) * Green(mPoints[i], point);
        value += b(0) + b(1) * point.x + b(2) * point.y;
        return value;
    }
};

ThinPlateSplinesInterpolation::ThinPlateSplinesInterpolation() : p(new Private)
{
}

ThinPlateSplinesInterpolation::~ThinPlateSplinesInterpolation()
{
    delete p;
}

void ThinPlateSplinesInterpolation::onSetPoints(const std::vector<GUI::Point> &points)
{
    if (points.size() < 3)
        throw std_runtime_error << "Thin Plate Splines Interpolation requires at least 3 points";

    p->mPoints = points;
    MatrixXf M(points.size(), points.size());
    for (int i = 0; i < M.rows(); ++i)
        for (int j = 0; j < M.cols(); ++j)
            M(i, j) = Green(points[i], points[j]);
    p->invM = M.inverse();

    p->N = MatrixXf(points.size(), 3);
    for (int i = 0; i < points.size(); ++i)
    {
        p->N(i, 0) = 1;
        p->N(i, 1) = points[i].x;
        p->N(i, 2) = points[i].y;
    }
}

void ThinPlateSplinesInterpolation::onSetValues(const std::vector<float> &values)
{
    MatrixXf m = p->N.adjoint() * p->invM * p->N;
    Eigen::Map<const VectorXf> y(values.data(), values.size());
    p->b = m.inverse() * p->N.adjoint() * p->invM * y;
    p->a = p->invM * (y - p->N * p->b);
}

void ThinPlateSplinesInterpolation::onInterpolatePlane(float *pData, int nx, int ny) const
{
    for (int x = 0; x < nx; ++x)
    {
        for (int y = 0; y < ny; ++y)
        {
            GUI::Point point = {1.0 * x / nx, 1.0 * y / ny};
            float value = p->computeAt(point);
            pData[x + nx * y] = value;
        }
    }
}

float ThinPlateSplinesInterpolation::onInterpolateAt(const GUI::Point &point) const
{
    return p->computeAt(point);
}
