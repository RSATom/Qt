/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
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

#include "ieframe.h" // generated header

#include <metaobjectdump.h>
#include <textdialog.h>

#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

#include <QScreen>

#include <QDebug>

class MainWindow :public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

public slots:
    void navigate(const QString &);
    void showMetaObject();

private:
    SHDocVw::WebBrowser *m_browser;
};

MainWindow::MainWindow() : m_browser(new SHDocVw::WebBrowser)
{
    setCentralWidget(m_browser);

    auto fileMenu = menuBar()->addMenu("File");
    auto toolbar = new QToolBar;
    addToolBar(Qt::TopToolBarArea, toolbar);

    auto action = fileMenu->addAction("Dump MetaObject...",
                                      this, &MainWindow::showMetaObject);
    action->setShortcut(Qt::CTRL + Qt::Key_D);
    toolbar->addAction(action);

    action = fileMenu->addAction("Quit", this, &QWidget::close);
    action->setShortcut(Qt::CTRL + Qt::Key_Q);
    toolbar->addAction(action);
}

void  MainWindow::navigate(const QString &url)
{
    m_browser->Navigate(url);
}

void MainWindow::showMetaObject()
{
    auto mo = m_browser->metaObject();
    QString dump;
    {
        QTextStream str(&dump);
        str << *mo;
    }
    auto dialog = new TextDialog(dump, this);
    dialog->setWindowTitle(QLatin1String("MetaObject of ") + QLatin1String(mo->className()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->resize(screen()->geometry().size() * 2 / 3);
    dialog->show();
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle(qVersion());
    w.show();
    w.navigate("https://qt.io/");

    return a.exec();
}

#include "main.moc"
