////////////////////////////////////////////////////////////////////////////////
// $Id: LinearInterpolation.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Linearly interpolate between data points on a nonuniform grid.
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
#include "LinearInterpolation.h"
#include "Debugging.h"
#include "delaunator.hpp"
#include "Numeric.h"

namespace
{
struct Triangle
{
    size_t p1, p2, p3;
};
struct Index
{
    float *pData;
    int nx, ny;
    float &operator()(int x, int y)
    {
        return pData[x + nx * y];
    }
};
// Compute barycentric weights for a point wrt a triangle.
// Returns true if point is inside triangle.
struct TrianglePoints
{
    const GUI::Point &v1, &v2, &v3;
};
struct BarycentricWeights
{
    float w1, w2, w3;
};
bool barycentricTriangleTest(const GUI::Point &p, const TrianglePoints &t, BarycentricWeights &w)
{
    float den = (t.v2.y - t.v3.y) * (t.v1.x - t.v3.x) + (t.v3.x - t.v2.x) * (t.v1.y - t.v3.y), invden = 1.0 / den;
    w.w1 = ((t.v2.y - t.v3.y) * (p.x - t.v3.x) + (t.v3.x - t.v2.x) * (p.y - t.v3.y)) * invden;
    w.w2 = ((t.v3.y - t.v1.y) * (p.x - t.v3.x) + (t.v1.x - t.v3.x) * (p.y - t.v3.y)) * invden;
    w.w3 = 1 - w.w1 - w.w2;
    return w.w1 >= 0 && w.w2 >= 0 && w.w3 >= 0;
}
} // namespace

struct LinearInterpolation::Private
{
    std::vector<GUI::Point> mPoints;
    std::vector<Triangle> mTriangles;
    std::vector<float> mValues;
};

LinearInterpolation::LinearInterpolation() : p(new Private)
{
}

LinearInterpolation::~LinearInterpolation()
{
    delete p;
}

void LinearInterpolation::onSetPoints(const std::vector<GUI::Point> &points)
{
    p->mPoints = points;
    std::vector<double> coords;
    coords.reserve(2 * points.size());
    for (const auto &point : points)
    {
        coords.push_back(point.x);
        coords.push_back(point.y);
    }
    delaunator::Delaunator d(coords);
    Assert(d.triangles.size() % 3 == 0);
    p->mTriangles.clear();
    p->mTriangles.reserve(d.triangles.size() / 3);
    for (int i = 0; i < d.triangles.size(); i += 3)
    {
        Triangle t = {d.triangles[i], d.triangles[i + 1], d.triangles[i + 2]};
        p->mTriangles.push_back(t);
    }
}

void LinearInterpolation::onSetValues(const std::vector<float> &values)
{
    p->mValues = values;
}

void LinearInterpolation::onInterpolatePlane(float *pData, int nx, int ny) const
{
    for (int i = 0; i < nx * ny; ++i)
        pData[i] = NaN<float>();

    Index pixels = {pData, nx, ny};
    for (const auto &triangle : p->mTriangles)
    {
        TrianglePoints t = {p->mPoints[triangle.p1], p->mPoints[triangle.p2], p->mPoints[triangle.p3]};
        const float &v1 = p->mValues[triangle.p1], &v2 = p->mValues[triangle.p2], &v3 = p->mValues[triangle.p3];
        float xmax_ = std::max(t.v1.x, std::max(t.v2.x, t.v3.x)), xmin_ = std::min(t.v1.x, std::min(t.v2.x, t.v3.x)),
              ymax_ = std::max(t.v1.y, std::max(t.v2.y, t.v3.y)), ymin_ = std::min(t.v1.y, std::min(t.v2.y, t.v3.y));
        int xmax = std::min<int>(nx - 1, xmax_ * nx), xmin = std::max<int>(0, xmin_ * nx),
            ymax = std::min<int>(ny - 1, ymax_ * ny), ymin = std::max<int>(0, ymin_ * ny);
        for (int x = xmin; x <= xmax; ++x)
        {
            for (int y = ymin; y <= ymax; ++y)
            {
                float px = 1.0 * x / nx, py = 1.0 * y / ny;
                GUI::Point p = {px, py};
                BarycentricWeights w;
                if (barycentricTriangleTest(p, t, w))
                    pixels(x, y) = w.w1 * v1 + w.w2 * v2 + w.w3 * v3;
            }
        }
    }
}

float LinearInterpolation::onInterpolateAt(const GUI::Point &point) const
{
    for (const auto &triangle : p->mTriangles)
    {
        TrianglePoints t = {p->mPoints[triangle.p1], p->mPoints[triangle.p2], p->mPoints[triangle.p3]};
        BarycentricWeights w;
        if (barycentricTriangleTest(point, t, w))
        {
            const float &v1 = p->mValues[triangle.p1], &v2 = p->mValues[triangle.p2], &v3 = p->mValues[triangle.p3];
            return w.w1 * v1 + w.w2 * v2 + w.w3 * v3;
        }
    }
    return NaN<float>();
}
