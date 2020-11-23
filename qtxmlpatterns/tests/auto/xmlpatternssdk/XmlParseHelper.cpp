/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
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

#include "XmlParseHelper.h"

QT_BEGIN_NAMESPACE

namespace QPatternistSDK {

bool XmlParseHelper::parse(QIODevice *input)
{
    QXmlStreamReader reader(input);

    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.hasError())
            return false;

        switch (reader.tokenType()) {
        case QXmlStreamReader::StartElement:
            if (!startElement(reader.namespaceUri(), reader.name(),
                              reader.qualifiedName(), reader.attributes())) {
                return false;
            }
            break;
        case QXmlStreamReader::EndElement:
            if (!endElement(reader.namespaceUri(), reader.name(),
                            reader.qualifiedName())) {
                return false;
            }
            break;
        case QXmlStreamReader::Characters:
            if (!reader.isWhitespace() && !reader.text().toString().trimmed().isEmpty()) {
                if (!characters(reader.text()))
                    return false;
            }
            break;
        default:
            break;
        }
    }

    if (reader.isEndDocument() && !endDocument())
        return false;

    return true;
}

bool XmlParseHelper::startElement(const QStringRef &namespaceURI, const QStringRef &localName,
                                  const QStringRef &qName, const QXmlStreamAttributes &atts)
{
    Q_UNUSED(namespaceURI)
    Q_UNUSED(localName)
    Q_UNUSED(qName)
    Q_UNUSED(atts)
    return true;
}

bool XmlParseHelper::endElement(const QStringRef &namespaceURI, const QStringRef &localName,
                                const QStringRef &qName)
{
    Q_UNUSED(namespaceURI)
    Q_UNUSED(localName)
    Q_UNUSED(qName)
    return true;
}

bool XmlParseHelper::characters(const QStringRef &text)
{
    Q_UNUSED(text)
    return true;
}

bool XmlParseHelper::endDocument()
{
    return true;
}

} // namespace QPatternistSDK

QT_END_NAMESPACE
