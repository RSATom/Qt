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

#ifndef BMANIMATION_H
#define BMANIMATION_H

#include <QQuickPaintedItem>
#include <QByteArray>
#include <QList>
#include <QImage>
#include <QThread>
#include <QMetaObject>

#include <QtBodymovin/private/bmconstants_p.h>

QT_BEGIN_NAMESPACE

class QQmlFile;

class BMBase;
class BMLayer;
class BatchRenderer;

class LottieAnimation : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(int frameRate READ frameRate WRITE setFrameRate RESET resetFrameRate NOTIFY frameRateChanged)
    Q_PROPERTY(int startFrame READ startFrame NOTIFY startFrameChanged)
    Q_PROPERTY(int endFrame READ endFrame NOTIFY endFrameChanged)
    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(Quality quality READ quality WRITE setQuality NOTIFY qualityChanged)
    Q_PROPERTY(bool autoPlay MEMBER m_autoPlay NOTIFY autoPlayChanged)
    Q_PROPERTY(int loops MEMBER m_loops NOTIFY loopsChanged)
    Q_PROPERTY(Direction direction READ direction WRITE setDirection NOTIFY directionChanged)

public:
    enum Status{Null, Loading, Ready, Error};
    Q_ENUM(Status)

    enum Quality{LowQuality, MediumQuality, HighQuality};
    Q_ENUM(Quality)

    enum Direction{Forward = 1, Reverse = -1};
    Q_ENUM(Direction)

    enum LoopCount{Infinite = -1};
    Q_ENUM(LoopCount)

    explicit LottieAnimation(QQuickItem *parent = nullptr);
    ~LottieAnimation() override;

    void paint(QPainter *painter) override;

    Status status() const;

    QUrl source() const;
    void setSource(const QUrl &source);

    int frameRate() const;
    void setFrameRate(int frameRate);
    void resetFrameRate();

    Quality quality() const;
    void setQuality(Quality quality);

    Direction direction() const;
    void setDirection(Direction direction);

    int startFrame() const;
    int endFrame() const;
    int currentFrame() const;

    Q_INVOKABLE void start();

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void togglePause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void gotoAndPlay(int frame);
    Q_INVOKABLE bool gotoAndPlay(const QString &frameMarker);
    Q_INVOKABLE void gotoAndStop(int frame);
    Q_INVOKABLE bool gotoAndStop(const QString &frameMarker);
    Q_INVOKABLE double getDuration(bool inFrames = false);

    QByteArray jsonSource() const;

signals:
    void statusChanged();
    void qualityChanged();
    void sourceChanged();
    void finished();
    void frameRateChanged();
    void autoPlayChanged();
    void loopsChanged();
    void directionChanged();
    void startFrameChanged();
    void endFrameChanged();

protected slots:
    void loadFinished();

    void renderNextFrame();

protected:
    void componentComplete() override;

    void setStatus(Status status);

    void setStartFrame(int startFrame);
    void setEndFrame(int endFrame);

    void load();

    virtual int parse(QByteArray jsonSource);

protected:
    BatchRenderer *m_frameRenderThread = nullptr;
    QMetaObject::Connection m_waitForFrameConn;

    Status m_status = Null;
    int m_startFrame = 0;
    int m_endFrame = 0;
    int m_currentFrame = 0;
    int m_frameRate = 30;
    int m_animFrameRate = 30;
    qreal m_animWidth = 0;
    qreal m_animHeight = 0;
    QHash<QString, int> m_markers;
    QUrl m_source;
    QScopedPointer<QQmlFile> m_file;
    QTimer *m_frameAdvance = nullptr;

    void gotoFrame(int frame);
    void reset();

private:
    Quality m_quality = MediumQuality;
    bool m_autoPlay = true;
    int m_loops = 1;
    int m_currentLoop = 0;
    int m_direction = Forward;
    QByteArray m_jsonSource;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(LottieAnimation*)

#endif // BMANIMATION_H
