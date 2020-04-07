/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Quick Designer Components.
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

#include "qquickkeyframe_p.h"

#include "qquicktimeline_p.h"

#include <QtCore/qdebug.h>
#include <QtCore/QVariantAnimation>
#include <QtCore/qmath.h>
#include <QtGui/qpainter.h>
#include <QtQuick/private/qquickitem_p.h>
#include <QtQml/QQmlProperty>

#include <private/qvariantanimation_p.h>

#include <algorithm>

QT_BEGIN_NAMESPACE

class QQuickKeyframeGroupPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QQuickKeyframeGroup)
public:
    QQuickKeyframeGroupPrivate() = default;

    QObject *target = nullptr;
    QString propertyName;
    bool componentComplete = false;
    int userType = -1;

protected:
    void setupKeyframes();

    static void append_keyframe(QQmlListProperty<QQuickKeyframe> *list, QQuickKeyframe *a);
    static int keyframe_count(QQmlListProperty<QQuickKeyframe> *list);
    static QQuickKeyframe* keyframe_at(QQmlListProperty<QQuickKeyframe> *list, int pos);
    static void clear_keyframes(QQmlListProperty<QQuickKeyframe> *list);

    QList<QQuickKeyframe *> keyframes;
    QList<QQuickKeyframe *> sortedKeyframes;

    QVariant originalValue;
};

void QQuickKeyframeGroupPrivate::setupKeyframes()
{
    sortedKeyframes = keyframes;
    std::sort(sortedKeyframes.begin(), sortedKeyframes.end(), [](const QQuickKeyframe *first, const QQuickKeyframe *second) {
        return first->frame() < second->frame();
    });
}

void QQuickKeyframeGroupPrivate::append_keyframe(QQmlListProperty<QQuickKeyframe> *list, QQuickKeyframe *a)
{
    auto q = static_cast<QQuickKeyframeGroup *>(list->object);
    q->d_func()->keyframes.append(a);
    q->d_func()->setupKeyframes();
    q->reset();
}

int QQuickKeyframeGroupPrivate::keyframe_count(QQmlListProperty<QQuickKeyframe> *list)
{
    auto q = static_cast<QQuickKeyframeGroup *>(list->object);
    return q->d_func()->keyframes.count();
}

QQuickKeyframe* QQuickKeyframeGroupPrivate::keyframe_at(QQmlListProperty<QQuickKeyframe> *list, int pos)
{
    auto q = static_cast<QQuickKeyframeGroup *>(list->object);
    return q->d_func()->keyframes.at(pos);
}

void QQuickKeyframeGroupPrivate::clear_keyframes(QQmlListProperty<QQuickKeyframe> *list)
{
    auto q = static_cast<QQuickKeyframeGroup *>(list->object);
    while (q->d_func()->keyframes.count()) {
        QQuickKeyframe *firstKeyframe = q->d_func()->keyframes.at(0);
        q->d_func()->keyframes.removeAll(firstKeyframe);
    }
}

class QQuickKeyframePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QQuickKeyframe)
public:
    QQuickKeyframePrivate() = default;

    qreal frame = 0;
    QEasingCurve easingCurve;
    QVariant value;
};

/*!
    \qmltype Keyframe
    \inherits QObject
    \instantiates QQuickKeyframe
    \inqmlmodule QtQuick.Timeline
    \ingroup qtqmltypes

    \brief A keyframe.

    The value of a keyframe on a timeline.

    An easing curve can be attached to the keyframe.
*/

/*!
    \qmlproperty double Keyframe::frame

    The position of the keyframe on the timeline.
*/

/*!
    \qmlproperty var Keyframe::easing

    The easing curve attached to the keyframe.
*/

/*!
    \qmlproperty var Keyframe::value

    The value of the keyframe.
*/

/*!
    \qmlsignal Keyframe::easingCurveChanged

    This signal is emitted when the easing curve attached to the keyframe
    changes.
*/

QQuickKeyframe::QQuickKeyframe(QObject *parent)
    : QObject(*(new QQuickKeyframePrivate), parent)
{
}

qreal QQuickKeyframe::frame() const
{
    Q_D(const QQuickKeyframe);
    return d->frame;
}

void QQuickKeyframe::setFrame(qreal f)
{
    Q_D(QQuickKeyframe);
    if (d->frame == f)
        return;
    d->frame = f;

    reset();

    emit frameChanged();
}

void QQuickKeyframe::reset()
{
    auto keyframes = qobject_cast<QQuickKeyframeGroup*>(parent());
    if (keyframes)
        keyframes->reset();
}

QQuickKeyframe::QQuickKeyframe(QQuickKeyframePrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
}

/*!
    \qmltype KeyframeGroup
    \inherits QObject
    \instantiates QQuickKeyframeGroup
    \inqmlmodule QtQuick.Timeline
    \ingroup qtqmltypes

    \brief A keyframe group.

    A keyframe group contains all keyframes for a specific property of an item
    and always belongs to a timeline.
*/

/*!
    \qmlproperty var KeyframeGroup::target

    The item that is targeted by the keyframe group.
*/

/*!
    \qmlproperty string KeyframeGroup::property

    The property that is targeted by the keyframe group.
*/

/*!
    \qmlproperty list KeyframeGroup::keyframes
    \readonly

    A list of keyframes that belong to the keyframe group.
*/

QQuickKeyframeGroup::QQuickKeyframeGroup(QObject *parent)
    : QObject(*(new QQuickKeyframeGroupPrivate), parent)
{

}

