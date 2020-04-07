/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the lottie-qt module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <trimpath_p.h>
#include <private/qpainterpath_p.h>
#include <private/qbezier_p.h>
#include <QtMath>

QT_BEGIN_NAMESPACE

/*
Returns the path trimmed to length fractions f1, f2, in range [0.0, 1.0].
f1 and f2 are displaced, with wrapping, by the fractional part of offset, effective range <-1.0, 1.0>
*/
QPainterPath TrimPath::trimmed(qreal f1, qreal f2, qreal offset) const
{
    QPainterPath res;
    if (mPath.isEmpty() || !mPath.elementAt(0).isMoveTo())
        return res;

    f1 = qBound(qreal(0.0), f1, qreal(1.0));
    f2 = qBound(qreal(0.0), f2, qreal(1.0));
    if (qFuzzyCompare(f1, f2))
        return res;
    if (f1 > f2)
        qSwap(f1, f2);
    if (qFuzzyCompare(f2 - f1, 1.0))  // Shortcut for no trimming
        return mPath;

    qreal dummy;
    offset = std::modf(offset, &dummy);    // Use only the fractional part of offset, range <-1, 1>

    qreal of1 = f1 + offset;
    qreal of2 = f2 + offset;
    if (offset < 0.0) {
        f1 = of1 < 0.0 ? of1 + 1.0 : of1;
        f2 = of2 + 1.0 > 1.0 ? of2 : of2 + 1.0;
    } else if (offset > 0.0) {
        f1 = of1 - 1.0 < 0.0 ? of1 : of1 - 1.0;
        f2 = of2 > 1.0 ? of2 - 1.0 : of2;
    }
    bool wrapping = (f1 > f2);
    //qDebug() << "ADJ:" << f1 << f2 << wrapping << "(" << of1 << of2 << ")";

    if (lensIsDirty())
        updateLens();
    qreal totLen = mLens.last();
    if (qFuzzyIsNull(totLen))
        return res;

    qreal l1 = f1 * totLen;
    qreal l2 = f2 * totLen;
    const int e1 = elementAtLength(l1);
    const bool mustTrimE1 = !qFuzzyCompare(mLens.at(e1), l1);
    const int e2 = elementAtLength(l2);
    const bool mustTrimE2 = !qFuzzyCompare(mLens.at(e2), l2);

    //qDebug() << "Trim [" << f1 << f2 << "] e1:" << e1 << mustTrimE1 << "e2:" << e2 << mustTrimE2 << "wrapping:" << wrapping;

    if (e1 == e2 && !wrapping && mustTrimE1 && mustTrimE2) {
        // Entire result is one element, clipped in both ends
        appendTrimmedElement(&res, e1, true, l1, true, l2);
    } else {
        // Partial start element, or just its end point
        if (mustTrimE1)
            appendEndOfElement(&res, e1, l1);
        else
            res.moveTo(endPointOfElement(e1));

        // Complete elements between start and end
        if (wrapping) {
            appendElementRange(&res, e1 + 1, mPath.elementCount() - 1);
            res.moveTo(mPath.elementAt(0));
            appendElementRange(&res, 1, (mustTrimE2 ? e2 - 1 : e2));
        } else {
            appendElementRange(&res, e1 + 1, (mustTrimE2 ? e2 - 1 : e2));
        }

        // Partial end element
        if (mustTrimE2)
            appendStartOfElement(&res, e2, l2);
    }
    return res;
}

void TrimPath::updateLens() const
{
    const int numElems = mPath.elementCount();
    mLens.resize(numElems);
    if (!numElems)
        return;

    QPointF runPt = mPath.elementAt(0);
    qreal runLen = 0.0;
    for (int i = 0; i < numElems; i++) {
        QPainterPath::Element e = mPath.elementAt(i);
        switch (e.type) {
        case QPainterPath::LineToElement:
            runLen += QLineF(runPt, e).length();
            runPt = e;
            break;
        case QPainterPath::CurveToElement: {
            Q_ASSERT(i < numElems - 2);
            QPainterPath::Element ee = mPath.elementAt(i + 2);
            runLen += QBezier::fromPoints(runPt, e, mPath.elementAt(i + 1), ee).length();
            runPt = ee;
            break;
        }
        case QPainterPath::MoveToElement:
            runPt = e;
            break;
        case QPainterPath::CurveToDataElement:
            break;
        }
        mLens[i] = runLen;
    }
}

int TrimPath::elementAtLength(qreal len) const
{
    const auto it = std::lower_bound(mLens.constBegin(), mLens.constEnd(), len);
    return (it == mLens.constEnd()) ? mLens.size() - 1 : int(it - mLens.constBegin());
}

QPointF TrimPath::endPointOfElement(int elemIdx) const
{
    QPainterPath::Element e = mPath.elementAt(elemIdx);
    if (e.isCurveTo())
        return mPath.elementAt(qMin(elemIdx + 2, mPath.elementCount() - 1));
    else
        return e;
}

void TrimPath::appendTrimmedElement(QPainterPath *to, int elemIdx, bool trimStart, qreal startLen, bool trimEnd, qreal endLen) const
{
    Q_ASSERT(elemIdx > 0);

    if (lensIsDirty())
        updateLens();

    qreal prevLen = mLens.at(elemIdx - 1);
    qreal elemLen = mLens.at(elemIdx) - prevLen;
    qreal len1 = startLen - prevLen;
    qreal len2 = endLen - prevLen;
    if (qFuzzyIsNull(elemLen))
        return;

    QPointF pp = mPath.elementAt(elemIdx - 1);
    QPainterPath::Element e = mPath.elementAt(elemIdx);
    if (e.isLineTo()) {
        QLineF l(pp, e);
        QPointF p1 = trimStart ? l.pointAt(len1 / elemLen) : pp;
        QPointF p2 = trimEnd ? l.pointAt(len2 / elemLen) : e;
        if (to->isEmpty())
            to->moveTo(p1);
        to->lineTo(p2);
    } else if (e.isCurveTo()) {
        Q_ASSERT(elemIdx < mPath.elementCount() - 2);

        QBezier b = QBezier::fromPoints(pp, e, mPath.elementAt(elemIdx + 1), mPath.elementAt(elemIdx + 2));
        qreal t1 = trimStart ? b.tAtLength(len1) : 0.0;  // or simply len1/elemLen to trim by t instead of len
        qreal t2 = trimEnd ? b.tAtLength(len2) : 1.0;
        QBezier c = b.getSubRange(t1, t2);
        if (to->isEmpty())
            to->moveTo(c.pt1());
        to->cubicTo(c.pt2(), c.pt3(), c.pt4());
    }
    else {
        Q_UNREACHABLE();
    }
}

void TrimPath::appendElementRange(QPainterPath *to, int first, int last) const
{
    //# (in QPPP, could do direct vector copy, better performance)
    if (first >= mPath.elementCount() || last >= mPath.elementCount())
        return;

    for (int i = first; i <= last; i++) {
        QPainterPath::Element e = mPath.elementAt(i);
        switch (e.type) {
        case QPainterPath::MoveToElement:
            to->moveTo(e);
            break;
        case QPainterPath::LineToElement:
            to->lineTo(e);
            break;
        case QPainterPath::CurveToElement:
            Q_ASSERT(i < mPath.elementCount() - 2);
            to->cubicTo(e, mPath.elementAt(i + 1), mPath.elementAt(i + 2));
            i += 2;
            break;
        default:
            // 'first' may point to CurveToData element, just skip it
            break;
        }
    }
}

QT_END_NAMESPACE
