/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QIOPIPE_P_H
#define QIOPIPE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qiodevice.h>
#include <QtCore/qbytearray.h>
#include <QtCore/private/qiodevice_p.h>

QT_BEGIN_NAMESPACE

class QObject;
class QIOPipePrivate;

class QIOPipe : public QIODevice
{
    Q_OBJECT

public:
    enum Mode {
        EndPipe = 0x0000,
        ProxyPipe = 0x0001
    };

     explicit QIOPipe(QIODevice *parent, Mode mode = EndPipe);
    ~QIOPipe() override;

    bool open(OpenMode openMode) override;
    bool isSequential() const override;
    void setReadChannelCount(int count);
    void addChildPipe(QIOPipe *childPipe);

protected:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

private:
    Q_DECLARE_PRIVATE(QIOPipe)
    Q_DISABLE_COPY(QIOPipe)
};

class QIOPipePrivate : public QIODevicePrivate
{
    Q_DECLARE_PUBLIC(QIOPipe)

public:
    explicit QIOPipePrivate(QIODevice *iodevice, bool proxying);
    ~QIOPipePrivate() override;

    void initialize();
    bool readAvailableData();
    void pumpData(const QByteArray &ba);
    void pushData(const QByteArray &ba);
    void _q_onReadyRead();
    void addChildPipe(QIOPipe *childPipe);
    void removeChildPipe(QIOPipe *childPipe);

    bool m_proxying = false;
    QPointer<QIODevice> source;
    QVector<QPointer<QIOPipe>> childPipes;
};

#endif // QIOPIPE_P_H

QT_END_NAMESPACE