QQmlListProperty<QQuickKeyframe> QQuickKeyframeGroup::keyframes()
{
    Q_D(QQuickKeyframeGroup);

    return { this, &d->keyframes, QQuickKeyframeGroupPrivate::append_keyframe,
                QQuickKeyframeGroupPrivate::keyframe_count,
                QQuickKeyframeGroupPrivate::keyframe_at,
                QQuickKeyframeGroupPrivate::clear_keyframes };
}

QObject *QQuickKeyframeGroup::target() const
{
    Q_D(const QQuickKeyframeGroup);
    return d->target;
}

void QQuickKeyframeGroup::setTargetObject(QObject *o)
{
    Q_D(QQuickKeyframeGroup);
    if (d->target == o)
        return;
    d->target = o;

    if (!property().isEmpty())
        init();

    emit targetChanged();
}

QString QQuickKeyframeGroup::property() const
{
    Q_D(const QQuickKeyframeGroup);
    return d->propertyName;
}

void QQuickKeyframeGroup::setProperty(const QString &n)
{
    Q_D(QQuickKeyframeGroup);
    if (d->propertyName == n)
        return;
    d->propertyName = n;

    if (target())
        init();

    emit propertyChanged();
}

QVariant QQuickKeyframeGroup::evaluate(qreal frame) const
{
    Q_D(const QQuickKeyframeGroup);

    if (d->sortedKeyframes.isEmpty())
        return QVariant();

    static QQuickKeyframe dummy;
    auto timeline = qobject_cast<QQuickTimeline*>(parent());
    if (timeline)
        dummy.setFrame(timeline->startFrame() - 0.0001);
    dummy.setValue(d->originalValue);

     QQuickKeyframe *lastFrame = &dummy;

    for (auto keyFrame :  qAsConst(d->sortedKeyframes)) {
        if (qFuzzyCompare(frame, keyFrame->frame()) || frame < keyFrame->frame())
            return keyFrame->evaluate(lastFrame, frame, d->userType);
        lastFrame = keyFrame;
    }

    return lastFrame->value();
}

void QQuickKeyframeGroup::setProperty(qreal frame)
{
    if (target()) {
        QQmlProperty qmlProperty(target(), property());

        if (!qmlProperty.write(evaluate(frame)))
            qWarning() << "Cannot set property" << property();
    }
}

void QQuickKeyframeGroup::init()
{
    Q_D(QQuickKeyframeGroup);
    if (target()) {
        d->originalValue = QQmlProperty::read(target(), property());
        d->userType = QQmlProperty(target(), property()).property().userType();
        if (property().contains(QLatin1Char('.'))) {
            if (d->userType == QMetaType::QVector2D
                    || d->userType == QMetaType::QVector3D
                    || d->userType == QMetaType::QVector4D
                    || d->userType == QMetaType::QQuaternion)
                d->userType = QMetaType::Double;
        }
    }
}

void QQuickKeyframeGroup::resetDefaultValue()
{
    Q_D(QQuickKeyframeGroup);
    QQmlProperty::write(target(), property(), d->originalValue);
}

void QQuickKeyframeGroup::reset()
{
    Q_D(QQuickKeyframeGroup);
    if (!d->componentComplete)
        return;

    auto *timeline = qobject_cast<QQuickTimeline*>(parent());
    if (timeline)
        timeline->reevaulate();
}

void QQuickKeyframeGroup::setupKeyframes()
{
    Q_D(QQuickKeyframeGroup);

    if (d->componentComplete)
        d->setupKeyframes();
}

void QQuickKeyframeGroup::classBegin()
{
    Q_D(QQuickKeyframeGroup);
    d->componentComplete = false;
}

void QQuickKeyframeGroup::componentComplete()
{
    Q_D(QQuickKeyframeGroup);
    d->componentComplete = true;
    setupKeyframes();
}

QEasingCurve QQuickKeyframe::easing() const
{
    Q_D(const QQuickKeyframe);
    return d->easingCurve;
}

void QQuickKeyframe::setEasing(const QEasingCurve &e)
{
    Q_D(QQuickKeyframe);
    if (d->easingCurve == e)
        return;

    d->easingCurve = e;

    reset();

    emit easingCurveChanged();
}

QVariant QQuickKeyframe::value() const
{
    Q_D(const QQuickKeyframe);
    return d->value;
}

void QQuickKeyframe::setValue(const QVariant &v)
{
    Q_D(QQuickKeyframe);
    if (d->value == v)
        return;
    d->value = v;

    reset();

    emit valueChanged();
}

QVariant QQuickKeyframe::evaluate(QQuickKeyframe *pre, qreal frametime, int userType) const
{
    QVariantAnimation::Interpolator interpolator = QVariantAnimationPrivate::getInterpolator(userType);
    if (!pre)
        return value();

    QVariant preValue = pre->value();
    qreal preFrame = pre->frame();

    qreal duration = frame() - preFrame;
    qreal offset = frametime - preFrame;

    qreal progress = easing().valueForProgress(offset / duration);

    preValue.convert(userType);
    QVariant convertedValue = value();
    convertedValue.convert(userType);

    if (!interpolator) {
        if (progress < 1.0)
            return preValue;

        return convertedValue;
    }

    if (preValue.isValid() && convertedValue.isValid())
        return interpolator(preValue.constData(), convertedValue.constData(), progress);

    qWarning() << "invalid keyframe target" << preValue << convertedValue << userType;

    return QVariant();
}

QT_END_NAMESPACE


