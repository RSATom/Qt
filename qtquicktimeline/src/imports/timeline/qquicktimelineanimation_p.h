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

#ifndef QQUICKTIMELINEANIMATION_H
#define QQUICKTIMELINEANIMATION_H

#include <QtQuick/private/qquickanimation_p.h>

QT_BEGIN_NAMESPACE

class QQuickTimelineAnimation : public QQuickNumberAnimation
{
    Q_OBJECT

    Q_PROPERTY(bool pingPong READ pingPong WRITE setPingPong NOTIFY pingPongChanged)

public:
    explicit QQuickTimelineAnimation(QObject *parent = nullptr);

    void setPingPong(bool b);
    bool pingPong() const;

Q_SIGNALS:
    void pingPongChanged();
    void finished();

private:
    void handleStarted();
    void handleStopped();

    bool m_pinpong = false;
    bool m_reversed = false;
    bool m_originalStart = true;
    int m_currentLoop = 0;
    int m_originalLoop = 0;
};

QT_END_NAMESPACE

#endif // QQUICKTIMELINEANIMATION_H
