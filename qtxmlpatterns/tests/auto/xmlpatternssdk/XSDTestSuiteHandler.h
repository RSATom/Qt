/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the autotests of the Qt Toolkit.
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

#ifndef PatternistSDK_XSDTestSuiteHandler_H
#define PatternistSDK_XSDTestSuiteHandler_H

#include <QUrl>

#include "ExternalSourceLoader.h"
#include "TestSuite.h"
#include "XmlParseHelper.h"
#include "XQTSTestCase.h"

QT_BEGIN_NAMESPACE

namespace QPatternistSDK
{
    class TestBaseLine;
    class TestGroup;
    class XSDTSTestCase;

    /**
     * @short Creates a TestSuite from the XSD Test Suite.
     *
     * The created TestSuite can be retrieved via testSuite().
     *
     * @note XSDTestSuiteHandler assumes the XML is valid by having been validated
     * against the W3C XML Schema. It has no safety checks for that the XML format
     * is correct but is hard coded for it. Thus, the behavior is undefined if
     * the XML is invalid.
     *
     * @ingroup PatternistSDK
     * @author Tobias Koenig <tobias.koenig@nokia.com>
     */
    class XSDTestSuiteHandler : public XmlParseHelper
    {
    public:
        /**
         * @param catalogFile the URI for the catalog file being parsed. This
         * URI is used for creating absolute URIs for files mentioned in
         * the catalog with relative URIs.
         * @param useExclusionList whether excludeTestGroups.txt should be used to ignore
         * test groups when loading
         */
        XSDTestSuiteHandler(const QUrl &catalogFile);
        bool characters(const QStringRef &ch) override;

        bool endElement(const QStringRef &namespaceURI, const QStringRef &localName,
                        const QStringRef &qName) override;
        bool startElement(const QStringRef &namespaceURI, const QStringRef &localName,
                          const QStringRef &qName, const QXmlStreamAttributes &atts) override;

        TestSuite *testSuite() const;

    private:
        TestSuite* m_ts;
        const QUrl m_catalogFile;

        TestGroup* m_topLevelGroup;
        TestGroup* m_currentTestSet;
        TestGroup* m_currentTestGroup;
        XSDTSTestCase* m_currentTestCase;
        bool m_inSchemaTest;
        bool m_inInstanceTest;
        bool m_inTestGroup;
        bool m_inDescription;
        bool m_schemaBlacklisted;
        QString m_documentation;
        QString m_currentSchemaLink;
        int m_counter;
        QSet<QString> m_blackList;
    };
}

QT_END_NAMESPACE

#endif
// vim: et:ts=4:sw=4:sts=4
