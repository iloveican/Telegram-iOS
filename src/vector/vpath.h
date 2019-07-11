/* 
 * Copyright (c) 2018 Samsung Electronics Co., Ltd. All rights reserved.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef VPATH_H
#define VPATH_H
#include <vector>
#include "vcowptr.h"
#include "vmatrix.h"
#include "vpoint.h"
#include "vrect.h"

V_BEGIN_NAMESPACE

struct VPathData;
class VPath {
public:
    enum class Direction { CCW, CW };

    enum class Element : uchar { MoveTo, LineTo, CubicTo, Close };
    bool  empty() const;
    bool  null() const;
    void  moveTo(const VPointF &p);
    void  moveTo(float x, float y);
    void  lineTo(const VPointF &p);
    void  lineTo(float x, float y);
    void  cubicTo(const VPointF &c1, const VPointF &c2, const VPointF &e);
    void  cubicTo(float c1x, float c1y, float c2x, float c2y, float ex,
                  float ey);
    void  arcTo(const VRectF &rect, float startAngle, float sweepLength,
                bool forceMoveTo);
    void  close();
    void  reset();
    void  reserve(size_t pts, size_t elms);
    size_t segments() const;
    void  addCircle(float cx, float cy, float radius,
                    VPath::Direction dir = Direction::CW);
    void  addOval(const VRectF &rect, VPath::Direction dir = Direction::CW);
    void  addRoundRect(const VRectF &rect, float rx, float ry,
                       VPath::Direction dir = Direction::CW);
    void  addRoundRect(const VRectF &rect, float roundness,
                       VPath::Direction dir = Direction::CW);
    void  addRect(const VRectF &rect, VPath::Direction dir = Direction::CW);
    void  addPolystar(float points, float innerRadius, float outerRadius,
                      float innerRoundness, float outerRoundness,
                      float startAngle, float cx, float cy,
                      VPath::Direction dir = Direction::CW);
    void  addPolygon(float points, float radius, float roundness,
                     float startAngle, float cx, float cy,
                     VPath::Direction dir = Direction::CW);
    void addPath(const VPath &path);
    void  transform(const VMatrix &m);
    float length() const;
    const std::vector<VPath::Element> &elements() const;
    const std::vector<VPointF> &       points() const;
    void  clone(const VPath &srcPath);
    bool unique() const { return d.unique();}
    int refCount() const { return d.refCount();}

private:
    struct VPathData {
        bool  empty() const { return m_elements.empty(); }
        bool  null() const { return empty() && !m_elements.capacity();}
        void  moveTo(float x, float y);
        void  lineTo(float x, float y);
        void  cubicTo(float cx1, float cy1, float cx2, float cy2, float ex, float ey);
        void  close();
        void  reset();
        void  reserve(size_t, size_t);
        void  checkNewSegment();
        size_t segments() const;
        void  transform(const VMatrix &m);
        float length() const;
        void  addRoundRect(const VRectF &, float, float, VPath::Direction);
        void  addRoundRect(const VRectF &, float, VPath::Direction);
        void  addRect(const VRectF &, VPath::Direction);
        void  arcTo(const VRectF &, float, float, bool);
        void  addCircle(float, float, float, VPath::Direction);
        void  addOval(const VRectF &, VPath::Direction);
        void  addPolystar(float points, float innerRadius, float outerRadius,
                          float innerRoundness, float outerRoundness,
                          float startAngle, float cx, float cy,
                          VPath::Direction dir = Direction::CW);
        void  addPolygon(float points, float radius, float roundness,
                         float startAngle, float cx, float cy,
                         VPath::Direction dir = Direction::CW);
        void  addPath(const VPathData &path);
        void  clone(const VPath::VPathData &o) { *this = o;}
        const std::vector<VPath::Element> &elements() const
        {
            return m_elements;
        }
        const std::vector<VPointF> &points() const { return m_points; }
        std::vector<VPointF>        m_points;
        std::vector<VPath::Element> m_elements;
        size_t                      m_segments;
        VPointF                     mStartPoint;
        mutable float               mLength{0};
        mutable bool                mLengthDirty{true};
        bool                        mNewSegment;
    };

    vcow_ptr<VPathData> d;
};

inline bool VPath::empty() const
{
    return d->empty();
}

/*
 * path is empty as well as null(no memory for data allocated yet).
 */
