/****************************************************************************
**
** Copyright (C) 2017 Ford Motor Company
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtRemoteObjects module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
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
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QString>
#include <QtTest>
#include <qqmlengine.h>
#include <qqmlcomponent.h>
#include "rep_usertypes_merged.h"

class TypeWithReply : public TypeWithReplySimpleSource
{
public:
    QString uppercase(const QString & input) override {
        return input.toUpper();
    }
    QMap<QString, QString> complexReturnType() override {
        return QMap<QString, QString>{{"one","1"}};
    }
    int slowFunction() override {
        QTest::qWait(1000);
        return 15;
    }
};

class tst_usertypes : public QObject
{
    Q_OBJECT

public:
    tst_usertypes();

private Q_SLOTS:
    void extraPropertyInQml();
    void extraPropertyInQml2();
    void extraPropertyInQmlComplex();
    void modelInQml();
    void subObjectInQml();
    void complexInQml_data();
    void complexInQml();
    void watcherInQml();
    void hostInQml();
    void twoReplicas();
};

tst_usertypes::tst_usertypes()
{
    qmlRegisterType<ComplexTypeReplica>("usertypes", 1, 0, "ComplexTypeReplica");
}

void tst_usertypes::extraPropertyInQml()
{
    qmlRegisterType<SimpleClockReplica>("usertypes", 1, 0, "SimpleClockReplica");

    QRemoteObjectRegistryHost host(QUrl("local:test"));
    SimpleClockSimpleSource clock;
    host.enableRemoting(&clock);

    QQmlEngine e;
    QQmlComponent c(&e, SRCDIR "data/extraprop.qml");
    QObject *obj = c.create();
    QVERIFY(obj);

    QTRY_COMPARE_WITH_TIMEOUT(obj->property("result").value<int>(), 10, 300);
}

void tst_usertypes::extraPropertyInQml2()
{
    qmlRegisterType<SimpleClockReplica>("usertypes", 1, 0, "SimpleClockReplica");

    QRemoteObjectRegistryHost host(QUrl("local:test2"));
    SimpleClockSimpleSource clock;
    clock.setHour(10);
    host.enableRemoting(&clock);

    QQmlEngine e;
    QQmlComponent c(&e, SRCDIR "data/extraprop2.qml");
    QObject *obj = c.create();
    QVERIFY(obj);

    QTRY_COMPARE_WITH_TIMEOUT(obj->property("hour").value<int>(), 10, 300);
    QCOMPARE(obj->property("result").value<int>(), 10);
}

void tst_usertypes::extraPropertyInQmlComplex()
{
    QRemoteObjectRegistryHost host(QUrl("local:testExtraComplex"));

    SimpleClockSimpleSource clock;
    QStringListModel *model = new QStringListModel();
    model->setStringList(QStringList() << "Track1" << "Track2" << "Track3");
    ComplexTypeSimpleSource source;
    source.setClock(&clock);
    source.setTracks(model);
    host.enableRemoting(&source);

    QQmlEngine e;
    QQmlComponent c(&e, SRCDIR "data/extraPropComplex.qml");
    QObject *obj = c.create();
    QVERIFY(obj);

    ComplexTypeReplica *rep = qobject_cast<ComplexTypeReplica*>(obj);
    QVERIFY(rep);

    // don't crash
    QTRY_VERIFY_WITH_TIMEOUT(rep->isInitialized(), 300);
}

void tst_usertypes::modelInQml()
{
    qmlRegisterType<TypeWithModelReplica>("usertypes", 1, 0, "TypeWithModelReplica");

    QRemoteObjectRegistryHost host(QUrl("local:testModel"));

    QStringListModel *model = new QStringListModel();
    model->setStringList(QStringList() << "Track1" << "Track2" << "Track3");
    TypeWithModelSimpleSource source;
    source.setTracks(model);
    host.enableRemoting<TypeWithModelSourceAPI>(&source);

    QQmlEngine e;
    QQmlComponent c(&e, SRCDIR "data/model.qml");
    QObject *obj = c.create();
    QVERIFY(obj);

    QTRY_VERIFY_WITH_TIMEOUT(obj->property("tracks").value<QAbstractItemModelReplica*>() != nullptr, 300);
    auto tracks = obj->property("tracks").value<QAbstractItemModelReplica*>();
    QTRY_VERIFY_WITH_TIMEOUT(tracks->isInitialized(), 300);

    TypeWithModelReplica *rep = qobject_cast<TypeWithModelReplica *>(obj);
    QVERIFY(rep->isInitialized());
}

void tst_usertypes::subObjectInQml()
{
    qmlRegisterType<TypeWithSubObjectReplica>("usertypes", 1, 0, "TypeWithSubObjectReplica");

    QRemoteObjectRegistryHost host(QUrl("local:testSubObject"));

    SimpleClockSimpleSource clock;
    TypeWithSubObjectSimpleSource source;
    source.setClock(&clock);
    host.enableRemoting(&source);

    QQmlEngine e;
    QQmlComponent c(&e, SRCDIR "data/subObject.qml");
    QObject *obj = c.create();
    QVERIFY(obj);

    TypeWithSubObjectReplica *replica = obj->property("replica").value<TypeWithSubObjectReplica*>();
    QVERIFY(replica);

    QTRY_VERIFY_WITH_TIMEOUT(replica->property("clock").value<SimpleClockReplica*>() != nullptr, 300);
    QTRY_COMPARE_WITH_TIMEOUT(obj->property("result").toInt(), 7, 300);
}

void tst_usertypes::complexInQml_data()
{
    QTest::addColumn<bool>("templated");
    QTest::addColumn<bool>("nullobject");
    QTest::newRow("non-templated pointer") << false << false;
    QTest::newRow("templated pointer") << true << false;
    QTest::newRow("non-templated nullptr") << false << true;
    QTest::newRow("templated nullptr") << true << true;
}

void tst_usertypes::complexInQml()
{
    QFETCH(bool, templated);
    QFETCH(bool, nullobject);

    QRemoteObjectRegistryHost host(QUrl("local:testModel"));

    QStringListModel *model = new QStringListModel();
    model->setStringList(QStringList() << "Track1" << "Track2" << "Track3");
    QScopedPointer<SimpleClockSimpleSource> clock;
    if (!nullobject)
        clock.reset(new SimpleClockSimpleSource());
    ComplexTypeSimpleSource source;
    source.setTracks(model);
    source.setClock(clock.data());
    if (templated)
        host.enableRemoting<ComplexTypeSourceAPI>(&source);
    else
        host.enableRemoting(&source);

    QQmlEngine e;
    QQmlComponent c(&e, SRCDIR "data/complex.qml");
    QObject *obj = c.create();
    QVERIFY(obj);

    QTRY_VERIFY_WITH_TIMEOUT(obj->property("tracks").value<QAbstractItemModelReplica*>() != nullptr, 300);
    auto tracks = obj->property("tracks").value<QAbstractItemModelReplica*>();
    QTRY_VERIFY_WITH_TIMEOUT(tracks->isInitialized(), 300);
    ComplexTypeReplica *rep = qobject_cast<ComplexTypeReplica *>(obj);
    QVERIFY(rep->waitForSource(300));
    QCOMPARE(rep->property("before").value<int>(), 0);
    QCOMPARE(rep->property("after").value<int>(), 42);
    if (nullobject) {
        QCOMPARE(rep->clock(), nullptr);
        QCOMPARE(source.clock(), nullptr);
    } else {
        QCOMPARE(source.clock()->hour(), 6);
        QCOMPARE(rep->clock()->hour(), source.clock()->hour());
    }
}

void tst_usertypes::watcherInQml()
{
    qmlRegisterType<TypeWithReplyReplica>("usertypes", 1, 0, "TypeWithReplyReplica");

    QRemoteObjectRegistryHost host(QUrl("local:testWatcher"));
    TypeWithReply source;
    host.enableRemoting(&source);

    QQmlEngine e;
    QQmlComponent c(&e, SRCDIR "data/watcher.qml");
    QObject *obj = c.create();
    QVERIFY(obj);

    QTRY_COMPARE_WITH_TIMEOUT(obj->property("result").value<QString>(), QString::fromLatin1("HELLO"), 300);
    QCOMPARE(obj->property("hasError").value<bool>(), false);

    QMetaObject::invokeMethod(obj, "callSlowFunction");
    QTRY_COMPARE_WITH_TIMEOUT(obj->property("hasError").value<bool>(), true, 1000);
    QVERIFY(obj->property("result").value<int>() != 10);

    QMetaObject::invokeMethod(obj, "callComplexFunction");
    QTRY_VERIFY_WITH_TIMEOUT(!obj->property("result").isNull(), 300);
    auto map = obj->property("result").value<QMap<QString,QString>>();
    QCOMPARE(map.value("one"), QString::fromLatin1("1"));
    QCOMPARE(obj->property("hasError").value<bool>(), false);
}

void tst_usertypes::hostInQml()
{
    qmlRegisterType<SimpleClockSimpleSource>("usertypes", 1, 0, "SimpleClockSimpleSource");

    QQmlEngine e;
    QQmlComponent c(&e, SRCDIR "data/hosted.qml");
    QObject *obj = c.create();
    QVERIFY(obj);

    QRemoteObjectNode node;
    node.connectToNode(QUrl("local:testHost"));
    SimpleClockReplica *replica = node.acquire<SimpleClockReplica>();
    QTRY_COMPARE_WITH_TIMEOUT(replica->state(), QRemoteObjectReplica::Valid, 300);

    QSignalSpy spy(replica, &SimpleClockReplica::timeUpdated);
    spy.wait();
    QCOMPARE(spy.count(), 1);
}

void tst_usertypes::twoReplicas()
{
    qmlRegisterType<SimpleClockReplica>("usertypes", 1, 0, "SimpleClockReplica");

    QRemoteObjectRegistryHost host(QUrl("local:testTwoReplicas"));
    SimpleClockSimpleSource clock;
    clock.setHour(7);
    host.enableRemoting(&clock);

    QQmlEngine e;
    QQmlComponent c(&e, SRCDIR "data/twoReplicas.qml");
    QObject *obj = c.create();
    QVERIFY(obj);

    QTRY_COMPARE_WITH_TIMEOUT(obj->property("result").value<int>(), 7, 300);
    QTRY_COMPARE_WITH_TIMEOUT(obj->property("result2").value<int>(), 7, 500);
}

QTEST_MAIN(tst_usertypes)

#include "tst_usertypes.moc"
