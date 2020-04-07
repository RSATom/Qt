/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt SVG module of the Qt Toolkit.
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

#include <QtCore>
#include <QtSvg>
#include <QtGui>
#include <stdlib.h>

static QImage render(const QString &filePath)
{
    fprintf(stdout, "Rendering %s\n", qPrintable(filePath));
    QSvgRenderer renderer(filePath);
    if (!renderer.isValid()) {
        fprintf(stderr, "Could not load SVG file %s\n", qPrintable(filePath));
        return QImage();
    }
    QImage image(480, 360, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    {
        QPainter p(&image);
        renderer.render(&p);
    }
    return image;
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument(QLatin1String("command"), QLatin1String("[create-baseline,diff]"));
    parser.addPositionalArgument(QLatin1String("path-to-svg-testsuite"), QLatin1String("Path to the svg/ sub-directory of the test suite"));

    parser.process(app);

    const auto args = parser.positionalArguments();

    if (args.count() != 2) {
        parser.showHelp(EXIT_FAILURE);
        return EXIT_FAILURE; // never reached
    }

    const QString commandAsString = args.at(0);
    const QString sourcePath = args.at(1);

    QDirIterator sourceFileIterator(sourcePath, QStringList(QLatin1String("*.svg")), QDir::Files);
    const QString baselinePath = "baseline";

    const auto referenceFilePath = [baselinePath](const QFileInfo &testCaseFileInfo) -> QString {
        return baselinePath + QLatin1Char('/') + testCaseFileInfo.baseName() + QLatin1String(".png");
    };

    if (commandAsString == "create-baseline") {
        while (sourceFileIterator.hasNext()) {
            sourceFileIterator.next();

            QImage image = render(sourceFileIterator.filePath());
            if (image.isNull())
                return EXIT_FAILURE;
            QString outputFileName = referenceFilePath(sourceFileIterator.fileInfo());
            if (!image.save(outputFileName)) {
                fprintf(stderr, "Could not save PNG file %s\n", qPrintable(outputFileName));
                return EXIT_FAILURE;
            }
        }
    } else if (commandAsString == "diff") {
        while (sourceFileIterator.hasNext()) {
            sourceFileIterator.next();

            QImage actual = render(sourceFileIterator.filePath());
            const QString referencePath = referenceFilePath(sourceFileIterator.fileInfo());
            QImage reference;
            if (!reference.load(referencePath)) {
                fprintf(stderr, "Could not load reference file %s\n", qPrintable(referencePath));
                return EXIT_FAILURE;
            }

            if (actual == reference)
                continue;

            QImage sideBySideImage(actual.width() * 2, actual.height(), QImage::Format_ARGB32);
            sideBySideImage.fill(Qt::transparent);
            {
                QPainter p(&sideBySideImage);
                p.drawImage(0, 0, actual);
                p.drawImage(actual.width(), 0, reference);
            }

            const QString sideBySideFileName = "difference/" + sourceFileIterator.fileInfo().baseName() + QLatin1String(".png");
            if (!sideBySideImage.save(sideBySideFileName)) {
                fprintf(stderr, "Could not save side-by-side image at %s\n", qPrintable(sideBySideFileName));
                return EXIT_FAILURE;
            }
        }
    } else {
        fprintf(stderr, "Unknown command %s\n", qPrintable(commandAsString));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