inline bool VPath::null() const
{
    return d->null();
}

inline void VPath::moveTo(const VPointF &p)
{
    d.write().moveTo(p.x(), p.y());
}

inline void VPath::lineTo(const VPointF &p)
{
    d.write().lineTo(p.x(), p.y());
}

inline void VPath::close()
{
    d.write().close();
}

inline void VPath::reset()
{
    d.write().reset();
}

inline void VPath::reserve(size_t pts, size_t elms)
{
    d.write().reserve(pts, elms);
}

inline size_t VPath::segments() const
{
    return d->segments();
}

inline float VPath::length() const
{
    return d->length();
}

inline void VPath::cubicTo(const VPointF &c1, const VPointF &c2,
                           const VPointF &e)
{
    d.write().cubicTo(c1.x(), c1.y(), c2.x(), c2.y(), e.x(), e.y());
}

inline void VPath::lineTo(float x, float y)
{
    d.write().lineTo(x, y);
}

inline void VPath::moveTo(float x, float y)
{
    d.write().moveTo(x, y);
}

inline void VPath::cubicTo(float c1x, float c1y, float c2x, float c2y, float ex,
                           float ey)
{
    d.write().cubicTo(c1x, c1y, c2x, c2y, ex, ey);
}

inline void VPath::transform(const VMatrix &m)
{
    d.write().transform(m);
}

inline void VPath::arcTo(const VRectF &rect, float startAngle,
                         float sweepLength, bool forceMoveTo)
{
    d.write().arcTo(rect, startAngle, sweepLength, forceMoveTo);
}

inline void VPath::addRect(const VRectF &rect, VPath::Direction dir)
{
    d.write().addRect(rect, dir);
}

inline void VPath::addRoundRect(const VRectF &rect, float rx, float ry,
                                VPath::Direction dir)
{
    d.write().addRoundRect(rect, rx, ry, dir);
}

inline void VPath::addRoundRect(const VRectF &rect, float roundness,
                                VPath::Direction dir)
{
    d.write().addRoundRect(rect, roundness, dir);
}

inline void VPath::addCircle(float cx, float cy, float radius,
                             VPath::Direction dir)
{
    d.write().addCircle(cx, cy, radius, dir);
}

inline void VPath::addOval(const VRectF &rect, VPath::Direction dir)
{
    d.write().addOval(rect, dir);
}

inline void VPath::addPolystar(float points, float innerRadius,
                               float outerRadius, float innerRoundness,
                               float outerRoundness, float startAngle, float cx,
                               float cy, VPath::Direction dir)
{
    d.write().addPolystar(points, innerRadius, outerRadius, innerRoundness,
                          outerRoundness, startAngle, cx, cy, dir);
}

inline void VPath::addPolygon(float points, float radius, float roundness,
                              float startAngle, float cx, float cy,
                              VPath::Direction dir)
{
    d.write().addPolygon(points, radius, roundness, startAngle, cx, cy, dir);
}

inline void VPath::addPath(const VPath &path)
{
    if (path.empty()) return;

    if (null()) {
        *this = path;
    } else {
        d.write().addPath(path.d.read());
    }
}

inline const std::vector<VPath::Element> &VPath::elements() const
{
    return d->elements();
}

inline const std::vector<VPointF> &VPath::points() const
{
    return d->points();
}

inline void VPath::clone(const VPath &o)
{
   d.write().clone(o.d.read());
}

V_END_NAMESPACE

#endif  // VPATH_H
