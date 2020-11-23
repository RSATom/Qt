/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtXmlPatterns module of the Qt Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.3.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         XPATHSTYPE
#define YYLTYPE         XPATHLTYPE
/* Substitute the variable and function names.  */
#define yyparse         XPathparse
#define yylex           XPathlex
#define yyerror         XPatherror
#define yydebug         XPathdebug
#define yynerrs         XPathnerrs


/* First part of user prologue.  */
#line 50 "querytransformparser.ypp" /* yacc.c:337  */

/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtXmlPatterns module of the Qt Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <limits>

#include <QUrl>

#include <private/qabstractfloat_p.h>
#include <private/qandexpression_p.h>
#include <private/qanyuri_p.h>
#include <private/qapplytemplate_p.h>
#include <private/qargumentreference_p.h>
#include <private/qarithmeticexpression_p.h>
#include <private/qatomicstring_p.h>
#include <private/qattributeconstructor_p.h>
#include <private/qattributenamevalidator_p.h>
#include <private/qaxisstep_p.h>
#include <private/qbuiltintypes_p.h>
#include <private/qcalltemplate_p.h>
#include <private/qcastableas_p.h>
#include <private/qcastas_p.h>
#include <private/qcombinenodes_p.h>
#include <private/qcommentconstructor_p.h>
#include <private/qcommonnamespaces_p.h>
#include <private/qcommonsequencetypes_p.h>
#include <private/qcommonvalues_p.h>
#include <private/qcomputednamespaceconstructor_p.h>
#include <private/qcontextitem_p.h>
#include <private/qcopyof_p.h>
#include <private/qcurrentitemstore_p.h>
#include <private/qdebug_p.h>
#include <private/qdelegatingnamespaceresolver_p.h>
#include <private/qdocumentconstructor_p.h>
#include <private/qelementconstructor_p.h>
#include <private/qemptysequence_p.h>
#include <private/qemptysequencetype_p.h>
#include <private/qevaluationcache_p.h>
#include <private/qexpressionfactory_p.h>
#include <private/qexpressionsequence_p.h>
#include <private/qexpressionvariablereference_p.h>
#include <private/qexternalvariablereference_p.h>
#include <private/qforclause_p.h>
#include <private/qfunctioncall_p.h>
#include <private/qfunctionfactory_p.h>
#include <private/qfunctionsignature_p.h>
#include <private/qgeneralcomparison_p.h>
#include <private/qgenericpredicate_p.h>
#include <private/qgenericsequencetype_p.h>
#include <private/qifthenclause_p.h>
#include <private/qinstanceof_p.h>
#include <private/qletclause_p.h>
#include <private/qliteral_p.h>
#include <private/qlocalnametest_p.h>
#include <private/qnamespaceconstructor_p.h>
#include <private/qnamespacenametest_p.h>
#include <private/qncnameconstructor_p.h>
#include <private/qnodecomparison_p.h>
#include <private/qnodesort_p.h>
#include <private/qorderby_p.h>
#include <private/qorexpression_p.h>
#include <private/qparsercontext_p.h>
#include <private/qpath_p.h>
#include <private/qpatternistlocale_p.h>
#include <private/qpositionalvariablereference_p.h>
#include <private/qprocessinginstructionconstructor_p.h>
#include <private/qqnameconstructor_p.h>
#include <private/qqnametest_p.h>
#include <private/qqnamevalue_p.h>
#include <private/qquantifiedexpression_p.h>
#include <private/qrangeexpression_p.h>
#include <private/qrangevariablereference_p.h>
#include <private/qreturnorderby_p.h>
#include <private/qschemanumeric_p.h>
#include <private/qschematypefactory_p.h>
#include <private/qsimplecontentconstructor_p.h>
#include <private/qstaticbaseuristore_p.h>
#include <private/qstaticcompatibilitystore_p.h>
#include <private/qtemplateparameterreference_p.h>
#include <private/qtemplate_p.h>
#include <private/qtextnodeconstructor_p.h>
#include <private/qtokenizer_p.h>
#include <private/qtreatas_p.h>
#include <private/qtypechecker_p.h>
#include <private/qunaryexpression_p.h>
#include <private/qunresolvedvariablereference_p.h>
#include <private/quserfunctioncallsite_p.h>
#include <private/qvaluecomparison_p.h>
#include <private/qxpathhelper_p.h>
#include <private/qxsltsimplecontentconstructor_p.h>

/*
 * The cpp generated with bison 2.1 wants to
 * redeclare the C-like prototypes of 'malloc' and 'free', so we avoid that.
 */
#define YYMALLOC malloc
#define YYFREE free

QT_BEGIN_NAMESPACE

/* Due to Qt's QT_BEGIN_NAMESPACE magic, we can't use `using namespace', for some
 * undocumented reason. */
namespace QPatternist
{

/**
 * "Macro that you define with #define in the Bison declarations
 * section to request verbose, specific error message strings when
 * yyerror is called."
 */
#define YYERROR_VERBOSE 1

#define YYLTYPE_IS_TRIVIAL 0
#define YYINITDEPTH 1
#define yyoverflow parseInfo->handleStackOverflow

/* Suppresses `warning: "YYENABLE_NLS" is not defined`
 * @c YYENABLE_NLS enables Bison internationalization, and we don't
 * use that, so disable it. See the Bison Manual, section 4.5 Parser Internationalization.
 */
#define YYENABLE_NLS 0

static inline QSourceLocation fromYYLTYPE(const YYLTYPE &sourceLocator,
                                          const ParserContext *const parseInfo)
{
    return QSourceLocation(parseInfo->tokenizer->queryURI(),
                           sourceLocator.first_line,
                           sourceLocator.first_column);
}

/**
 * @internal
 * @relates QXmlQuery
 */
typedef QFlags<QXmlQuery::QueryLanguage> QueryLanguages;

/**
 * @short Flags invalid expressions and declarations in the currently
 * parsed language.
 *
 * Since this grammar is used for several languages: XQuery 1.0, XSL-T 2.0, and
 * XPath 2.0 inside XSL-T, and field and selector patterns in W3C XML Schema's
 * identity constraints, it is the union of all the constructs in these
 * languages. However, when dealing with each language individually, we
 * regularly need to disallow some expressions, such as direct element
 * constructors when parsing XSL-T, or the typeswitch when parsing XPath.
 *
 * This is further complicated by that XSLTTokenizer sometimes generates code
 * which is allowed in XQuery but not in XPath. For that reason the token
 * INTERNAL is sometimes generated, which signals that an expression, for
 * instance the @c let clause, should not be flagged as an error, because it's
 * used for internal purposes.
 *
 * Hence, this function is called from each expression and declaration with @p
 * allowedLanguages stating what languages it is allowed in.
 *
 * If @p isInternal is @c true, no error is raised. Otherwise, if the current
 * language is not in @p allowedLanguages, an error is raised.
 */
static void allowedIn(const QueryLanguages allowedLanguages,
                      const ParserContext *const parseInfo,
                      const YYLTYPE &sourceLocator,
                      const bool isInternal = false)
{
    /* We treat XPath 2.0 as a subset of XSL-T 2.0, so if XPath 2.0 is allowed
     * and XSL-T is the language, it's ok. */
    if(!isInternal &&
       (!allowedLanguages.testFlag(parseInfo->languageAccent) && !(allowedLanguages.testFlag(QXmlQuery::XPath20) && parseInfo->languageAccent == QXmlQuery::XSLT20)))
    {

        QString langName;

        switch(parseInfo->languageAccent)
        {
            case QXmlQuery::XPath20:
                langName = QLatin1String("XPath 2.0");
                break;
            case QXmlQuery::XSLT20:
                langName = QLatin1String("XSL-T 2.0");
                break;
            case QXmlQuery::XQuery10:
                langName = QLatin1String("XQuery 1.0");
                break;
            case QXmlQuery::XmlSchema11IdentityConstraintSelector:
                langName = QtXmlPatterns::tr("W3C XML Schema identity constraint selector");
                break;
            case QXmlQuery::XmlSchema11IdentityConstraintField:
                langName = QtXmlPatterns::tr("W3C XML Schema identity constraint field");
                break;
        }

        parseInfo->staticContext->error(QtXmlPatterns::tr("A construct was encountered "
                                                          "which is disallowed in the current language(%1).").arg(langName),
                                        ReportContext::XPST0003,
                                        fromYYLTYPE(sourceLocator, parseInfo));

    }
}

static inline bool isVariableReference(const Expression::ID id)
{
    return    id == Expression::IDExpressionVariableReference
           || id == Expression::IDRangeVariableReference
           || id == Expression::IDArgumentReference;
}

class ReflectYYLTYPE : public SourceLocationReflection
{
public:
    inline ReflectYYLTYPE(const YYLTYPE &sourceLocator,
                          const ParserContext *const pi) : m_sl(sourceLocator)
                                                         , m_parseInfo(pi)
    {
    }

    virtual const SourceLocationReflection *actualReflection() const
    {
        return this;
    }

    virtual QSourceLocation sourceLocation() const
    {
        return fromYYLTYPE(m_sl, m_parseInfo);
    }

    virtual QString description() const
    {
        Q_ASSERT(false);
        return QString();
    }

private:
    const YYLTYPE &m_sl;
    const ParserContext *const m_parseInfo;
};

/**
 * @short Centralizes a translation string for the purpose of increasing consistency.
 */
static inline QString unknownType()
{
    return QtXmlPatterns::tr("%1 is an unknown schema type.");
}

static inline Expression::Ptr create(Expression *const expr,
                                     const YYLTYPE &sourceLocator,
                                     const ParserContext *const parseInfo)
{
    parseInfo->staticContext->addLocation(expr, fromYYLTYPE(sourceLocator, parseInfo));
    return Expression::Ptr(expr);
}

static inline Template::Ptr create(Template *const expr,
                                   const YYLTYPE &sourceLocator,
                                   const ParserContext *const parseInfo)
{
    parseInfo->staticContext->addLocation(expr, fromYYLTYPE(sourceLocator, parseInfo));
    return Template::Ptr(expr);
}

static inline Expression::Ptr create(const Expression::Ptr &expr,
                                     const YYLTYPE &sourceLocator,
                                     const ParserContext *const parseInfo)
{
    parseInfo->staticContext->addLocation(expr.data(), fromYYLTYPE(sourceLocator, parseInfo));
    return expr;
}

static Expression::Ptr createSimpleContent(const Expression::Ptr &source,
                                           const YYLTYPE &sourceLocator,
                                           const ParserContext *const parseInfo)
{
    return create(parseInfo->isXSLT() ? new XSLTSimpleContentConstructor(source) : new SimpleContentConstructor(source),
                  sourceLocator,
                  parseInfo);
}

static void loadPattern(const Expression::Ptr &matchPattern,
                        TemplatePattern::Vector &ourPatterns,
                        const TemplatePattern::ID id,
                        const PatternPriority priority,
                        const Template::Ptr &temp)
{
    Q_ASSERT(temp);

    const PatternPriority effectivePriority = qIsNaN(priority) ? matchPattern->patternPriority() : priority;

    ourPatterns.append(TemplatePattern::Ptr(new TemplatePattern(matchPattern, effectivePriority, id, temp)));
}

static Expression::Ptr typeCheckTemplateBody(const Expression::Ptr &body,
                                             const SequenceType::Ptr &reqType,
                                             const ParserContext *const parseInfo)
{
    return TypeChecker::applyFunctionConversion(body, reqType,
                                                parseInfo->staticContext,
                                                ReportContext::XTTE0505,
                                                TypeChecker::Options(TypeChecker::AutomaticallyConvert | TypeChecker::GeneratePromotion));
}

static void registerNamedTemplate(const QXmlName &name,
                                  const Expression::Ptr &body,
                                  ParserContext *const parseInfo,
                                  const YYLTYPE &sourceLocator,
                                  const Template::Ptr &temp)
{
    Template::Ptr &e = parseInfo->namedTemplates[name];

    if(e)
    {
        parseInfo->staticContext->error(QtXmlPatterns::tr("A template with name %1 "
                                                          "has already been declared.")
                                        .arg(formatKeyword(parseInfo->staticContext->namePool(),
                                                                         name)),
                                        ReportContext::XTSE0660,
                                        fromYYLTYPE(sourceLocator, parseInfo));
    }
    else
    {
        e = temp;
        e->body = body;
    }
}

/**
 * @short Centralizes code for creating numeric literals.
 */
template<typename TNumberClass>
Expression::Ptr createNumericLiteral(const QString &in,
                                     const YYLTYPE &sl,
                                     const ParserContext *const parseInfo)
{
    const Item num(TNumberClass::fromLexical(in));

    if(num.template as<AtomicValue>()->hasError())
    {
        parseInfo->staticContext->error(QtXmlPatterns::tr("%1 is not a valid numeric literal.")
                                           .arg(formatData(in)),
                                        ReportContext::XPST0003, fromYYLTYPE(sl, parseInfo));
        return Expression::Ptr(); /* Avoid compiler warning. */
    }
    else
        return create(new Literal(num), sl, parseInfo);
}

/**
 * @short The generated Bison parser calls this function when there is a parse error.
 *
 * It is not called, nor should be, for logical errors(which the Bison not know about). For those,
 * ReportContext::error() is called.
 */
static int XPatherror(YYLTYPE *sourceLocator, const ParserContext *const parseInfo, const char *const msg)
{
    Q_UNUSED(sourceLocator);
    Q_ASSERT(parseInfo);

    parseInfo->staticContext->error(escape(QLatin1String(msg)), ReportContext::XPST0003, fromYYLTYPE(*sourceLocator, parseInfo));
    return 1;
}

/**
 * When we want to connect the OrderBy and ReturnOrderBy, it might be that we have other expressions, such
 * as @c where and @c let inbetween. We need to continue through them. This function does that.
 */
static ReturnOrderBy *locateReturnClause(const Expression::Ptr &expr)
{
    Q_ASSERT(expr);

    const Expression::ID id = expr->id();
    if(id == Expression::IDLetClause || id == Expression::IDIfThenClause || id == Expression::IDForClause)
        return locateReturnClause(expr->operands()[1]);
    else if(id == Expression::IDReturnOrderBy)
        return expr->as<ReturnOrderBy>();
    else
        return 0;
}

static inline bool isPredicate(const Expression::ID id)
{
    return id == Expression::IDGenericPredicate ||
           id == Expression::IDFirstItemPredicate;
}

/**
 * Assumes expr is an AxisStep wrapped in some kind of predicates or paths. Filters
 * through the predicates and returns the AxisStep.
 */
static Expression::Ptr findAxisStep(const Expression::Ptr &expr,
                                    const bool throughStructures = true)
{
    Q_ASSERT(expr);

    if(!throughStructures)
        return expr;

    Expression *candidate = expr.data();
    Expression::ID id = candidate->id();

    while(isPredicate(id) || id == Expression::IDPath)
    {
        const Expression::List &children = candidate->operands();
        if(children.isEmpty())
            return Expression::Ptr();
        else
        {
            candidate = children.first().data();
            id = candidate->id();
        }
    }

    if(id == Expression::IDEmptySequence)
        return Expression::Ptr();
    else
    {
        Q_ASSERT(candidate->is(Expression::IDAxisStep));
        return Expression::Ptr(candidate);
    }
}

static void changeToTopAxis(const Expression::Ptr &op)
{
    /* This axis must have been written away by now. */
    Q_ASSERT(op->as<AxisStep>()->axis() != QXmlNodeModelIndex::AxisChild);

    if(op->as<AxisStep>()->axis() != QXmlNodeModelIndex::AxisSelf)
        op->as<AxisStep>()->setAxis(QXmlNodeModelIndex::AxisAttributeOrTop);
}

/**
 * @short Writes @p operand1 and @p operand2, two operands in an XSL-T pattern,
 * into an equivalent XPath expression.
 *
 * Essentially, the following rewrite is done:
 *
 * <tt>
 * axis1::test1(a)/axis2::test2(b)
 *              =>
 * child-or-top::test2(b)[parent::test1(a)]
 * </tt>
 *
 * Section 5.5.3 The Meaning of a Pattern talks about rewrites that are applied to
 * only the first step in a pattern, but since we're doing rewrites more radically,
 * its line of reasoning cannot be followed.
 *
 * Keep in mind the rewrites that non-terminal PatternStep do.
 *
 * @see createIdPatternPath()
 */
static inline Expression::Ptr createPatternPath(const Expression::Ptr &operand1,
                                                const Expression::Ptr &operand2,
                                                const QXmlNodeModelIndex::Axis axis,
                                                const YYLTYPE &sl,
                                                const ParserContext *const parseInfo)
{
    const Expression::Ptr operandL(findAxisStep(operand1, false));

    if(operandL->is(Expression::IDAxisStep))
        operandL->as<AxisStep>()->setAxis(axis);
    else
        findAxisStep(operand1)->as<AxisStep>()->setAxis(axis);

    return create(GenericPredicate::create(operand2, operandL,
                                           parseInfo->staticContext, fromYYLTYPE(sl, parseInfo)), sl, parseInfo);
}

/**
 * @short Performs the same role as createPatternPath(), but is tailored
 * for @c fn:key() and @c fn:id().
 *
 * @c fn:key() and @c fn:id() can be part of path patterns(only as the first step,
 * to be precise) and that poses a challenge to rewriting because what
 * createPatternPath() is not possible to express, since the functions cannot be
 * node tests. E.g, this rewrite is not possible:
 *
 * <tt>
 * id-or-key/abc
 *  =>
 * child-or-top::abc[parent::id-or-key]
 * </tt>
 *
 * Our approach is to rewrite like this:
 *
 * <tt>
 * id-or-key/abc
 * =>
 * child-or-top::abc[parent::node is id-or-key]
 * </tt>
 *
 * @p operand1 is the call to @c fn:key() or @c fn:id(), @p operand2
 * the right operand, and @p axis the target axis to rewrite to.
 *
 * @see createPatternPath()
 */
static inline Expression::Ptr createIdPatternPath(const Expression::Ptr &operand1,
                                                  const Expression::Ptr &operand2,
                                                  const QXmlNodeModelIndex::Axis axis,
                                                  const YYLTYPE &sl,
                                                  const ParserContext *const parseInfo)
{
    const Expression::Ptr operandR(findAxisStep(operand2));
    Q_ASSERT(operandR);
    changeToTopAxis(operandR);

    const Expression::Ptr parentStep(create(new AxisStep(axis, BuiltinTypes::node),
                                            sl,
                                            parseInfo));
    const Expression::Ptr isComp(create(new NodeComparison(parentStep,
                                                           QXmlNodeModelIndex::Is,
                                                           operand1),
                                         sl,
                                         parseInfo));

    return create(GenericPredicate::create(operandR, isComp,
                                           parseInfo->staticContext, fromYYLTYPE(sl, parseInfo)), sl, parseInfo);
}

/**
 * @short Centralizes a translation message, for the
 * purpose of consistency and modularization.
 */
static inline QString prologMessage(const char *const msg)
{
    Q_ASSERT(msg);
    return QtXmlPatterns::tr("Only one %1 declaration can occur in the query prolog.").arg(formatKeyword(msg));
}

/**
 * @short Resolves against the static base URI and checks that @p collation
 * is a supported Unicode Collation.
 *
 * "If a default collation declaration specifies a collation by a
 *  relative URI, that relative URI is resolved to an absolute
 *  URI using the base URI in the static context."
 *
 * @returns the Unicode Collation properly resolved, if @p collation is a valid collation
 */
template<const ReportContext::ErrorCode errorCode>
static QUrl resolveAndCheckCollation(const QString &collation,
                                     const ParserContext *const parseInfo,
                                     const YYLTYPE &sl)
{
    Q_ASSERT(parseInfo);
    const ReflectYYLTYPE ryy(sl, parseInfo);

    QUrl uri(AnyURI::toQUrl<ReportContext::XQST0046>(collation, parseInfo->staticContext, &ryy));

    if(uri.isRelative())
        uri = parseInfo->staticContext->baseURI().resolved(uri);

    XPathHelper::checkCollationSupport<errorCode>(uri.toString(), parseInfo->staticContext, &ryy);

    return uri;
}

/* The Bison generated parser declares macros that aren't used
 * so suppress the warnings by fake usage of them.
 *
 * We do the same for some more defines in the first action. */
#if    defined(YYLSP_NEEDED)    \
    || defined(YYBISON)         \
    || defined(YYBISON_VERSION) \
    || defined(YYPURE)          \
    || defined(yydebug)         \
    || defined(YYSKELETON_NAME)
#endif

/**
 * Wraps @p operand with a CopyOf in case it makes any difference.
 *
 * There is no need to wrap the return value in a call to create(), it's
 * already done.
 */
static Expression::Ptr createCopyOf(const Expression::Ptr &operand,
                                    const ParserContext *const parseInfo,
                                    const YYLTYPE &sl)
{
    return create(new CopyOf(operand, parseInfo->inheritNamespacesMode,
                             parseInfo->preserveNamespacesMode), sl, parseInfo);
}

static Expression::Ptr createCompatStore(const Expression::Ptr &expr,
                                         const YYLTYPE &sourceLocator,
                                         const ParserContext *const parseInfo)
{
    return create(new StaticCompatibilityStore(expr), sourceLocator, parseInfo);
}

/**
 * @short Creates an Expression that corresponds to <tt>/</tt>. This is literally
 * <tt>fn:root(self::node()) treat as document-node()</tt>.
 */
static Expression::Ptr createRootExpression(const ParserContext *const parseInfo,
                                            const YYLTYPE &sl)
{
    Q_ASSERT(parseInfo);
    const QXmlName name(StandardNamespaces::fn, StandardLocalNames::root);

    Expression::List args;
    args.append(create(new ContextItem(), sl, parseInfo));

    const ReflectYYLTYPE ryy(sl, parseInfo);

    const Expression::Ptr fnRoot(parseInfo->staticContext->functionSignatures()
                                 ->createFunctionCall(name, args, parseInfo->staticContext, &ryy));
    Q_ASSERT(fnRoot);

    return create(new TreatAs(create(fnRoot, sl, parseInfo), CommonSequenceTypes::ExactlyOneDocumentNode), sl, parseInfo);
}

static int XPathlex(YYSTYPE *lexVal, YYLTYPE *sourceLocator, const ParserContext *const parseInfo)
{
#ifdef Patternist_DEBUG_PARSER
    /**
     * "External integer variable set to zero by default. If yydebug
     *  is given a nonzero value, the parser will output information on
     *  input symbols and parser action. See section Debugging Your Parser."
     */
#   define YYDEBUG 1

    extern int XPathdebug;
    XPathdebug = 1;
#endif

    Q_ASSERT(parseInfo);

    const Tokenizer::Token tok(parseInfo->tokenizer->nextToken(sourceLocator));

    (*lexVal).sval = tok.value;

    return static_cast<int>(tok.type);
}

/**
 * @short Creates a path expression which contains the step <tt>//</tt> between
 * @p begin and and @p end.
 *
 * <tt>begin//end</tt> is a short form for: <tt>begin/descendant-or-self::node()/end</tt>
 *
 * This will be compiled as two-path expression: <tt>(/)/(//.)/step/</tt>
 */
static Expression::Ptr createSlashSlashPath(const Expression::Ptr &begin,
                                            const Expression::Ptr &end,
                                            const YYLTYPE &sourceLocator,
                                            const ParserContext *const parseInfo)
{
    const Expression::Ptr twoSlash(create(new AxisStep(QXmlNodeModelIndex::AxisDescendantOrSelf, BuiltinTypes::node), sourceLocator, parseInfo));
    const Expression::Ptr p1(create(new Path(begin, twoSlash), sourceLocator, parseInfo));

    return create(new Path(p1, end), sourceLocator, parseInfo);
}

/**
 * @short Creates a call to <tt>fn:concat()</tt> with @p args as the arguments.
 */
static inline Expression::Ptr createConcatFN(const ParserContext *const parseInfo,
                                             const Expression::List &args,
                                             const YYLTYPE &sourceLocator)
{
    Q_ASSERT(parseInfo);
    const QXmlName name(StandardNamespaces::fn, StandardLocalNames::concat);
    const ReflectYYLTYPE ryy(sourceLocator, parseInfo);

    return create(parseInfo->staticContext->functionSignatures()->createFunctionCall(name, args, parseInfo->staticContext, &ryy),
                  sourceLocator, parseInfo);
}

static inline Expression::Ptr createDirAttributeValue(const Expression::List &content,
                                                      const ParserContext *const parseInfo,
                                                      const YYLTYPE &sourceLocator)
{
    if(content.isEmpty())
        return create(new EmptySequence(), sourceLocator, parseInfo);
    else if(content.size() == 1)
        return content.first();
    else
        return createConcatFN(parseInfo, content, sourceLocator);
}

/**
 * @short Checks for variable initialization circularity.
 *
 * "A recursive function that checks for recursion is full of ironies."
 *
 *      -- The Salsa Master
 *
 * Issues an error via @p parseInfo's StaticContext if the initialization
 * expression @p checkee for the global variable @p var, contains a variable
 * reference to @p var. That is, if there's a circularity.
 *
 * @see <a href="http://www.w3.org/TR/xquery/#ERRXQST0054">XQuery 1.0: An XML
 * Query Language, err:XQST0054</a>
 */
static void checkVariableCircularity(const VariableDeclaration::Ptr &var,
                                     const Expression::Ptr &checkee,
                                     const VariableDeclaration::Type type,
                                     FunctionSignature::List &signList,
                                     const ParserContext *const parseInfo)
{
    Q_ASSERT(var);
    Q_ASSERT(checkee);
    Q_ASSERT(parseInfo);

    const Expression::ID id = checkee->id();

    if(id == Expression::IDExpressionVariableReference)
    {
        const ExpressionVariableReference *const ref =
                    static_cast<const ExpressionVariableReference *>(checkee.data());

        if(var->slot == ref->slot() && type == ref->variableDeclaration()->type)
        {
            parseInfo->staticContext->error(QtXmlPatterns::tr("The initialization of variable %1 "
                                                              "depends on itself").arg(formatKeyword(var, parseInfo->staticContext->namePool())),
                                            parseInfo->isXSLT() ? ReportContext::XTDE0640 : ReportContext::XQST0054, ref);
            return;
        }
        else
        {
            /* If the variable we're checking is below another variable, it can be a recursive
             * dependency through functions, so we need to check variable references too. */
            checkVariableCircularity(var, ref->sourceExpression(), type, signList, parseInfo);
            return;
        }
    }
    else if(id == Expression::IDUserFunctionCallsite)
    {
        const UserFunctionCallsite::Ptr callsite(checkee);
        const FunctionSignature::Ptr sign(callsite->callTargetDescription());
        const FunctionSignature::List::const_iterator end(signList.constEnd());
        FunctionSignature::List::const_iterator it(signList.constBegin());
        bool noMatch = true;

        for(; it != end; ++it)
        {
            if(*it == sign)
            {
                /* The variable we're checking is depending on a function that's recursive. The
                 * user has written a weird query, in other words. Since it's the second time
                 * we've encountered a callsite, we now skip it. */
                noMatch = false;
                break;
            }
        }

        if(noMatch)
        {
            signList.append(sign);
            /* Check the body of the function being called. */
            checkVariableCircularity(var, callsite->body(), type, signList, parseInfo);
        }
        /* Continue with the operands, such that we also check the arguments of the callsite. */
    }
    else if(id == Expression::IDUnresolvedVariableReference)
    {
        /* We're called before it has rewritten itself. */
        checkVariableCircularity(var, checkee->as<UnresolvedVariableReference>()->replacement(), type, signList, parseInfo);
    }

    /* Check the operands. */
    const Expression::List ops(checkee->operands());
    if(ops.isEmpty())
        return;

    const Expression::List::const_iterator end(ops.constEnd());
    Expression::List::const_iterator it(ops.constBegin());

    for(; it != end; ++it)
        checkVariableCircularity(var, *it, type, signList, parseInfo);
}

static void variableUnavailable(const QXmlName &variableName,
                                const ParserContext *const parseInfo,
                                const YYLTYPE &location)
{
    parseInfo->staticContext->error(QtXmlPatterns::tr("No variable with name %1 exists")
                                       .arg(formatKeyword(parseInfo->staticContext->namePool(), variableName)),
                                    ReportContext::XPST0008, fromYYLTYPE(location, parseInfo));
}

/**
 * The Cardinality in a TypeDeclaration for a variable in a quantification has no effect,
 * and this function ensures this by changing @p type to Cardinality Cardinality::zeroOrMore().
 *
 * @see <a href="http://www.w3.org/Bugs/Public/show_bug.cgi?id=3305">Bugzilla Bug 3305
 * Cardinality + on range variables</a>
 * @see ParserContext::finalizePushedVariable()
 */
static inline SequenceType::Ptr quantificationType(const SequenceType::Ptr &type)
{
    Q_ASSERT(type);
    return makeGenericSequenceType(type->itemType(), Cardinality::zeroOrMore());
}

/**
 * @p seqType and @p expr may be @c null.
 */
static Expression::Ptr pushVariable(const QXmlName name,
                                    const SequenceType::Ptr &seqType,
                                    const Expression::Ptr &expr,
                                    const VariableDeclaration::Type type,
                                    const YYLTYPE &sourceLocator,
                                    ParserContext *const parseInfo,
                                    const bool checkSource = true)
{
    Q_ASSERT(!name.isNull());
    Q_ASSERT(parseInfo);

    /* -2 will cause Q_ASSERTs to trigger if it isn't changed. */
    VariableSlotID slot = -2;

    switch(type)
    {
        case VariableDeclaration::FunctionArgument:
        case VariableDeclaration::ExpressionVariable:
        {
            slot = parseInfo->allocateExpressionSlot();
            break;
        }
        case VariableDeclaration::GlobalVariable:
        {
            slot = parseInfo->allocateGlobalVariableSlot();
            break;
        }
        case VariableDeclaration::RangeVariable:
        {
            slot = parseInfo->staticContext->allocateRangeSlot();
            break;
        }
        case VariableDeclaration::PositionalVariable:
        {
            slot = parseInfo->allocatePositionalSlot();
            break;
        }
        case VariableDeclaration::TemplateParameter:
            /* Fallthrough. We do nothing, template parameters
             * doesn't use context slots at all, they're hashed
             * on the name. */
        case VariableDeclaration::ExternalVariable:
            /* We do nothing, external variables doesn't use
             *context slots/stack frames at all. */
            ;
    }

    const VariableDeclaration::Ptr var(new VariableDeclaration(name, slot, type, seqType));

    Expression::Ptr checked;

    if(checkSource && seqType)
    {
        if(expr)
        {
            /* We only want to add conversion for function arguments, and variables
             * if we're XSL-T.
             *
             * We unconditionally skip TypeChecker::CheckFocus because the StaticContext we
             * pass hasn't set up the focus yet, since that's the parent's responsibility. */
            const TypeChecker::Options options((   type == VariableDeclaration::FunctionArgument
                                                || type == VariableDeclaration::TemplateParameter
                                                || parseInfo->isXSLT())
                                               ? TypeChecker::AutomaticallyConvert : TypeChecker::Options());

            checked = TypeChecker::applyFunctionConversion(expr, seqType, parseInfo->staticContext,
                                                           parseInfo->isXSLT() ? ReportContext::XTTE0570 : ReportContext::XPTY0004,
                                                           options);
        }
    }
    else
        checked = expr;

    /* Add an evaluation cache for all expression variables. No EvaluationCache is needed for
     * positional variables because in the end they are calls to Iterator::position(). Similarly,
     * no need to cache range variables either because they are calls to DynamicContext::rangeVariable().
     *
     * We don't do it for function arguments because the Expression being cached depends -- it depends
     * on the callsite. UserFunctionCallsite is responsible for the evaluation caches in that case.
     *
     * In some cases the EvaluationCache instance isn't necessary, but in those cases EvaluationCache
     * optimizes itself away. */
    if(type == VariableDeclaration::ExpressionVariable)
        checked = create(new EvaluationCache<false>(checked, var, parseInfo->allocateCacheSlot()), sourceLocator, parseInfo);
    else if(type == VariableDeclaration::GlobalVariable)
        checked = create(new EvaluationCache<true>(checked, var, parseInfo->allocateCacheSlot()), sourceLocator, parseInfo);

    var->setExpression(checked);

    parseInfo->variables.push(var);
    return checked;
}

static inline VariableDeclaration::Ptr variableByName(const QXmlName name,
                                                      const ParserContext *const parseInfo)
{
    Q_ASSERT(!name.isNull());
    Q_ASSERT(parseInfo);

    /* We walk the list backwards. */
    const VariableDeclaration::Stack::const_iterator start(parseInfo->variables.constBegin());
    VariableDeclaration::Stack::const_iterator it(parseInfo->variables.constEnd());

    while(it != start)
    {
        --it;
        Q_ASSERT(*it);
        if((*it)->name == name)
            return *it;
    }

    return VariableDeclaration::Ptr();
}

static Expression::Ptr resolveVariable(const QXmlName &name,
                                       const YYLTYPE &sourceLocator,
                                       ParserContext *const parseInfo,
                                       const bool raiseErrorOnUnavailability)
{
    const VariableDeclaration::Ptr var(variableByName(name, parseInfo));
    Expression::Ptr retval;

    if(var && var->type != VariableDeclaration::ExternalVariable)
    {
        switch(var->type)
        {
            case VariableDeclaration::RangeVariable:
            {
                retval = create(new RangeVariableReference(var->expression(), var->slot), sourceLocator, parseInfo);
                break;
            }
            case VariableDeclaration::GlobalVariable:
            /* Fallthrough. From the perspective of an ExpressionVariableReference, it can't tell
             * a difference between a global and a local expression variable. However, the cache
             * mechanism must. */
            case VariableDeclaration::ExpressionVariable:
            {
                retval = create(new ExpressionVariableReference(var->slot, var.data()), sourceLocator, parseInfo);
                break;
            }
            case VariableDeclaration::FunctionArgument:
            {
                retval = create(new ArgumentReference(var->sequenceType, var->slot), sourceLocator, parseInfo);
                break;
            }
            case VariableDeclaration::PositionalVariable:
            {
                retval = create(new PositionalVariableReference(var->slot), sourceLocator, parseInfo);
                break;
            }
            case VariableDeclaration::TemplateParameter:
            {
                retval = create(new TemplateParameterReference(var.data()), sourceLocator, parseInfo);
                break;
            }
            case VariableDeclaration::ExternalVariable:
                /* This code path will never be hit, but the case
                 * label silences a warning. See above. */
                ;
        }
        Q_ASSERT(retval);
        var->references.append(retval);
    }
    else
    {
        /* Let's see if your external variable loader can provide us with one. */
        const SequenceType::Ptr varType(parseInfo->staticContext->
                                        externalVariableLoader()->announceExternalVariable(name, CommonSequenceTypes::ZeroOrMoreItems));

        if(varType)
        {
            const Expression::Ptr extRef(create(new ExternalVariableReference(name, varType), sourceLocator, parseInfo));
            const Expression::Ptr checked(TypeChecker::applyFunctionConversion(extRef, varType, parseInfo->staticContext));
            retval = checked;
        }
        else if(!raiseErrorOnUnavailability && parseInfo->isXSLT())
        {
            /* In XSL-T, global variables are in scope for the whole
             * stylesheet, so we must resolve this first at the end. */
            retval = create(new UnresolvedVariableReference(name), sourceLocator, parseInfo);
            parseInfo->unresolvedVariableReferences.insert(name, retval);
        }
        else
            variableUnavailable(name, parseInfo, sourceLocator);
    }

    return retval;
}

static Expression::Ptr createReturnOrderBy(const OrderSpecTransfer::List &orderSpecTransfer,
                                           const Expression::Ptr &returnExpr,
                                           const OrderBy::Stability stability,
                                           const YYLTYPE &sourceLocator,
                                           const ParserContext *const parseInfo)
{
    // TODO do resize(orderSpec.size() + 1)
    Expression::List exprs;
    OrderBy::OrderSpec::Vector orderSpecs;

    exprs.append(returnExpr);

    const int len = orderSpecTransfer.size();

    for(int i = 0; i < len; ++i)
    {
        exprs.append(orderSpecTransfer.at(i).expression);
        orderSpecs.append(orderSpecTransfer.at(i).orderSpec);
    }

    return create(new ReturnOrderBy(stability, orderSpecs, exprs), sourceLocator, parseInfo);
}


#line 1134 "qquerytransformparser.cpp" /* yacc.c:337  */
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "qquerytransformparser_p.h".  */
#ifndef YY_XPATH_QQUERYTRANSFORMPARSER_P_H_INCLUDED
# define YY_XPATH_QQUERYTRANSFORMPARSER_P_H_INCLUDED
/* Debug traces.  */
#ifndef XPATHDEBUG
# if defined YYDEBUG
#  if YYDEBUG
#   define XPATHDEBUG 1
#  else
#   define XPATHDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define XPATHDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined XPATHDEBUG */
#if XPATHDEBUG
extern int XPathdebug;
#endif

/* Token type.  */
#ifndef XPATHTOKENTYPE
# define XPATHTOKENTYPE
  enum XPathtokentype
  {
    T_END_OF_FILE = 0,
    T_STRING_LITERAL = 258,
    T_NON_BOUNDARY_WS = 259,
    T_XPATH2_STRING_LITERAL = 260,
    T_QNAME = 261,
    T_NCNAME = 262,
    T_CLARK_NAME = 263,
    T_ANY_LOCAL_NAME = 264,
    T_ANY_PREFIX = 265,
    T_NUMBER = 266,
    T_XPATH2_NUMBER = 267,
    T_ANCESTOR = 268,
    T_ANCESTOR_OR_SELF = 269,
    T_AND = 270,
    T_APOS = 271,
    T_APPLY_TEMPLATE = 272,
    T_AS = 273,
    T_ASCENDING = 274,
    T_ASSIGN = 275,
    T_AT = 276,
    T_AT_SIGN = 277,
    T_ATTRIBUTE = 278,
    T_AVT = 279,
    T_BAR = 280,
    T_BASEURI = 281,
    T_BEGIN_END_TAG = 282,
    T_BOUNDARY_SPACE = 283,
    T_BY = 284,
    T_CALL_TEMPLATE = 285,
    T_CASE = 286,
    T_CASTABLE = 287,
    T_CAST = 288,
    T_CHILD = 289,
    T_COLLATION = 290,
    T_COLONCOLON = 291,
    T_COMMA = 292,
    T_COMMENT = 293,
    T_COMMENT_START = 294,
    T_CONSTRUCTION = 295,
    T_COPY_NAMESPACES = 296,
    T_CURLY_LBRACE = 297,
    T_CURLY_RBRACE = 298,
    T_DECLARE = 299,
    T_DEFAULT = 300,
    T_DESCENDANT = 301,
    T_DESCENDANT_OR_SELF = 302,
    T_DESCENDING = 303,
    T_DIV = 304,
    T_DOCUMENT = 305,
    T_DOCUMENT_NODE = 306,
    T_DOLLAR = 307,
    T_DOT = 308,
    T_DOTDOT = 309,
    T_ELEMENT = 310,
    T_ELSE = 311,
    T_EMPTY = 312,
    T_EMPTY_SEQUENCE = 313,
    T_ENCODING = 314,
    T_END_SORT = 315,
    T_EQ = 316,
    T_ERROR = 317,
    T_EVERY = 318,
    T_EXCEPT = 319,
    T_EXTERNAL = 320,
    T_FOLLOWING = 321,
    T_FOLLOWING_SIBLING = 322,
    T_FOLLOWS = 323,
    T_FOR_APPLY_TEMPLATE = 324,
    T_FOR = 325,
    T_FUNCTION = 326,
    T_GE = 327,
    T_G_EQ = 328,
    T_G_GE = 329,
    T_G_GT = 330,
    T_G_LE = 331,
    T_G_LT = 332,
    T_G_NE = 333,
    T_GREATEST = 334,
    T_GT = 335,
    T_IDIV = 336,
    T_IF = 337,
    T_IMPORT = 338,
    T_INHERIT = 339,
    T_IN = 340,
    T_INSTANCE = 341,
    T_INTERSECT = 342,
    T_IS = 343,
    T_ITEM = 344,
    T_LAX = 345,
    T_LBRACKET = 346,
    T_LEAST = 347,
    T_LE = 348,
    T_LET = 349,
    T_LPAREN = 350,
    T_LT = 351,
    T_MAP = 352,
    T_MATCHES = 353,
    T_MINUS = 354,
    T_MODE = 355,
    T_MOD = 356,
    T_MODULE = 357,
    T_NAME = 358,
    T_NAMESPACE = 359,
    T_NE = 360,
    T_NODE = 361,
    T_NO_INHERIT = 362,
    T_NO_PRESERVE = 363,
    T_OF = 364,
    T_OPTION = 365,
    T_ORDERED = 366,
    T_ORDERING = 367,
    T_ORDER = 368,
    T_OR = 369,
    T_PARENT = 370,
    T_PI_START = 371,
    T_PLUS = 372,
    T_POSITION_SET = 373,
    T_PRAGMA_END = 374,
    T_PRAGMA_START = 375,
    T_PRECEDES = 376,
    T_PRECEDING = 377,
    T_PRECEDING_SIBLING = 378,
    T_PRESERVE = 379,
    T_PRIORITY = 380,
    T_PROCESSING_INSTRUCTION = 381,
    T_QUESTION = 382,
    T_QUICK_TAG_END = 383,
    T_QUOTE = 384,
    T_RBRACKET = 385,
    T_RETURN = 386,
    T_RPAREN = 387,
    T_SATISFIES = 388,
    T_SCHEMA_ATTRIBUTE = 389,
    T_SCHEMA_ELEMENT = 390,
    T_SCHEMA = 391,
    T_SELF = 392,
    T_SEMI_COLON = 393,
    T_SLASH = 394,
    T_SLASHSLASH = 395,
    T_SOME = 396,
    T_SORT = 397,
    T_STABLE = 398,
    T_STAR = 399,
    T_STRICT = 400,
    T_STRIP = 401,
    T_SUCCESS = 402,
    T_COMMENT_CONTENT = 403,
    T_PI_CONTENT = 404,
    T_PI_TARGET = 405,
    T_XSLT_VERSION = 406,
    T_TEMPLATE = 407,
    T_TEXT = 408,
    T_THEN = 409,
    T_TO = 410,
    T_TREAT = 411,
    T_TUNNEL = 412,
    T_TYPESWITCH = 413,
    T_UNION = 414,
    T_UNORDERED = 415,
    T_VALIDATE = 416,
    T_VARIABLE = 417,
    T_VERSION = 418,
    T_WHERE = 419,
    T_XQUERY = 420,
    T_INTERNAL = 421,
    T_INTERNAL_NAME = 422,
    T_CURRENT = 423
  };
#endif

/* Value type.  */

/* Location type.  */
#if ! defined XPATHLTYPE && ! defined XPATHLTYPE_IS_DECLARED
typedef struct XPATHLTYPE XPATHLTYPE;
struct XPATHLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define XPATHLTYPE_IS_DECLARED 1
# define XPATHLTYPE_IS_TRIVIAL 1
#endif



int XPathparse (QT_PREPEND_NAMESPACE(QPatternist)::ParserContext *const parseInfo);

#endif /* !YY_XPATH_QQUERYTRANSFORMPARSER_P_H_INCLUDED  */



#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined XPATHLTYPE_IS_TRIVIAL && XPATHLTYPE_IS_TRIVIAL \
             && defined XPATHSTYPE_IS_TRIVIAL && XPATHSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2052

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  169
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  237
/* YYNRULES -- Number of rules.  */
#define YYNRULES  472
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  812

#define YYUNDEFTOK  2
#define YYMAXUTOK   423

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168
};

#if XPATHDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1431,  1431,  1432,  1434,  1435,  1466,  1467,  1483,  1581,
    1583,  1589,  1591,  1598,  1604,  1610,  1617,  1620,  1624,  1628,
    1648,  1662,  1666,  1660,  1729,  1733,  1750,  1753,  1755,  1760,
    1761,  1765,  1766,  1770,  1774,  1778,  1780,  1781,  1783,  1785,
    1831,  1845,  1850,  1855,  1856,  1858,  1873,  1888,  1898,  1913,
    1917,  1922,  1936,  1940,  1945,  1959,  1964,  1969,  1974,  1979,
    1995,  2018,  2026,  2027,  2028,  2030,  2047,  2048,  2050,  2051,
    2053,  2054,  2056,  2111,  2115,  2121,  2124,  2129,  2143,  2147,
    2153,  2152,  2261,  2264,  2270,  2291,  2297,  2301,  2303,  2308,
    2318,  2319,  2324,  2325,  2334,  2404,  2415,  2416,  2420,  2425,
    2494,  2495,  2499,  2504,  2548,  2549,  2554,  2561,  2567,  2568,
    2569,  2570,  2571,  2572,  2578,  2583,  2589,  2592,  2597,  2603,
    2609,  2613,  2638,  2639,  2643,  2647,  2641,  2688,  2691,  2686,
    2707,  2708,  2709,  2712,  2716,  2724,  2723,  2737,  2736,  2745,
    2746,  2747,  2749,  2757,  2768,  2771,  2773,  2778,  2785,  2792,
    2798,  2818,  2823,  2829,  2832,  2834,  2835,  2842,  2848,  2852,
    2857,  2858,  2861,  2865,  2860,  2875,  2879,  2874,  2887,  2890,
    2894,  2889,  2904,  2908,  2903,  2916,  2918,  2946,  2945,  2957,
    2965,  2956,  2976,  2977,  2980,  2984,  2989,  2994,  2993,  3009,
    3015,  3016,  3022,  3023,  3029,  3030,  3031,  3032,  3034,  3035,
    3041,  3042,  3048,  3049,  3051,  3052,  3058,  3059,  3060,  3061,
    3063,  3064,  3074,  3075,  3081,  3082,  3084,  3088,  3093,  3094,
    3101,  3102,  3108,  3109,  3115,  3116,  3122,  3123,  3129,  3133,
    3138,  3139,  3140,  3142,  3148,  3149,  3150,  3151,  3152,  3153,
    3155,  3160,  3161,  3162,  3163,  3164,  3165,  3167,  3172,  3173,
    3174,  3176,  3190,  3191,  3192,  3194,  3211,  3215,  3220,  3221,
    3223,  3228,  3229,  3231,  3237,  3241,  3247,  3250,  3251,  3255,
    3264,  3269,  3273,  3274,  3279,  3278,  3293,  3301,  3300,  3316,
    3324,  3324,  3333,  3335,  3338,  3343,  3345,  3349,  3415,  3418,
    3424,  3427,  3436,  3440,  3444,  3449,  3450,  3455,  3456,  3459,
    3458,  3488,  3490,  3491,  3493,  3537,  3538,  3539,  3540,  3541,
    3542,  3543,  3544,  3545,  3546,  3547,  3548,  3551,  3550,  3561,
    3572,  3577,  3579,  3584,  3585,  3590,  3594,  3596,  3600,  3609,
    3616,  3617,  3623,  3624,  3625,  3626,  3627,  3628,  3629,  3630,
    3640,  3641,  3646,  3651,  3657,  3663,  3668,  3673,  3678,  3684,
    3689,  3694,  3724,  3728,  3735,  3737,  3741,  3746,  3747,  3748,
    3782,  3791,  3780,  4032,  4036,  4056,  4059,  4065,  4070,  4075,
    4081,  4084,  4094,  4101,  4105,  4111,  4125,  4131,  4148,  4153,
    4166,  4167,  4168,  4169,  4170,  4171,  4172,  4174,  4182,  4181,
    4221,  4224,  4229,  4244,  4249,  4256,  4268,  4272,  4268,  4278,
    4280,  4284,  4286,  4301,  4305,  4314,  4319,  4323,  4329,  4332,
    4337,  4342,  4347,  4348,  4349,  4350,  4352,  4353,  4354,  4355,
    4360,  4396,  4397,  4398,  4399,  4400,  4401,  4402,  4404,  4409,
    4414,  4420,  4421,  4423,  4428,  4433,  4438,  4443,  4459,  4460,
    4462,  4467,  4472,  4476,  4488,  4501,  4511,  4516,  4521,  4526,
    4540,  4554,  4555,  4557,  4567,  4569,  4574,  4581,  4588,  4590,
    4592,  4593,  4595,  4599,  4604,  4605,  4607,  4613,  4615,  4617,
    4621,  4626,  4638
};
#endif

#if XPATHDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "$undefined", "\"<string literal>\"",
  "\"<non-boundary text node>\"", "\"<string literal(XPath 2.0)>\"",
  "\"QName\"", "\"NCName\"", "\"ClarkName\"", "T_ANY_LOCAL_NAME",
  "T_ANY_PREFIX", "\"<number literal>\"",
  "\"<number literal(XPath 2.0)>\"", "\"ancestor\"",
  "\"ancestor-or-self\"", "\"and\"", "\"'\"", "\"apply-template\"",
  "\"as\"", "\"ascending\"", "\":=\"", "\"at\"", "\"@\"", "\"attribute\"",
  "T_AVT", "\"|\"", "\"base-uri\"", "\"</\"", "\"boundary-space\"",
  "\"by\"", "\"call-template\"", "\"case\"", "\"castable\"", "\"cast\"",
  "\"child\"", "\"collation\"", "\"::\"", "\",\"", "\"comment\"",
  "\"<!--\"", "\"construction\"", "\"copy-namespaces\"", "\"{\"", "\"}\"",
  "\"declare\"", "\"default\"", "\"descendant\"", "\"descendant-or-self\"",
  "\"descending\"", "\"div\"", "\"document\"", "\"document-node\"",
  "\"$\"", "\".\"", "\"..\"", "\"element\"", "\"else\"", "\"empty\"",
  "\"empty-sequence\"", "\"encoding\"", "\"end_sort\"", "\"eq\"",
  "\"unknown keyword\"", "\"every\"", "\"except\"", "\"external\"",
  "\"following\"", "\"following-sibling\"", "\">>\"",
  "\"for-apply-template\"", "\"for\"", "\"function\"", "\"ge\"", "\"=\"",
  "\">=\"", "\">\"", "\"<=\"", "\"<\"", "\"!=\"", "\"greatest\"", "\"gt\"",
  "\"idiv\"", "\"if\"", "\"import\"", "\"inherit\"", "\"in\"",
  "\"instance\"", "\"intersect\"", "\"is\"", "\"item\"", "\"lax\"",
  "\"[\"", "\"least\"", "\"le\"", "\"let\"", "\"(\"", "\"lt\"", "\"map\"",
  "\"matches\"", "\"-\"", "\"mode\"", "\"mod\"", "\"module\"", "\"name\"",
  "\"namespace\"", "\"ne\"", "\"node\"", "\"no-inherit\"",
  "\"no-preserve\"", "\"of\"", "\"option\"", "\"ordered\"", "\"ordering\"",
  "\"order\"", "\"or\"", "\"parent\"", "\"<?\"", "\"+\"", "T_POSITION_SET",
  "\"#)\"", "\"(#\"", "\"<<\"", "\"preceding\"", "\"preceding-sibling\"",
  "\"preserve\"", "\"priority\"", "\"processing-instruction\"", "\"?\"",
  "\"/>\"", "\"\\\"\"", "\"]\"", "\"return\"", "\")\"", "\"satisfies\"",
  "\"schema-attribute\"", "\"schema-element\"", "\"schema\"", "\"self\"",
  "\";\"", "\"/\"", "\"//\"", "\"some\"", "\"sort\"", "\"stable\"",
  "\"*\"", "\"strict\"", "\"strip\"", "T_SUCCESS", "T_COMMENT_CONTENT",
  "T_PI_CONTENT", "T_PI_TARGET", "T_XSLT_VERSION", "\"template\"",
  "\"text\"", "\"then\"", "\"to\"", "\"treat\"", "\"tunnel\"",
  "\"typeswitch\"", "\"union\"", "\"unordered\"", "\"validate\"",
  "\"variable\"", "\"version\"", "\"where\"", "\"xquery\"", "\"internal\"",
  "\"internal-name\"", "\"current\"", "$accept", "Module", "VersionDecl",
  "Encoding", "MainModule", "LibraryModule", "ModuleDecl", "Prolog",
  "TemplateDecl", "$@1", "$@2", "OptionalPriority", "OptionalTemplateName",
  "TemplateName", "Setter", "Import", "Separator", "NamespaceDecl",
  "BoundarySpaceDecl", "BoundarySpacePolicy", "DefaultNamespaceDecl",
  "DeclareDefaultElementNamespace", "DeclareDefaultFunctionNamespace",
  "OptionDecl", "OrderingModeDecl", "OrderingMode", "EmptyOrderDecl",
  "OrderingEmptySequence", "CopyNamespacesDecl", "PreserveMode",
  "InheritMode", "DefaultCollationDecl", "BaseURIDecl", "SchemaImport",
  "SchemaPrefix", "ModuleImport", "ModuleNamespaceDecl", "FileLocations",
  "FileLocation", "VarDecl", "VariableValue", "OptionalDefaultValue",
  "ConstructionDecl", "ConstructionMode", "FunctionDecl", "@3",
  "ParamList", "Param", "FunctionBody", "EnclosedExpr", "QueryBody",
  "Pattern", "PathPattern", "IdKeyPattern", "RelativePathPattern",
  "PatternStep", "Expr", "ExpressionSequence", "ExprSingle",
  "OptionalModes", "OptionalMode", "Modes", "Mode", "FLWORExpr",
  "ForClause", "@4", "@5", "ForTail", "$@6", "@7", "PositionalVar",
  "LetClause", "@8", "LetTail", "@9", "WhereClause", "OrderByClause",
  "MandatoryOrderByClause", "OrderSpecList", "OrderSpec",
  "DirectionModifier", "EmptynessModifier", "CollationModifier",
  "OrderByInputOrder", "QuantifiedExpr", "SomeQuantificationExpr", "$@10",
  "@11", "SomeQuantificationTail", "@12", "@13", "EveryQuantificationExpr",
  "$@14", "@15", "EveryQuantificationTail", "@16", "@17",
  "SatisfiesClause", "TypeswitchExpr", "$@18", "CaseClause", "$@19",
  "$@20", "CaseTail", "CaseVariable", "CaseDefault", "$@21", "IfExpr",
  "OrExpr", "AndExpr", "ComparisonExpr", "RangeExpr", "AdditiveExpr",
  "AdditiveOperator", "MultiplicativeExpr", "MultiplyOperator",
  "UnionExpr", "IntersectExceptExpr", "UnionOperator", "IntersectOperator",
  "InstanceOfExpr", "TreatExpr", "CastableExpr", "CastExpr", "UnaryExpr",
  "UnaryOperator", "ValueExpr", "GeneralComp", "GeneralComparisonOperator",
  "ValueComp", "ValueComparisonOperator", "NodeComp", "NodeOperator",
  "ValidateExpr", "ValidationMode", "ExtensionExpr",
  "EnclosedOptionalExpr", "Pragmas", "Pragma", "PragmaContents",
  "PathExpr", "RelativePathExpr", "StepExpr", "@22", "$@23",
  "TemplateWithParameters", "$@24", "TemplateParameters",
  "OptionalTemplateParameters", "TemplateParameter", "IsTunnel",
  "OptionalAssign", "MapOrSlash", "FilteredAxisStep", "AxisStep",
  "ForwardStep", "$@25", "NodeTestInAxisStep", "Axis", "AxisToken",
  "AbbrevForwardStep", "$@26", "ReverseStep", "AbbrevReverseStep",
  "NodeTest", "NameTest", "WildCard", "FilterExpr", "PrimaryExpr",
  "Literal", "NumericLiteral", "VarRef", "VarName", "ParenthesizedExpr",
  "ContextItemExpr", "OrderingExpr", "FunctionCallExpr",
  "FunctionArguments", "Constructor", "DirectConstructor",
  "DirElemConstructor", "@27", "@28", "DirElemConstructorTail",
  "DirAttributeList", "Attribute", "DirAttributeValue", "AttrValueContent",
  "DirElemContent", "DirCommentConstructor", "DirPIConstructor",
  "ComputedConstructor", "CompDocConstructor", "CompElemConstructor",
  "$@29", "IsInternal", "CompAttrConstructor", "CompTextConstructor",
  "CompCommentConstructor", "CompPIConstructor", "CompAttributeName",
  "$@30", "$@31", "CompElementName", "CompNameExpr", "CompPIName",
  "CompNamespaceConstructor", "SingleType", "TypeDeclaration",
  "SequenceType", "OccurrenceIndicator", "ItemType", "AtomicType",
  "KindTest", "AnyKindTest", "DocumentTest", "AnyElementTest", "TextTest",
  "CommentTest", "PITest", "AnyAttributeTest", "AttributeTest",
  "SchemaAttributeTest", "ElementTest", "OptionalQuestionMark",
  "SchemaElementTest", "EmptyParanteses", "AttributeName", "ElementName",
  "TypeName", "FunctionName", "NCName", "LexicalName", "PragmaName",
  "URILiteral", "StringLiteral", "QName", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423
};
# endif

#define YYPACT_NINF -668

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-668)))

#define YYTABLE_NINF -463

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -63,   -28,   185,    86,   337,  -668,   117,  -668,  -668,  -668,
     734,  -668,  -668,   181,   253,   156,  -668,   213,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,   212,  -668,   -12,   230,   337,
     342,  -668,   -38,   189,   298,  -668,  -668,   188,   272,   353,
    -668,  -668,    71,   316,  -668,  -668,   318,   239,   276,   134,
     188,   900,  -668,   334,   282,  -668,  -668,   233,  -668,   367,
    -668,  -668,   133,   290,   295,  -668,  1730,  1730,   345,  -668,
    -668,   -38,   305,  -668,   -36,   396,   334,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,   334,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,   369,
     370,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,   307,
     389,  -668,   601,   173,    24,   -22,    32,  -668,   338,   267,
     393,   394,  1398,  -668,  -668,  -668,  -668,  -668,   334,  -668,
      59,  -668,  -668,   166,  -668,   339,  -668,  -668,  -668,   395,
    -668,  -668,  -668,  -668,  -668,  -668,   341,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,
     340,  -668,  -668,   347,   337,   291,   360,   493,   373,   349,
    1885,    64,  -668,   334,  -668,   226,   392,  -668,   358,  -668,
     304,   334,  -668,  -668,   188,   167,   174,   206,    21,   188,
     430,   342,   -53,   351,   188,   334,     6,  -668,  -668,  -668,
    -668,    79,   287,  -668,   353,   353,  -668,  -668,  -668,  1232,
     336,    18,   403,   344,  -668,   324,  1232,   334,  -668,   308,
    -668,   337,  -668,  -668,    23,  -668,   416,  -668,   342,   342,
     166,   166,   353,   334,   334,  -668,  1232,  -668,  -668,  -668,
    -668,  -668,  1232,  1232,  1398,  1398,  -668,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,  1398,  1398,  1398,  -668,  -668,  1398,  1398,  -668,  -668,
    -668,  -668,  1398,  -668,  -668,  1398,  -668,  -668,  1398,   352,
     447,   448,   449,  -668,  -668,  1066,  -668,  -668,  -668,  -668,
    -668,  1730,  1564,  1232,   108,  -668,  1232,  1232,  -668,  -668,
    -668,   337,   461,  -668,  -668,  -668,  -668,   282,   374,   378,
     282,  -668,  -668,  -668,     0,    51,  -668,  -668,   416,   342,
    -668,   226,   343,   226,  1232,  -668,  -668,   337,  -668,  -668,
     291,  -668,  -668,   291,  -668,  -668,   437,   337,   372,   376,
     421,    26,   408,   337,   291,   342,   384,    -1,   431,  -668,
     355,  -668,  -668,    52,    69,  -668,  -668,  -668,   466,   466,
    -668,   356,   482,   337,   435,   484,   337,   353,   485,  -668,
     453,  -668,  -668,   379,  -668,   365,   368,  -668,   371,   377,
     466,  -668,  -668,   380,  -668,  -668,   389,  -668,  -668,  -668,
    -668,   168,    24,   -22,    32,  -668,   456,   456,   342,   342,
    -668,   459,  -668,   191,  -668,   375,   404,  -668,  -668,  -668,
     383,   369,   370,   386,   291,  -668,   442,   388,    -6,   342,
    -668,   342,  -668,  -668,  -668,  -668,  -668,  -668,   465,   391,
     291,  -668,  -668,   157,   291,   337,   337,    16,   291,  -668,
     409,  -668,   348,   291,  -668,  -668,   415,    -6,   466,   353,
    -668,   342,  -668,   342,  -668,   416,   456,   440,   495,   239,
     381,   454,   507,   425,   457,   507,   466,   463,  -668,  -668,
    -668,  -668,  -668,  -668,   462,  -668,   282,   282,  -668,   121,
    -668,  -668,  -668,  -668,  -668,  -668,   412,  -668,  -668,   512,
     433,   417,  1232,  -668,  -668,  -668,  -668,   337,  -668,  -668,
     513,  -668,   497,  -668,   422,   423,  -668,  -668,  -668,  -668,
    -668,  -668,   291,  -668,   291,   291,  -668,  -668,  -668,   504,
     515,   188,  -668,  -668,    83,   416,   466,   432,   432,  -668,
    -668,  1232,   508,   476,   450,  -668,   492,  1232,  -668,   337,
     291,  -668,  -668,   291,   547,   566,  1232,   539,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,   543,  1730,    62,   536,
    -668,   419,   353,  -668,  -668,  -668,  -668,  -668,   353,    84,
    -668,  -668,   291,  1804,  -668,   291,    46,  -668,   445,   446,
    -668,   353,  1232,  -668,    33,   524,   550,  -668,  -668,  -668,
    1232,   515,  -668,   537,  -668,  -668,   528,  -668,  -668,   421,
    1232,  -668,   466,   466,   504,  -668,  1232,  -668,   404,  1899,
    1899,   567,  -668,   140,   148,  -668,   339,  -668,  -668,  1232,
    -668,   573,  -668,  -668,  -668,  -668,  -668,    92,   226,   226,
    -668,  1232,   337,  -668,  -668,   342,   456,  -668,  -668,   -23,
    -668,   574,  -668,  -668,   466,   552,   148,   148,  1804,   464,
    1899,  1899,  1899,  1899,  -668,  1232,   291,    11,  -668,  -668,
    -668,  -668,   582,   472,  -668,  -668,    10,    47,   584,  -668,
     337,   569,  -668,  1232,  -668,   234,  -668,  -668,   506,   148,
     148,  -668,  -668,  -668,  -668,   555,  1232,  -668,  -668,    63,
     250,  -668,  -668,   556,  1232,  -668,  -668,  -668,  -668,   479,
    -668,   559,  -668,  -668,  -668,   481,  -668,  1232,  -668,  -668,
     291,  -668,   373,   488,   353,  -668,   562,  -668,  -668,  -668,
    -668,  -668,   342,  -668,  -668,  -668,   353,   191,  1232,   353,
    1232,  -668,  -668,   578,  -668,   337,   521,   466,   353,   542,
     466,   487,  -668,   466,  -668,   373,  -668,   466,   534,   466,
    -668,   600,  1232,   538,   125,  -668,   416,  1232,   495,  1232,
    -668,  1232,    -2,  -668,  -668,  -668,   291,  -668,   544,  -668,
    -668,   342,  1232,  -668,  -668,  1232,    10,  -668,  -668,  -668,
      11,  -668,  -668,    47,   490,  -668,  -668,  -668,  1232,    63,
    -668,  -668
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,     0,    11,     0,     1,     0,     3,     2,    11,
       0,   469,   470,     6,     0,     9,   471,   457,   472,   328,
     329,   343,   342,   306,   305,   116,   317,   390,     0,     0,
       0,   308,   390,     0,     0,   310,   309,   390,     0,     0,
     349,   322,   390,     0,   311,   313,     0,     0,     0,     0,
     390,     0,   229,     0,     0,    49,   315,     0,   228,     0,
     312,   314,     0,     0,     0,   316,   265,     0,     0,   327,
     274,   390,     0,    50,   252,     0,     0,    16,    13,    15,
      14,    29,    12,    43,    44,    19,    33,     0,    34,    35,
      30,    31,    36,    37,    17,    32,    18,     8,    89,   105,
     104,   109,   122,   123,   110,   160,   161,   111,   112,   108,
     190,   192,   194,   198,   200,   204,   210,   212,   218,   220,
     222,   224,     0,   226,   196,   195,   197,   230,     0,   232,
       0,   259,   231,   266,   267,   271,   295,   297,   299,     0,
     301,   298,   321,   319,   323,   326,   272,   330,   332,   340,
     333,   334,   335,   337,   336,   338,   355,   357,   358,   359,
     356,   380,   381,   382,   383,   384,   385,   386,   324,   427,
     421,   426,   425,   424,   320,   438,   439,   422,   423,   325,
       0,   460,   341,   458,     0,     0,     0,     0,     0,     0,
       0,     0,   391,   396,   440,   370,     0,   457,     0,   458,
       0,     0,   434,   378,   390,     0,     0,     0,     0,   390,
       0,     0,     0,    26,   390,     0,     0,   429,   345,   344,
     346,     0,     0,   446,     0,     0,   465,   464,   360,     0,
      66,    62,     0,     0,   348,     0,     0,     0,   428,     0,
     466,   261,   467,   403,     0,   404,     0,   435,     0,     0,
     263,   264,     0,     0,     0,   433,     0,   254,   253,   463,
     273,   350,     0,     0,     0,     0,   241,   250,   243,   234,
     236,   237,   238,   239,   235,   244,   248,   245,   246,   242,
     249,     0,     0,     0,   203,   202,     0,     0,   207,   208,
     209,   206,     0,   215,   214,     0,   217,   216,     0,     0,
       0,     0,     0,   227,   251,     0,   255,   258,   294,   293,
     292,     0,     0,     0,     0,   304,     0,   352,     7,    38,
       5,     0,     0,   121,   117,   120,   280,     0,     0,     0,
       0,   318,   455,   454,     0,     0,   456,   402,     0,     0,
     399,   370,     0,   370,     0,   280,   394,     0,    42,    41,
       0,    79,    78,     0,    56,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   285,     0,   387,
       0,   431,   432,     0,     0,   388,   401,   400,   408,   408,
     365,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,   405,   379,     0,   262,     0,     0,   395,     0,     0,
     408,   275,   393,     0,   107,   106,   191,   193,   233,   240,
     247,   199,   201,   205,   211,   213,     0,     0,     0,     0,
     256,     0,   270,     0,   268,     0,     0,   300,   302,   303,
       0,   354,   353,     0,     0,   468,     0,     0,   282,     0,
     441,     0,   442,   392,   397,   371,   113,   372,     0,     0,
       0,    40,    77,     0,     0,     0,     0,     0,     0,   462,
       0,   461,     0,     0,    48,    28,     0,   282,   408,     0,
     430,     0,   447,     0,   448,     0,     0,     0,   133,   361,
       0,     0,    68,     0,     0,    68,   408,     0,    88,   260,
     436,   437,   445,   453,     0,   177,     0,     0,   219,   412,
     416,   417,   418,   420,   221,   223,   406,   225,   257,     0,
       0,     0,     0,   296,   331,   351,    10,     0,   339,   289,
     281,   283,     0,   459,     0,     0,   398,   276,   279,    60,
      57,    58,     0,    59,     0,     0,    53,    52,    51,    82,
     469,   390,    47,    21,     0,     0,   408,   451,   451,   389,
     409,     0,     0,     0,     0,   366,     0,     0,    67,     0,
       0,    63,    64,     0,     0,     0,     0,     0,   411,   419,
     413,   415,   414,   410,   407,   159,     0,     0,   150,   146,
     148,   288,     0,   444,   443,    54,    45,    46,     0,     0,
      83,   277,     0,     0,   286,     0,     0,   452,     0,     0,
     169,     0,     0,   365,     0,     0,    69,    70,    65,    61,
       0,     0,   162,   184,   178,   158,     0,   151,   152,   153,
       0,   284,   408,   408,     0,    80,     0,    39,   307,    93,
       0,    22,    90,    96,    92,   100,   103,    99,    20,     0,
      73,    75,   450,   449,   170,   134,   124,     0,   370,   370,
     367,     0,     0,   135,   163,     0,     0,   269,   154,   155,
     147,   290,    85,    84,   408,     0,    94,    95,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,   125,   373,
     363,   362,     0,     0,   189,    71,   144,     0,     0,   179,
       0,     0,   149,     0,   287,     0,   278,    91,   114,    97,
      98,   101,   102,    76,    72,     0,     0,   171,   175,   144,
       0,   369,   368,     0,     0,   140,   141,   136,   139,     0,
     145,     0,   164,   168,   185,     0,   156,     0,   291,    86,
       0,    87,     0,    24,     0,   176,     0,   131,   126,   132,
     130,   376,     0,   377,   374,   375,     0,   144,     0,     0,
       0,   157,    81,   115,   118,     0,   285,   408,     0,     0,
     408,     0,   142,   408,   180,     0,    25,   408,     0,   408,
     364,     0,     0,     0,     0,   119,     0,     0,   133,     0,
     143,     0,     0,   182,   181,   183,     0,   172,     0,   137,
     165,     0,     0,    23,   173,     0,   144,   166,   187,   186,
       0,   127,   138,     0,     0,   174,   128,   167,     0,   144,
     188,   129
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -668,  -668,  -668,  -668,  -668,  -668,  -668,   613,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -285,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,   418,  -668,     5,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,   142,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,     4,  -668,   -51,
    -668,  -668,   -35,  -668,  -397,  -340,   -47,   317,  -255,  -668,
    -668,  -668,  -641,  -668,  -619,  -668,  -668,  -174,  -668,  -668,
    -142,  -583,  -668,  -159,  -668,  -657,  -109,   216,  -668,    27,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -157,  -668,
    -668,  -668,  -668,  -668,  -152,  -668,  -668,  -667,  -668,  -668,
    -125,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,   387,
     385,   131,   366,  -668,   397,  -668,   361,   359,  -668,  -668,
     362,  -668,  -668,  -668,   535,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -245,  -668,   526,  -668,
    -668,   279,  -294,  -668,  -668,   313,  -668,   194,   -91,    85,
    -668,  -668,  -668,   -87,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,  -175,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,  -183,  -668,  -668,  -668,  -538,  -668,  -668,   -42,
    -668,  -668,  -668,  -668,    67,  -668,  -668,  -327,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,     3,  -668,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,   458,  -668,  -668,   252,  -341,  -412,
    -668,  -668,   -55,  -394,  -668,  -668,  -668,  -668,  -668,  -668,
    -304,  -668,  -668,   467,   124,   469,   -11,  -668,   -24,  -170,
     321,  -668,   639,  -668,  -308,    15,   -30
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,   185,     7,     8,     9,    10,    77,   593,
     669,   756,   366,   367,    78,    79,   320,    80,    81,   350,
      82,    83,    84,    85,    86,    87,    88,   458,    89,   356,
     532,    90,    91,    92,   386,    93,   383,   560,   606,    94,
     641,   676,    95,   353,    96,   664,   589,   590,   730,   341,
      97,   631,   632,   633,   634,   635,    98,    99,   100,   733,
     189,   753,   324,   101,   102,   678,   709,   738,   806,   809,
     553,   103,   686,   717,   796,   718,   719,   720,   579,   580,
     619,   659,   692,   512,   104,   105,   654,   687,   722,   797,
     803,   106,   644,   677,   707,   794,   800,   708,   107,   567,
     614,   725,   774,   784,   656,   785,   804,   108,   109,   110,
     111,   112,   113,   287,   114,   292,   115,   116,   295,   298,
     117,   118,   119,   120,   121,   122,   123,   124,   281,   125,
     282,   126,   283,   127,   128,   129,   306,   130,   131,   393,
     132,   133,   134,   253,   626,   437,   438,   520,   468,   521,
     522,   694,   312,   135,   136,   137,   314,   427,   138,   139,
     140,   190,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   219,   151,   152,   153,   154,   433,   155,   156,
     157,   380,   554,   681,   479,   555,   650,   342,   710,   158,
     159,   160,   161,   162,   475,   193,   163,   164,   165,   166,
     338,   339,   526,   375,   340,   246,   167,   505,   477,   498,
     573,   499,   500,   168,   169,   170,   370,   171,   172,   173,
     174,   175,   176,   177,   598,   178,   194,   335,   179,   524,
     180,   181,   556,   241,   541,   182,   183
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     199,   397,   237,   293,   235,   504,   198,   404,   405,   220,
     429,   245,   690,   434,   445,   331,   447,   422,   424,    13,
     723,   202,   501,   501,  -307,   260,    11,   217,    12,   242,
     395,   223,    16,   459,    18,   201,   261,   439,   478,   450,
     215,   378,   379,   238,   196,   222,  -288,   713,   705,   648,
     791,   247,   740,   232,   257,   637,   357,   200,    55,   494,
     255,   328,   432,   384,   550,   451,   639,   715,   452,   400,
      16,   332,    18,   288,   254,   482,   358,   304,   485,   464,
      46,   617,   501,   191,   721,    16,   197,    18,   441,   471,
     737,   754,   359,   443,   467,   536,   296,   -27,   226,   227,
     736,   305,     1,   716,    50,   289,   473,    73,   537,   258,
     618,   640,   502,   502,    16,   197,    18,    19,    20,   297,
     581,   624,   385,   509,   775,   290,   739,   545,   192,   792,
     637,   426,   440,    46,   360,     4,   723,   294,   333,   428,
     243,    64,   337,   691,   706,   564,   327,   534,   535,   516,
     346,   519,   740,   510,   192,   333,   613,    50,   325,    38,
     199,   336,   649,   328,   369,   529,   221,   679,   291,   533,
     782,   337,   502,   538,   714,   236,   509,   715,   542,    59,
     706,   199,   381,   442,   472,     5,   391,   363,     6,   390,
     737,   199,   199,    75,   220,   220,   333,   374,   377,   318,
     187,   474,   401,   402,   486,   596,   510,   347,   334,   403,
     343,   333,   361,   716,    54,   594,   625,   368,   199,   199,
     680,    14,   220,   373,   398,   399,   739,   714,   244,    11,
     549,    12,   666,   667,   329,   308,   230,   192,   570,    49,
     184,   530,    63,    64,   689,   226,   227,   585,   571,   586,
     587,   607,    69,    11,   741,    12,   394,   578,   421,   396,
     186,   330,   501,   309,   531,   572,   425,   284,   236,   430,
     231,   525,   284,   699,   700,   608,   236,   742,   609,   670,
     671,   661,   662,   616,   199,   285,   546,   672,   673,    33,
     285,   348,   236,    16,   197,    18,   600,   448,   351,   729,
     595,   547,   605,   548,   509,   310,   311,   627,  -462,   199,
     638,   612,   188,   349,   354,   444,   202,   223,   247,   255,
     352,   682,   683,   695,   204,   195,   205,    47,   286,   236,
     355,   461,   701,   702,   510,   199,   435,   203,   206,   207,
      11,   465,    12,   208,   685,   250,   251,   646,    16,   197,
      18,   540,   502,    12,   192,   653,   343,   220,   343,    16,
     218,    18,   435,   506,   506,   578,    57,   216,   224,   209,
     225,   229,   454,    16,   240,    18,   236,   200,   463,    16,
     323,    18,   726,   239,   674,   248,   199,   199,   199,   199,
     249,   704,   503,   503,   503,   503,   684,   252,   435,   622,
     256,   435,   210,   259,   265,   623,   262,   263,   211,   199,
     212,   199,   408,   409,   410,   523,   768,   523,   645,   771,
     703,   264,   773,   300,   299,   301,   776,   302,   778,   319,
     313,   315,   316,   321,   344,   317,   333,   362,   728,   220,
     382,   199,  -461,   199,   326,   752,   199,   523,   388,   523,
     213,   735,   503,   345,   365,   387,   389,   392,   305,   747,
     214,   416,    16,   197,    18,   417,   418,   419,   436,   221,
     435,   435,   751,   244,   453,   446,   455,   435,   457,   426,
     456,   462,   466,   469,   476,   568,   569,   470,   480,   481,
     483,   484,   487,   762,   327,   764,   488,   490,   489,   191,
     491,   793,   508,   492,   539,   513,   636,    38,   527,   493,
     543,   328,   495,   514,   496,   517,   552,   780,   515,   204,
     518,   205,   787,   528,   789,   551,   790,   558,   559,   561,
     562,   786,   435,   206,   207,   557,   565,   799,   208,   574,
     801,   575,   636,   636,   592,   497,   576,   566,   577,   582,
     581,   757,   220,   810,   583,   584,   588,   591,   220,   597,
     601,   602,    54,   760,   209,   604,   763,   610,   603,   611,
     613,   220,   615,   620,   435,   769,   519,   642,   643,   665,
     651,   636,   329,   636,   636,   636,   636,   652,   657,   655,
      63,    64,   668,   675,   693,   696,   698,   322,   711,   199,
     199,   712,   724,   211,   727,   212,   732,   734,   746,   330,
     748,   749,   750,   755,   758,   765,   467,   770,   772,   777,
     779,   808,    15,   781,   658,   199,   199,   563,   663,   795,
     364,   688,   503,   697,   431,   811,   788,   802,   761,   511,
     199,   199,   199,   199,   731,   213,   807,   660,   805,   783,
     407,   406,   411,   413,   414,   214,   307,   303,   449,   743,
     415,   544,   266,   343,   343,   767,   621,   435,   744,   267,
     647,   507,   599,   268,   269,   270,   271,   272,   273,   274,
     376,   275,   460,   371,   412,   372,   228,     0,     0,   276,
       0,     0,     0,     0,   277,     0,     0,   278,     0,     0,
       0,     0,   325,     0,   220,   435,   279,     0,     0,     0,
       0,     0,   199,     0,     0,     0,   220,     0,   759,   220,
       0,     0,   280,     0,     0,   745,     0,     0,   220,     0,
       0,     0,     0,     0,     0,   325,     0,    11,     0,    12,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
       0,    25,     0,     0,     0,     0,    26,    27,    28,     0,
      29,   199,     0,     0,    30,     0,     0,   798,    31,     0,
     766,     0,    32,    33,     0,     0,     0,     0,    34,     0,
      35,    36,     0,     0,    37,    38,    39,    40,    41,    42,
       0,     0,     0,     0,     0,     0,     0,    43,     0,     0,
      44,    45,     0,     0,    46,     0,     0,     0,     0,     0,
       0,    47,     0,     0,     0,     0,    48,    49,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
       0,     0,     0,    52,     0,     0,     0,     0,    53,     0,
      54,     0,     0,     0,     0,    55,     0,     0,     0,    56,
      57,    58,     0,     0,    59,     0,    60,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,    63,    64,
       0,    65,     0,    66,    67,    68,     0,     0,    69,     0,
       0,     0,     0,     0,     0,    70,     0,    71,     0,     0,
       0,     0,    72,     0,    73,    74,     0,     0,     0,     0,
       0,    75,    76,    11,     0,    12,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,     0,    25,     0,     0,
       0,     0,    26,    27,    28,     0,    29,     0,     0,     0,
      30,     0,     0,     0,    31,     0,     0,     0,    32,    33,
       0,     0,     0,     0,   233,     0,    35,    36,     0,     0,
      37,    38,    39,    40,    41,    42,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,    44,    45,     0,     0,
      46,     0,     0,     0,     0,     0,     0,    47,     0,     0,
       0,     0,    48,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,     0,     0,     0,    52,
       0,     0,     0,     0,    53,     0,    54,     0,     0,     0,
       0,    55,     0,     0,     0,    56,    57,    58,     0,     0,
      59,     0,    60,    61,     0,     0,    62,     0,     0,     0,
       0,     0,   234,     0,    63,    64,     0,    65,     0,    66,
      67,    68,     0,     0,    69,     0,     0,     0,     0,     0,
       0,    70,     0,    71,     0,     0,     0,     0,    72,     0,
      73,    74,     0,     0,     0,     0,     0,    75,    76,    11,
       0,    12,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,     0,    25,     0,     0,     0,     0,    26,    27,
      28,     0,    29,     0,     0,     0,    30,     0,     0,     0,
      31,     0,     0,     0,    32,    33,     0,     0,     0,   420,
     233,     0,    35,    36,     0,     0,    37,    38,    39,    40,
      41,    42,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,    44,    45,     0,     0,    46,     0,     0,     0,
       0,     0,     0,    47,     0,     0,     0,     0,    48,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,     0,     0,     0,    52,     0,     0,     0,     0,
      53,     0,    54,     0,     0,     0,     0,    55,     0,     0,
       0,    56,    57,    58,     0,     0,    59,     0,    60,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
      63,    64,     0,    65,     0,    66,    67,    68,     0,     0,
      69,     0,     0,     0,     0,     0,     0,    70,     0,    71,
       0,     0,     0,     0,    72,     0,    73,    74,     0,     0,
       0,     0,     0,    75,    76,    11,     0,    12,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,     0,    25,
       0,     0,     0,     0,    26,    27,    28,     0,    29,     0,
       0,     0,    30,     0,     0,     0,    31,     0,     0,     0,
      32,    33,     0,     0,     0,     0,   233,     0,    35,    36,
       0,     0,    37,    38,    39,    40,    41,    42,     0,     0,
       0,     0,     0,     0,     0,    43,     0,     0,    44,    45,
       0,     0,    46,     0,     0,     0,     0,     0,     0,    47,
       0,     0,     0,     0,    48,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,     0,     0,
       0,    52,     0,     0,     0,     0,    53,     0,    54,     0,
       0,     0,     0,    55,     0,     0,     0,    56,    57,    58,
       0,     0,    59,     0,    60,    61,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,    63,    64,     0,    65,
       0,    66,    67,    68,     0,     0,    69,     0,     0,     0,
       0,     0,     0,    70,     0,    71,     0,     0,     0,     0,
      72,     0,    73,    74,     0,     0,     0,     0,     0,    75,
      76,    11,     0,    12,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,     0,    25,     0,     0,     0,     0,
      26,    27,     0,     0,    29,     0,     0,     0,    30,     0,
       0,     0,    31,     0,     0,     0,    32,    33,     0,     0,
       0,     0,   233,     0,    35,    36,     0,     0,    37,    38,
      39,    40,    41,    42,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    44,    45,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,    52,     0,     0,
       0,     0,    53,     0,    54,     0,     0,     0,     0,    55,
       0,     0,     0,    56,    57,    58,     0,     0,    59,     0,
      60,    61,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,    63,    64,     0,    65,     0,    66,    67,     0,
       0,     0,    69,     0,     0,     0,     0,     0,     0,    70,
       0,    71,     0,     0,     0,     0,     0,     0,    73,    74,
       0,     0,     0,     0,     0,    75,    76,    11,     0,    12,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
       0,    25,     0,     0,     0,     0,    26,    27,     0,     0,
      29,     0,     0,     0,    30,     0,     0,     0,    31,     0,
       0,     0,    32,    33,     0,     0,     0,     0,   233,     0,
      35,    36,     0,     0,    37,    38,    39,    40,    41,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      44,    45,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    47,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
       0,     0,     0,     0,     0,     0,     0,     0,    53,     0,
      54,     0,     0,     0,     0,    55,     0,     0,     0,    56,
      57,     0,     0,     0,     0,     0,    60,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,    63,    64,
       0,    65,     0,     0,     0,     0,   423,     0,    69,     0,
       0,     0,     0,     0,     0,    70,     0,    71,     0,     0,
       0,     0,     0,     0,    73,     0,     0,     0,     0,     0,
       0,    75,    76,    11,     0,    12,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,     0,    25,     0,     0,
       0,     0,    26,    27,     0,     0,    29,     0,     0,     0,
      30,     0,     0,     0,    31,     0,     0,     0,    32,    33,
       0,     0,     0,     0,   233,     0,    35,    36,     0,     0,
      37,    38,    39,    40,    41,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    44,    45,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,     0,     0,
      16,    17,    18,    19,    20,     0,     0,    23,    24,     0,
       0,     0,     0,     0,     0,    51,    26,   628,     0,     0,
       0,     0,     0,     0,    53,     0,    54,     0,    31,     0,
       0,    55,   327,     0,     0,    56,    57,     0,     0,     0,
      35,    36,    60,    61,     0,    38,    62,     0,    41,   328,
       0,     0,     0,     0,    63,    64,     0,    65,     0,     0,
      44,    45,     0,     0,    69,     0,     0,     0,     0,     0,
       0,    70,     0,    71,     0,     0,     0,     0,     0,     0,
      73,    16,   197,    18,    19,    20,     0,    75,    76,     0,
       0,     0,     0,     0,     0,    16,   197,    18,    19,    20,
      54,     0,    23,    24,     0,     0,     0,     0,     0,    56,
       0,    26,   628,   327,     0,     0,    60,    61,     0,     0,
     329,     0,     0,    31,     0,     0,    38,   327,    63,    64,
     328,    65,     0,   629,   630,    35,    36,     0,    69,     0,
      38,     0,     0,    41,   328,     0,     0,   330,     0,     0,
       0,     0,     0,     0,     0,    44,    45,     0,     0,     0,
       0,    75,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,   329,     0,     0,    56,     0,     0,     0,     0,     0,
      64,    60,    61,     0,     0,   329,     0,     0,     0,    69,
       0,     0,     0,    63,    64,     0,    65,     0,   330,     0,
       0,     0,     0,    69,     0,     0,     0,     0,     0,     0,
       0,     0,   330
};

static const yytype_int16 yycheck[] =
{
      30,   246,    53,    25,    51,   417,    30,   262,   263,    39,
     314,    62,    35,   321,   341,   190,   343,   311,   312,     4,
     687,    32,   416,   417,    36,    76,     3,    38,     5,    59,
       7,    42,     6,     7,     8,    32,    87,    37,   379,   347,
      37,   224,   225,    54,    29,    42,    52,    37,    37,    16,
      52,    62,   709,    50,    90,   593,    35,    95,   111,   400,
      71,    55,   317,    45,   476,   350,    20,   686,   353,   252,
       6,     7,     8,    49,    71,   383,    55,   128,   386,   364,
      70,    19,   476,    95,    37,     6,     7,     8,    37,    37,
     709,   732,    71,   338,    95,    79,    64,    98,     6,     7,
      37,    42,   165,   686,    94,    81,    37,   160,    92,   145,
      48,    65,   416,   417,     6,     7,     8,     9,    10,    87,
      37,    37,   104,   113,   765,   101,   709,   468,   166,   131,
     668,    23,   132,    70,   113,   163,   803,   159,   132,   314,
       7,   135,   193,   166,   133,   486,    38,   455,   456,   434,
     201,   157,   809,   143,   166,   132,    31,    94,   188,    51,
     190,   191,   129,    55,   215,   450,    95,    75,   144,   454,
      45,   222,   476,   458,   164,    42,   113,   796,   463,   120,
     133,   211,   229,   132,   132,     0,   237,   211,   102,   236,
     809,   221,   222,   167,   224,   225,   132,   221,   222,   184,
      44,   132,   253,   254,   387,   546,   143,   204,   144,   256,
     195,   132,   209,   796,   106,   132,   132,   214,   248,   249,
     128,   104,   252,   144,   248,   249,   809,   164,    95,     3,
     475,     5,   629,   630,   126,    69,   102,   166,   117,    83,
      59,    84,   134,   135,   656,     6,     7,   532,   127,   534,
     535,   559,   144,     3,     4,     5,   241,   512,   305,   244,
       7,   153,   656,    97,   107,   144,   313,    99,    42,   316,
     136,   441,    99,   670,   671,   560,    42,    27,   563,   139,
     140,   622,   623,   577,   314,   117,   469,   139,   140,    39,
     117,   124,    42,     6,     7,     8,   551,   344,   124,    65,
     545,   471,   557,   473,   113,   139,   140,   592,    95,   339,
     595,   566,   100,   146,   108,   339,   327,   328,   329,   330,
     146,   648,   649,   664,    26,    95,    28,    77,   155,    42,
     124,   361,   672,   673,   143,   365,   321,   148,    40,    41,
       3,   365,     5,    45,   652,    66,    67,   602,     6,     7,
       8,     3,   656,     5,   166,   610,   341,   387,   343,     6,
       7,     8,   347,   418,   419,   620,   116,    95,    52,    71,
      52,    95,   357,     6,     7,     8,    42,    95,   363,     6,
       7,     8,   690,   150,   639,    95,   416,   417,   418,   419,
      95,   676,   416,   417,   418,   419,   651,    52,   383,   582,
      95,   386,   104,     7,    15,   588,    37,    37,   110,   439,
     112,   441,   281,   282,   283,   439,   757,   441,   601,   760,
     675,   114,   763,   156,    86,    32,   767,    33,   769,   138,
      91,    36,    91,    73,    42,    95,   132,     7,   693,   469,
     104,   471,    95,   473,    95,   730,   476,   471,   104,   473,
     152,   706,   476,    95,   103,    52,   132,   149,    42,   714,
     162,   109,     6,     7,     8,    18,    18,    18,     7,    95,
     455,   456,   727,    95,    37,   132,   104,   462,    57,    23,
     104,    73,    98,    52,    18,   496,   497,   132,   132,     7,
      55,     7,     7,   748,    38,   750,    43,   132,   119,    95,
     132,   786,    43,   132,    95,   130,   593,    51,    43,   132,
      95,    55,   132,   130,    58,    73,    21,   772,   132,    26,
     132,    28,   777,   132,   779,    85,   781,    73,    21,   104,
      73,   776,   517,    40,    41,   154,    73,   792,    45,   127,
     795,    29,   629,   630,   541,    89,   113,    85,   131,    52,
      37,   734,   582,   808,   132,   132,    52,    42,   588,   127,
      52,    85,   106,   746,    71,    73,   749,    20,   118,     3,
      31,   601,    29,    37,   559,   758,   157,   132,   132,   626,
      56,   668,   126,   670,   671,   672,   673,    37,    60,    52,
     134,   135,    25,    20,    20,    43,   132,   104,    16,   629,
     630,   129,    18,   110,    35,   112,   100,    52,    52,   153,
     131,    52,   131,   125,    52,    37,    95,    75,   131,    85,
      20,   131,     9,    85,   619,   655,   656,   485,   624,    85,
     212,   655,   656,   668,   317,   809,   778,   796,   747,   423,
     670,   671,   672,   673,   695,   152,   803,   620,   800,   774,
     265,   264,   286,   292,   295,   162,   130,   122,   345,   710,
     298,   467,    61,   648,   649,   756,   581,   652,   710,    68,
     603,   419,   548,    72,    73,    74,    75,    76,    77,    78,
     222,    80,   361,   216,   287,   216,    47,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,    -1,   732,    -1,   734,   690,   105,    -1,    -1,    -1,
      -1,    -1,   742,    -1,    -1,    -1,   746,    -1,   742,   749,
      -1,    -1,   121,    -1,    -1,   710,    -1,    -1,   758,    -1,
      -1,    -1,    -1,    -1,    -1,   765,    -1,     3,    -1,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    22,    23,    24,    -1,
      26,   791,    -1,    -1,    30,    -1,    -1,   791,    34,    -1,
     755,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,
      46,    47,    -1,    -1,    50,    51,    52,    53,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    82,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      -1,    -1,    -1,    99,    -1,    -1,    -1,    -1,   104,    -1,
     106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,   115,
     116,   117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
      -1,   137,    -1,   139,   140,   141,    -1,    -1,   144,    -1,
      -1,    -1,    -1,    -1,    -1,   151,    -1,   153,    -1,    -1,
      -1,    -1,   158,    -1,   160,   161,    -1,    -1,    -1,    -1,
      -1,   167,   168,     3,    -1,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    22,    23,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    34,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    44,    -1,    46,    47,    -1,    -1,
      50,    51,    52,    53,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    66,    67,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    99,
      -1,    -1,    -1,    -1,   104,    -1,   106,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,   115,   116,   117,    -1,    -1,
     120,    -1,   122,   123,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,   132,    -1,   134,   135,    -1,   137,    -1,   139,
     140,   141,    -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,   153,    -1,    -1,    -1,    -1,   158,    -1,
     160,   161,    -1,    -1,    -1,    -1,    -1,   167,   168,     3,
      -1,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    -1,    -1,    17,    -1,    -1,    -1,    -1,    22,    23,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    43,
      44,    -1,    46,    47,    -1,    -1,    50,    51,    52,    53,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    66,    67,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    82,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    -1,    -1,    -1,    99,    -1,    -1,    -1,    -1,
     104,    -1,   106,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,   115,   116,   117,    -1,    -1,   120,    -1,   122,   123,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,    -1,   139,   140,   141,    -1,    -1,
     144,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,   153,
      -1,    -1,    -1,    -1,   158,    -1,   160,   161,    -1,    -1,
      -1,    -1,    -1,   167,   168,     3,    -1,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    22,    23,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      38,    39,    -1,    -1,    -1,    -1,    44,    -1,    46,    47,
      -1,    -1,    50,    51,    52,    53,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,
      -1,    99,    -1,    -1,    -1,    -1,   104,    -1,   106,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    -1,   115,   116,   117,
      -1,    -1,   120,    -1,   122,   123,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
      -1,   139,   140,   141,    -1,    -1,   144,    -1,    -1,    -1,
      -1,    -1,    -1,   151,    -1,   153,    -1,    -1,    -1,    -1,
     158,    -1,   160,   161,    -1,    -1,    -1,    -1,    -1,   167,
     168,     3,    -1,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      22,    23,    -1,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    38,    39,    -1,    -1,
      -1,    -1,    44,    -1,    46,    47,    -1,    -1,    50,    51,
      52,    53,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,
      -1,    -1,   104,    -1,   106,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,   115,   116,   117,    -1,    -1,   120,    -1,
     122,   123,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,   137,    -1,   139,   140,    -1,
      -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,   160,   161,
      -1,    -1,    -1,    -1,    -1,   167,   168,     3,    -1,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    22,    23,    -1,    -1,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,
      46,    47,    -1,    -1,    50,    51,    52,    53,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
     106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,   115,
     116,    -1,    -1,    -1,    -1,    -1,   122,   123,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
      -1,   137,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,
      -1,    -1,    -1,    -1,    -1,   151,    -1,   153,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,    -1,    -1,    -1,
      -1,   167,   168,     3,    -1,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    22,    23,    -1,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    34,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    44,    -1,    46,    47,    -1,    -1,
      50,    51,    52,    53,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
       6,     7,     8,     9,    10,    -1,    -1,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    22,    23,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,   106,    -1,    34,    -1,
      -1,   111,    38,    -1,    -1,   115,   116,    -1,    -1,    -1,
      46,    47,   122,   123,    -1,    51,   126,    -1,    54,    55,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,    -1,    -1,
      66,    67,    -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,
     160,     6,     7,     8,     9,    10,    -1,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,     6,     7,     8,     9,    10,
     106,    -1,    13,    14,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    22,    23,    38,    -1,    -1,   122,   123,    -1,    -1,
     126,    -1,    -1,    34,    -1,    -1,    51,    38,   134,   135,
      55,   137,    -1,   139,   140,    46,    47,    -1,   144,    -1,
      51,    -1,    -1,    54,    55,    -1,    -1,   153,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
     135,   122,   123,    -1,    -1,   126,    -1,    -1,    -1,   144,
      -1,    -1,    -1,   134,   135,    -1,   137,    -1,   153,    -1,
      -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   165,   170,   171,   163,     0,   102,   173,   174,   175,
     176,     3,     5,   404,   104,   176,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    17,    22,    23,    24,    26,
      30,    34,    38,    39,    44,    46,    47,    50,    51,    52,
      53,    54,    55,    63,    66,    67,    70,    77,    82,    83,
      94,    95,    99,   104,   106,   111,   115,   116,   117,   120,
     122,   123,   126,   134,   135,   137,   139,   140,   141,   144,
     151,   153,   158,   160,   161,   167,   168,   177,   183,   184,
     186,   187,   189,   190,   191,   192,   193,   194,   195,   197,
     200,   201,   202,   204,   208,   211,   213,   219,   225,   226,
     227,   232,   233,   240,   253,   254,   260,   267,   276,   277,
     278,   279,   280,   281,   283,   285,   286,   289,   290,   291,
     292,   293,   294,   295,   296,   298,   300,   302,   303,   304,
     306,   307,   309,   310,   311,   322,   323,   324,   327,   328,
     329,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   342,   343,   344,   345,   347,   348,   349,   358,   359,
     360,   361,   362,   365,   366,   367,   368,   375,   382,   383,
     384,   386,   387,   388,   389,   390,   391,   392,   394,   397,
     399,   400,   404,   405,    59,   172,     7,    44,   100,   229,
     330,    95,   166,   364,   395,    95,   404,     7,   397,   405,
      95,   364,   395,   148,    26,    28,    40,    41,    45,    71,
     104,   110,   112,   152,   162,   364,    95,   395,     7,   341,
     405,    95,   364,   395,    52,    52,     6,     7,   401,    95,
     102,   136,   364,    44,   132,   225,    42,   218,   395,   150,
       7,   402,   405,     7,    95,   218,   374,   395,    95,    95,
     310,   310,    52,   312,   364,   395,    95,    90,   145,     7,
     218,   218,    37,    37,   114,    15,    61,    68,    72,    73,
      74,    75,    76,    77,    78,    80,    88,    93,    96,   105,
     121,   297,   299,   301,    99,   117,   155,   282,    49,    81,
     101,   144,   284,    25,   159,   287,    64,    87,   288,    86,
     156,    32,    33,   293,   218,    42,   305,   307,    69,    97,
     139,   140,   321,    91,   325,    36,    91,    95,   404,   138,
     185,    73,   104,     7,   231,   405,    95,    38,    55,   126,
     153,   333,     7,   132,   144,   396,   405,   218,   369,   370,
     373,   218,   356,   404,    42,    95,   218,   364,   124,   146,
     188,   124,   146,   212,   108,   124,   198,    35,    55,    71,
     113,   364,     7,   397,   194,   103,   181,   182,   364,   218,
     385,   392,   394,   144,   397,   372,   373,   397,   341,   341,
     350,   225,   104,   205,    45,   104,   203,    52,   104,   132,
     225,   218,   149,   308,   404,     7,   404,   305,   397,   397,
     341,   218,   218,   225,   227,   227,   278,   279,   280,   280,
     280,   281,   283,   285,   286,   289,   109,    18,    18,    18,
      43,   225,   311,   142,   311,   225,    23,   326,   333,   389,
     225,   226,   227,   346,   403,   404,     7,   314,   315,    37,
     132,    37,   132,   305,   397,   356,   132,   356,   225,   314,
     403,   185,   185,    37,   404,   104,   104,    57,   196,     7,
     399,   405,    73,   404,   185,   397,    98,    95,   317,    52,
     132,    37,   132,    37,   132,   363,    18,   377,   377,   353,
     132,     7,   403,    55,     7,   403,   341,     7,    43,   119,
     132,   132,   132,   132,   377,   132,    58,    89,   378,   380,
     381,   382,   389,   397,   378,   376,   381,   376,    43,   113,
     143,   246,   252,   130,   130,   132,   185,    73,   132,   157,
     316,   318,   319,   397,   398,   398,   371,    43,   132,   185,
      84,   107,   199,   185,   403,   403,    79,    92,   185,    95,
       3,   403,   185,    95,   316,   377,   341,   398,   398,   305,
     378,    85,    21,   239,   351,   354,   401,   154,    73,    21,
     206,   104,    73,   206,   377,    73,    85,   268,   395,   395,
     117,   127,   144,   379,   127,    29,   113,   131,   227,   247,
     248,    37,    52,   132,   132,   185,   185,   185,    52,   215,
     216,    42,   364,   178,   132,   305,   377,   127,   393,   393,
     227,    52,    85,   118,    73,   227,   207,   403,   185,   185,
      20,     3,   227,    31,   269,    29,   311,    19,    48,   249,
      37,   318,   341,   341,    37,   132,   313,   185,    23,   139,
     140,   220,   221,   222,   223,   224,   322,   345,   185,    20,
      65,   209,   132,   132,   261,   341,   227,   353,    16,   129,
     355,    56,    37,   227,   255,    52,   273,    60,   196,   250,
     248,   377,   377,   216,   214,   225,   223,   223,    25,   179,
     139,   140,   139,   140,   227,    20,   210,   262,   234,    75,
     128,   352,   356,   356,   227,   403,   241,   256,   397,   378,
      35,   166,   251,    20,   320,   377,    43,   221,   132,   223,
     223,   224,   224,   227,   185,    37,   133,   263,   266,   235,
     357,    16,   129,    37,   164,   233,   240,   242,   244,   245,
     246,    37,   257,   266,    18,   270,   403,    35,   227,    65,
     217,   218,   100,   228,    52,   227,    37,   233,   236,   240,
     244,     4,    27,   218,   348,   404,    52,   227,   131,    52,
     131,   227,   185,   230,   231,   125,   180,   341,    52,   397,
     341,   245,   227,   341,   227,    37,   404,   317,   377,   341,
      75,   377,   131,   377,   271,   231,   377,    85,   377,    20,
     227,    85,    45,   269,   272,   274,   305,   227,   239,   227,
     227,    52,   131,   185,   264,    85,   243,   258,   397,   227,
     265,   227,   242,   259,   275,   263,   237,   257,   131,   238,
     227,   236
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   169,   170,   170,   171,   171,   172,   172,   173,   174,
     175,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     177,   178,   179,   177,   180,   180,   181,   181,   182,   183,
     183,   183,   183,   183,   183,   183,   184,   184,   185,   186,
     187,   188,   188,   189,   189,   190,   191,   192,   193,   194,
     194,   195,   196,   196,   197,   198,   198,   199,   199,   200,
     201,   202,   203,   203,   203,   204,   205,   205,   206,   206,
     207,   207,   208,   209,   209,   210,   210,   211,   212,   212,
     214,   213,   215,   215,   215,   216,   217,   217,   218,   219,
     220,   220,   221,   221,   221,   221,   221,   221,   221,   222,
     223,   223,   223,   224,   225,   225,   226,   226,   227,   227,
     227,   227,   227,   227,   228,   228,   229,   229,   230,   230,
     231,   231,   232,   232,   234,   235,   233,   237,   238,   236,
     236,   236,   236,   239,   239,   241,   240,   243,   242,   242,
     242,   242,   244,   244,   245,   245,   246,   247,   247,   248,
     249,   249,   249,   250,   250,   251,   251,   251,   252,   252,
     253,   253,   255,   256,   254,   258,   259,   257,   257,   261,
     262,   260,   264,   265,   263,   263,   266,   268,   267,   270,
     271,   269,   272,   272,   273,   273,   274,   275,   274,   276,
     277,   277,   278,   278,   279,   279,   279,   279,   280,   280,
     281,   281,   282,   282,   283,   283,   284,   284,   284,   284,
     285,   285,   286,   286,   287,   287,   288,   288,   289,   289,
     290,   290,   291,   291,   292,   292,   293,   293,   294,   294,
     295,   295,   295,   296,   297,   297,   297,   297,   297,   297,
     298,   299,   299,   299,   299,   299,   299,   300,   301,   301,
     301,   302,   303,   303,   303,   304,   305,   305,   306,   306,
     307,   308,   308,   309,   309,   309,   309,   310,   310,   310,
     310,   311,   311,   311,   312,   311,   311,   313,   311,   311,
     315,   314,   316,   316,   316,   317,   317,   318,   319,   319,
     320,   320,   321,   321,   321,   322,   322,   323,   323,   325,
     324,   324,   326,   326,   327,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   330,   329,   329,
     329,   331,   332,   333,   333,   334,   334,   335,   335,   335,
     336,   336,   337,   337,   337,   337,   337,   337,   337,   337,
     338,   338,   339,   339,   340,   341,   341,   342,   342,   343,
     344,   345,   346,   346,   346,   347,   347,   348,   348,   348,
     350,   351,   349,   352,   352,   353,   353,   354,   355,   355,
     356,   356,   356,   357,   357,   357,   357,   357,   358,   359,
     360,   360,   360,   360,   360,   360,   360,   361,   363,   362,
     364,   364,   365,   366,   367,   368,   370,   371,   369,   369,
     372,   372,   373,   374,   374,   375,   376,   376,   377,   377,
     378,   378,   379,   379,   379,   379,   380,   380,   380,   380,
     381,   382,   382,   382,   382,   382,   382,   382,   383,   384,
     384,   385,   385,   386,   387,   388,   388,   388,   389,   389,
     390,   390,   390,   390,   390,   391,   392,   392,   392,   392,
     392,   393,   393,   394,   395,   396,   396,   397,   397,   398,
     399,   399,   400,   400,   401,   401,   402,   402,   403,   404,
     404,   405,   405
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     0,     5,     0,     2,     2,     2,
       6,     0,     2,     2,     2,     2,     2,     2,     2,     2,
       7,     0,     0,    15,     0,     2,     0,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     7,
       4,     1,     1,     1,     1,     6,     6,     5,     4,     1,
       1,     5,     2,     2,     6,     1,     1,     1,     1,     5,
       5,     6,     0,     3,     3,     6,     0,     3,     0,     2,
       1,     3,     9,     1,     2,     0,     2,     4,     1,     1,
       0,    11,     0,     1,     3,     3,     1,     1,     3,     1,
       1,     3,     1,     1,     2,     2,     1,     3,     3,     1,
       1,     3,     3,     1,     1,     1,     3,     3,     1,     1,
       1,     1,     1,     4,     0,     2,     0,     2,     1,     3,
       1,     1,     1,     1,     0,     0,    10,     0,     0,    10,
       1,     1,     1,     0,     3,     0,     9,     0,     8,     1,
       1,     1,     3,     5,     0,     1,     2,     3,     1,     4,
       0,     1,     1,     0,     1,     0,     2,     3,     3,     2,
       1,     1,     0,     0,     9,     0,     0,     9,     1,     0,
       0,     9,     0,     0,     9,     1,     2,     0,     6,     0,
       0,     8,     1,     1,     0,     3,     3,     0,     6,     8,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     3,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     4,
       1,     4,     1,     4,     1,     4,     1,     2,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     2,     1,     2,     2,     2,     2,     3,     2,     1,
       4,     0,     1,     2,     2,     1,     1,     1,     3,     7,
       3,     1,     1,     2,     0,     3,     5,     0,     9,     5,
       0,     2,     0,     1,     3,     0,     3,     5,     0,     1,
       0,     2,     1,     1,     1,     1,     4,     1,     1,     0,
       3,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     1,     1,     1,     1,     1,     1,     1,     5,
       1,     1,     1,     1,     2,     1,     1,     3,     2,     1,
       2,     4,     0,     1,     1,     1,     1,     1,     1,     1,
       0,     0,     8,     1,     5,     0,     2,     3,     3,     3,
       0,     2,     2,     0,     2,     2,     2,     2,     2,     3,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     5,
       0,     1,     4,     3,     3,     3,     0,     0,     3,     1,
       1,     1,     1,     1,     1,     3,     1,     2,     0,     2,
       2,     2,     0,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       4,     1,     1,     2,     2,     2,     4,     4,     1,     1,
       2,     4,     4,     6,     6,     4,     2,     4,     4,     7,
       7,     0,     1,     4,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, parseInfo, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if XPATHDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined XPATHLTYPE_IS_TRIVIAL && XPATHLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, parseInfo); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, QT_PREPEND_NAMESPACE(QPatternist)::ParserContext *const parseInfo)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (parseInfo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, QT_PREPEND_NAMESPACE(QPatternist)::ParserContext *const parseInfo)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp, parseInfo);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, QT_PREPEND_NAMESPACE(QPatternist)::ParserContext *const parseInfo)
{
  unsigned long yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , parseInfo);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, parseInfo); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !XPATHDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !XPATHDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, QT_PREPEND_NAMESPACE(QPatternist)::ParserContext *const parseInfo)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (parseInfo);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (QT_PREPEND_NAMESPACE(QPatternist)::ParserContext *const parseInfo)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined XPATHLTYPE_IS_TRIVIAL && XPATHLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, &yylloc, parseInfo);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 5:
#line 1436 "querytransformparser.ypp" /* yacc.c:1652  */
    {

/* Suppress more compiler warnings about unused defines. */
#if    defined(YYNNTS)              \
    || defined(yyerrok)             \
    || defined(YYNSTATES)           \
    || defined(YYRHSLOC)            \
    || defined(YYRECOVERING)        \
    || defined(YYFAIL)              \
    || defined(YYERROR)             \
    || defined(YYNRULES)            \
    || defined(YYBACKUP)            \
    || defined(YYMAXDEPTH)          \
    || defined(yyclearin)           \
    || defined(YYERRCODE)           \
    || defined(YY_LOCATION_PRINT)   \
    || defined(YYLLOC_DEFAULT)
#endif

        if((yyvsp[-2].sval) != QLatin1String("1.0"))
        {
            const ReflectYYLTYPE ryy((yyloc), parseInfo);

            parseInfo->staticContext->error(QtXmlPatterns::tr("Version %1 is not supported. The supported "
                                               "XQuery version is 1.0.")
                                               .arg(formatData((yyvsp[-2].sval))),
                                            ReportContext::XQST0031, &ryy);
        }
    }
#line 3534 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 7:
#line 1468 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        const QRegExp encNameRegExp(QLatin1String("[A-Za-z][A-Za-z0-9._\\-]*"));

        if(!encNameRegExp.exactMatch((yyvsp[0].sval)))
        {
            parseInfo->staticContext->error(QtXmlPatterns::tr("The encoding %1 is invalid. "
                                               "It must contain Latin characters only, "
                                               "must not contain whitespace, and must match "
                                               "the regular expression %2.")
                                            .arg(formatKeyword((yyvsp[(2) - (2)].sval)),
                                                 formatExpression(encNameRegExp.pattern())),
                                            ReportContext::XQST0087, fromYYLTYPE((yyloc), parseInfo));
        }
    }
#line 3553 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 8:
#line 1484 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        /* In XSL-T, we can have dangling variable references, so resolve them
         * before we proceed with other steps, such as checking circularity. */
        if(parseInfo->isXSLT())
        {
            typedef QHash<QXmlName, Expression::Ptr> Hash;
            const Hash::const_iterator end(parseInfo->unresolvedVariableReferences.constEnd());

            for(Hash::const_iterator it(parseInfo->unresolvedVariableReferences.constBegin()); it != end; ++it)
            {
                const Expression::Ptr body(resolveVariable(it.key(), (yyloc), parseInfo, true)); // TODO source locations vaise
                Q_ASSERT(body);
                it.value()->as<UnresolvedVariableReference>()->bindTo(body);
            }
        }

        /* The UserFunction callsites aren't bound yet, so bind them(if possible!). */
        {
            const UserFunctionCallsite::List::const_iterator cend(parseInfo->userFunctionCallsites.constEnd());
            UserFunctionCallsite::List::const_iterator cit(parseInfo->userFunctionCallsites.constBegin());
            for(; cit != cend; ++cit) /* For each callsite. */
            {
                const UserFunctionCallsite::Ptr callsite(*cit);
                Q_ASSERT(callsite);
                const UserFunction::List::const_iterator end(parseInfo->userFunctions.constEnd());
                UserFunction::List::const_iterator it(parseInfo->userFunctions.constBegin());

                for(; it != end; ++it) /* For each UserFunction. */
                {
                    const FunctionSignature::Ptr sign((*it)->signature());
                    Q_ASSERT(sign);

                    if(callsite->isSignatureValid(sign))
                    {
                        callsite->setSource((*it),
                                            parseInfo->allocateCacheSlots((*it)->argumentDeclarations().count()));
                        break;
                    }
                }
                if(it == end)
                {
                    parseInfo->staticContext->error(QtXmlPatterns::tr("No function with signature %1 is available")
                                                       .arg(formatFunction(callsite)),
                                                    ReportContext::XPST0017, fromYYLTYPE((yyloc), parseInfo));
                }
            }
        }

        /* Mark callsites in UserFunction bodies as recursive, if they are. */
        {
            const UserFunction::List::const_iterator fend(parseInfo->userFunctions.constEnd());
            UserFunction::List::const_iterator fit(parseInfo->userFunctions.constBegin());
            for(; fit != fend; ++fit)
            {
                CallTargetDescription::List signList;
                signList.append((*fit)->signature());
                CallTargetDescription::checkCallsiteCircularity(signList, (*fit)->body());
            }
        }

        /* Now, check all global variables for circularity.  This is done
         * backwards because global variables are only in scope below them,
         * in XQuery. */
        {
            const VariableDeclaration::List::const_iterator start(parseInfo->declaredVariables.constBegin());
            VariableDeclaration::List::const_iterator it(parseInfo->declaredVariables.constEnd());

            while(it != start)
            {
                --it;
                if((*it)->type != VariableDeclaration::ExpressionVariable && (*it)->type != VariableDeclaration::GlobalVariable)
                    continue; /* We want to ignore 'external' variables. */

                FunctionSignature::List signList;
                checkVariableCircularity(*it, (*it)->expression(), (*it)->type, signList, parseInfo);
                ExpressionFactory::registerLastPath((*it)->expression());
                parseInfo->finalizePushedVariable(1, false); /* Warn if it's unused. */
            }
        }

        /* Generate code for doing initial template name calling. One problem
         * is that we compilation in the initial template name, since we throw away the
         * code if we don't have the requested template. */
        if(parseInfo->languageAccent == QXmlQuery::XSLT20
           && !parseInfo->initialTemplateName.isNull()
           && parseInfo->namedTemplates.contains(parseInfo->initialTemplateName))
        {
            parseInfo->queryBody = create(new CallTemplate(parseInfo->initialTemplateName,
                                                           WithParam::Hash()),
                                          (yyloc), parseInfo);
            parseInfo->templateCalls.append(parseInfo->queryBody);
            /* We just discard the template body that XSLTTokenizer generated. */
        }
        else
            parseInfo->queryBody = (yyvsp[0].expr);
    }
#line 3654 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 10:
#line 1584 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        // TODO add to namespace context
        parseInfo->moduleNamespace = parseInfo->staticContext->namePool()->allocateNamespace((yyvsp[-3].sval));
    }
#line 3663 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 12:
#line 1592 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc));
        if(parseInfo->hasSecondPrologPart)
            parseInfo->staticContext->error(QtXmlPatterns::tr("A default namespace declaration must occur before function, "
                                               "variable, and option declarations."), ReportContext::XPST0003, fromYYLTYPE((yyloc), parseInfo));
    }
#line 3674 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 13:
#line 1599 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(parseInfo->hasSecondPrologPart)
            parseInfo->staticContext->error(QtXmlPatterns::tr("A default namespace declaration must occur before function, "
                                               "variable, and option declarations."), ReportContext::XPST0003, fromYYLTYPE((yyloc), parseInfo));
    }
#line 3684 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 14:
#line 1605 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(parseInfo->hasSecondPrologPart)
            parseInfo->staticContext->error(QtXmlPatterns::tr("Namespace declarations must occur before function, "
                                               "variable, and option declarations."), ReportContext::XPST0003, fromYYLTYPE((yyloc), parseInfo));
    }
#line 3694 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 15:
#line 1611 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc));
        if(parseInfo->hasSecondPrologPart)
            parseInfo->staticContext->error(QtXmlPatterns::tr("Module imports must occur before function, "
                                               "variable, and option declarations."), ReportContext::XPST0003, fromYYLTYPE((yyloc), parseInfo));
    }
#line 3705 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 17:
#line 1621 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        parseInfo->hasSecondPrologPart = true;
    }
#line 3713 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 18:
#line 1625 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        parseInfo->hasSecondPrologPart = true;
    }
#line 3721 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 19:
#line 1629 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc));
        parseInfo->hasSecondPrologPart = true;
    }
#line 3730 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 20:
#line 1652 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        Template::Ptr temp(create(new Template(parseInfo->currentImportPrecedence, (yyvsp[-2].sequenceType)), (yyloc), parseInfo));

        registerNamedTemplate((yyvsp[-4].qName), typeCheckTemplateBody((yyvsp[-1].expr), (yyvsp[-2].sequenceType), parseInfo),
                              parseInfo, (yylsp[-6]), temp);
        temp->templateParameters = parseInfo->templateParameters;
        parseInfo->templateParametersHandled();
    }
#line 3743 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 21:
#line 1662 "querytransformparser.ypp" /* yacc.c:1652  */
    {
    parseInfo->isParsingPattern = true;
  }
#line 3751 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 22:
#line 1666 "querytransformparser.ypp" /* yacc.c:1652  */
    {
    parseInfo->isParsingPattern = false;
  }
#line 3759 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 23:
#line 1675 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        /* In this grammar branch, we're guaranteed to be a template rule, but
         * may also be a named template. */

        const ImportPrecedence ip = parseInfo->isFirstTemplate() ? 0 : parseInfo->currentImportPrecedence;
        Expression::Ptr pattern((yyvsp[-8].expr));
        const TemplatePattern::ID templateID = parseInfo->allocateTemplateID();

        Template::Ptr templ(create(new Template(ip, (yyvsp[-2].sequenceType)), (yyloc), parseInfo));
        templ->body = typeCheckTemplateBody((yyvsp[-1].expr), (yyvsp[-2].sequenceType), parseInfo);
        templ->templateParameters = parseInfo->templateParameters;
        parseInfo->templateParametersHandled();

        TemplatePattern::Vector ourPatterns;
        /* We do it as per 6.4 Conflict Resolution for Template Rules:
         *
         * "If the pattern contains multiple alternatives separated by |, then
         * the template rule is treated equivalently to a set of template
         * rules, one for each alternative. However, it is not an error if a
         * node matches more than one of the alternatives." */
        while(pattern->is(Expression::IDCombineNodes))
        {
            const Expression::List operands(pattern->operands());
            pattern = operands.first();

            loadPattern(operands.at(1), ourPatterns, templateID, (yyvsp[-4].enums.Double), templ);
        }

        loadPattern(pattern, ourPatterns, templateID, (yyvsp[-4].enums.Double), templ);

        if(!(yyvsp[-12].qName).isNull())
            registerNamedTemplate((yyvsp[-12].qName), (yyvsp[-1].expr), parseInfo, (yylsp[-14]), templ);

        /* Now, let's add it to all the relevant templates. */
        for(int i = 0; i < (yyvsp[-5].qNameVector).count(); ++i) /* For each mode. */
        {
            const QXmlName &modeName = (yyvsp[-5].qNameVector).at(i);

            if(modeName == QXmlName(StandardNamespaces::InternalXSLT, StandardLocalNames::all) && (yyvsp[-5].qNameVector).count() > 1)
            {
                parseInfo->staticContext->error(QtXmlPatterns::tr("The keyword %1 cannot occur with any other mode name.")
                                                                 .arg(formatKeyword(QLatin1String("#all"))),
                                                ReportContext::XTSE0530,
                                                fromYYLTYPE((yyloc), parseInfo));
            }

            /* For each pattern the template use. */
            const TemplateMode::Ptr mode(parseInfo->modeFor(modeName));
            for(int t = 0; t < ourPatterns.count(); ++t)
                mode->templatePatterns.append(ourPatterns.at(t));
        }
    }
#line 3816 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 24:
#line 1729 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.Double) = std::numeric_limits<xsDouble>::quiet_NaN();
    }
#line 3824 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 25:
#line 1734 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        const AtomicValue::Ptr val(Decimal::fromLexical((yyvsp[0].sval)));
        if(val->hasError())
        {
            parseInfo->staticContext->error(QtXmlPatterns::tr("The value of attribute %1 must be of type %2, which %3 isn't.")
                                                             .arg(formatKeyword(QLatin1String("priority")),
                                                                  formatType(parseInfo->staticContext->namePool(), BuiltinTypes::xsDecimal),
                                                                  formatData((yyvsp[0].sval))),
                                            ReportContext::XTSE0530,
                                            fromYYLTYPE((yyloc), parseInfo));
        }
        else
            (yyval.enums.Double) = val->as<Numeric>()->toDouble();
    }
#line 3843 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 26:
#line 1750 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.qName) = QXmlName();
    }
#line 3851 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 28:
#line 1756 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.qName) = (yyvsp[0].qName);
    }
#line 3859 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 30:
#line 1762 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc));
    }
#line 3867 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 32:
#line 1767 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc));
    }
#line 3875 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 33:
#line 1771 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc));
    }
#line 3883 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 34:
#line 1775 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc));
    }
#line 3891 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 39:
#line 1786 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(!(yyvsp[-1].enums.Bool))
            allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc));

        if((yyvsp[-4].sval) == QLatin1String("xmlns"))
        {
            parseInfo->staticContext->error(QtXmlPatterns::tr("It is not possible to redeclare prefix %1.")
                                               .arg(formatKeyword(QLatin1String("xmlns"))),
                                            ReportContext::XQST0070, fromYYLTYPE((yyloc), parseInfo));
        }
        else if ((yyvsp[-2].sval) == CommonNamespaces::XML || (yyvsp[-4].sval) == QLatin1String("xml"))
        {
             parseInfo->staticContext->error(QtXmlPatterns::tr(
                                            "The prefix %1 can not be bound. By default, it is already bound "
                                            "to the namespace %2.")
                                             .arg(formatKeyword("xml"))
                                             .arg(formatURI(CommonNamespaces::XML)),
                                             ReportContext::XQST0070,
                                             fromYYLTYPE((yyloc), parseInfo));
        }
        else if(parseInfo->declaredPrefixes.contains((yyvsp[-4].sval)))
        {
            /* This includes the case where the user has bound a default prefix(such
             * as 'local') and now tries to do it again. */
            parseInfo->staticContext->error(QtXmlPatterns::tr("Prefix %1 is already declared in the prolog.")
                                               .arg(formatKeyword((yyvsp[-4].sval))),
                                            ReportContext::XQST0033, fromYYLTYPE((yyloc), parseInfo));
        }
        else
        {
            parseInfo->declaredPrefixes.append((yyvsp[-4].sval));

            if((yyvsp[-2].sval).isEmpty())
            {
                parseInfo->staticContext->namespaceBindings()->addBinding(QXmlName(StandardNamespaces::UndeclarePrefix,
                                                                                   StandardLocalNames::empty,
                                                                                   parseInfo->staticContext->namePool()->allocatePrefix((yyvsp[-4].sval))));
            }
            else
            {
                parseInfo->staticContext->namespaceBindings()->addBinding(parseInfo->staticContext->namePool()->allocateBinding((yyvsp[-4].sval), (yyvsp[-2].sval)));
            }
        }
    }
#line 3940 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 40:
#line 1832 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(parseInfo->hasDeclaration(ParserContext::BoundarySpaceDecl))
        {
            parseInfo->staticContext->error(prologMessage("declare boundary-space"),
                                            ReportContext::XQST0068, fromYYLTYPE((yyloc), parseInfo));
        }
        else
        {
            parseInfo->staticContext->setBoundarySpacePolicy((yyvsp[-1].enums.boundarySpacePolicy));
            parseInfo->registerDeclaration(ParserContext::BoundarySpaceDecl);
        }
    }
#line 3957 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 41:
#line 1846 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.boundarySpacePolicy) = StaticContext::BSPStrip;
    }
#line 3965 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 42:
#line 1851 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.boundarySpacePolicy) = StaticContext::BSPPreserve;
    }
#line 3973 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 45:
#line 1860 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(parseInfo->hasDeclaration(ParserContext::DeclareDefaultElementNamespace))
        {
            parseInfo->staticContext->error(prologMessage("declare default element namespace"),
                                            ReportContext::XQST0066, fromYYLTYPE((yyloc), parseInfo));
        }
        else
        {
            parseInfo->staticContext->namespaceBindings()->addBinding(QXmlName(parseInfo->staticContext->namePool()->allocateNamespace((yyvsp[-1].sval)), StandardLocalNames::empty));
            parseInfo->registerDeclaration(ParserContext::DeclareDefaultElementNamespace);
        }
    }
#line 3990 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 46:
#line 1875 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(parseInfo->hasDeclaration(ParserContext::DeclareDefaultFunctionNamespace))
        {
            parseInfo->staticContext->error(prologMessage("declare default function namespace"),
                                            ReportContext::XQST0066, fromYYLTYPE((yyloc), parseInfo));
        }
        else
        {
            parseInfo->staticContext->setDefaultFunctionNamespace((yyvsp[-1].sval));
            parseInfo->registerDeclaration(ParserContext::DeclareDefaultFunctionNamespace);
        }
    }
#line 4007 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 47:
#line 1889 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if((yyvsp[-2].qName).prefix() == StandardPrefixes::empty)
        {
            parseInfo->staticContext->error(QtXmlPatterns::tr("The name of an option must have a prefix. "
                                               "There is no default namespace for options."),
                                            ReportContext::XPST0081, fromYYLTYPE((yyloc), parseInfo));
        }
    }
#line 4020 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 48:
#line 1899 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc));
        if(parseInfo->hasDeclaration(ParserContext::OrderingModeDecl))
        {
            parseInfo->staticContext->error(prologMessage("declare ordering"),
                                            ReportContext::XQST0065, fromYYLTYPE((yyloc), parseInfo));
        }
        else
        {
            parseInfo->registerDeclaration(ParserContext::OrderingModeDecl);
            parseInfo->staticContext->setOrderingMode((yyvsp[-1].enums.orderingMode));
        }
    }
#line 4038 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 49:
#line 1914 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.orderingMode) = StaticContext::Ordered;
    }
#line 4046 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 50:
#line 1918 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.orderingMode) = StaticContext::Unordered;
    }
#line 4054 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 51:
#line 1923 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(parseInfo->hasDeclaration(ParserContext::EmptyOrderDecl))
        {
            parseInfo->staticContext->error(prologMessage("declare default order"),
                                            ReportContext::XQST0069, fromYYLTYPE((yyloc), parseInfo));
        }
        else
        {
            parseInfo->registerDeclaration(ParserContext::EmptyOrderDecl);
            parseInfo->staticContext->setOrderingEmptySequence((yyvsp[-1].enums.orderingEmptySequence));
        }
    }
#line 4071 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 52:
#line 1937 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.orderingEmptySequence) = StaticContext::Least;
    }
#line 4079 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 53:
#line 1941 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.orderingEmptySequence) = StaticContext::Greatest;
    }
#line 4087 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 54:
#line 1947 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(parseInfo->hasDeclaration(ParserContext::CopyNamespacesDecl))
        {
            parseInfo->staticContext->error(prologMessage("declare copy-namespaces"),
                                            ReportContext::XQST0055, fromYYLTYPE((yyloc), parseInfo));
        }
        else
        {
            parseInfo->registerDeclaration(ParserContext::CopyNamespacesDecl);
        }
    }
#line 4103 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 55:
#line 1960 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        parseInfo->preserveNamespacesMode = true;
    }
#line 4111 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 56:
#line 1965 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        parseInfo->preserveNamespacesMode = false;
    }
#line 4119 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 57:
#line 1970 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        parseInfo->inheritNamespacesMode = true;
    }
#line 4127 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 58:
#line 1975 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        parseInfo->inheritNamespacesMode = false;
    }
#line 4135 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 59:
#line 1980 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(parseInfo->hasDeclaration(ParserContext::DefaultCollationDecl))
        {
            parseInfo->staticContext->error(prologMessage("declare default collation"),
                                            ReportContext::XQST0038, fromYYLTYPE((yyloc), parseInfo));
        }
        else
        {
            const QUrl coll(resolveAndCheckCollation<ReportContext::XQST0038>((yyvsp[-1].sval), parseInfo, (yyloc)));

            parseInfo->registerDeclaration(ParserContext::DefaultCollationDecl);
            parseInfo->staticContext->setDefaultCollation(coll);
        }
    }
#line 4154 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 60:
#line 1996 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XSLT20), parseInfo, (yyloc), (yyvsp[-2].enums.Bool));
        if(parseInfo->hasDeclaration(ParserContext::BaseURIDecl))
        {
            parseInfo->staticContext->error(prologMessage("declare base-uri"),
                                            ReportContext::XQST0032, fromYYLTYPE((yyloc), parseInfo));
        }
        else
        {
            parseInfo->registerDeclaration(ParserContext::BaseURIDecl);
            const ReflectYYLTYPE ryy((yyloc), parseInfo);

            QUrl toBeBase(AnyURI::toQUrl<ReportContext::XQST0046>((yyvsp[-1].sval), parseInfo->staticContext, &ryy));
            /* Now we're guaranteed that base is a valid lexical representation, but it can still be relative. */

            if(toBeBase.isRelative())
                toBeBase = parseInfo->staticContext->baseURI().resolved(toBeBase);

            parseInfo->staticContext->setBaseURI(toBeBase);
        }
    }
#line 4180 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 61:
#line 2019 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        parseInfo->staticContext->error(QtXmlPatterns::tr("The Schema Import feature is not supported, "
                                           "and therefore %1 declarations cannot occur.")
                                           .arg(formatKeyword("import schema")),
                                        ReportContext::XQST0009, fromYYLTYPE((yyloc), parseInfo));
    }
#line 4191 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 65:
#line 2031 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if((yyvsp[-2].sval).isEmpty())
        {
            parseInfo->staticContext->error(QtXmlPatterns::tr("The target namespace of a %1 cannot be empty.")
                                               .arg(formatKeyword("module import")),
                                           ReportContext::XQST0088, fromYYLTYPE((yyloc), parseInfo));

        }
        else
        {
            /* This is temporary until we have implemented it. */
            parseInfo->staticContext->error(QtXmlPatterns::tr("The module import feature is not supported"),
                                            ReportContext::XQST0016, fromYYLTYPE((yyloc), parseInfo));
        }
    }
#line 4211 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 72:
#line 2058 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc), (yyvsp[-6].enums.Bool));
        if(variableByName((yyvsp[-4].qName), parseInfo))
        {
            parseInfo->staticContext->error(QtXmlPatterns::tr("A variable with name %1 has already "
                                                              "been declared.")
                                               .arg(formatKeyword(parseInfo->staticContext->namePool()->toLexical((yyvsp[-4].qName)))),
                                            parseInfo->isXSLT() ? ReportContext::XTSE0630 : ReportContext::XQST0049,
                                            fromYYLTYPE((yyloc), parseInfo));
        }
        else
        {
            if((yyvsp[-2].expr)) /* We got a value assigned. */
            {
                const Expression::Ptr checked
                        (TypeChecker::applyFunctionConversion((yyvsp[-2].expr), (yyvsp[-3].sequenceType), parseInfo->staticContext,
                                                              (yyvsp[-6].enums.Bool) ? ReportContext::XTTE0570 : ReportContext::XPTY0004,
                                                              (yyvsp[-6].enums.Bool) ? TypeChecker::Options(TypeChecker::CheckFocus | TypeChecker::AutomaticallyConvert) : TypeChecker::CheckFocus));

                pushVariable((yyvsp[-4].qName), (yyvsp[-3].sequenceType), checked, VariableDeclaration::GlobalVariable, (yyloc), parseInfo);
                parseInfo->declaredVariables.append(parseInfo->variables.last());
            }
            else /* We got an 'external' declaration. */
            {
                const SequenceType::Ptr varType(parseInfo->staticContext->
                                                externalVariableLoader()->announceExternalVariable((yyvsp[-4].qName), (yyvsp[-3].sequenceType)));

                if(varType)
                {
                    /* We push the declaration such that we can see name clashes and so on, but we don't use it for tying
                     * any references to it. */
                    pushVariable((yyvsp[-4].qName), varType, Expression::Ptr(), VariableDeclaration::ExternalVariable, (yyloc), parseInfo);
                }
                else if((yyvsp[-1].expr))
                {
                    /* Ok, the xsl:param got a default value, we make it
                     * available as a regular variable declaration. */
                    // TODO turn into checked
                    pushVariable((yyvsp[-4].qName), (yyvsp[-3].sequenceType), (yyvsp[-1].expr), VariableDeclaration::GlobalVariable, (yyloc), parseInfo);
                    // TODO ensure that duplicates are trapped.
                }
                else
                {
                    parseInfo->staticContext->error(QtXmlPatterns::tr("No value is available for the external "
                                                                      "variable with name %1.")
                                                       .arg(formatKeyword(parseInfo->staticContext->namePool(), (yyvsp[-4].qName))),
                                                    parseInfo->isXSLT() ? ReportContext::XTDE0050 : ReportContext::XPDY0002,
                                                    fromYYLTYPE((yyloc), parseInfo));
                }
            }
        }
    }
#line 4268 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 73:
#line 2112 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr).reset();
    }
#line 4276 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 74:
#line 2116 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 4284 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 75:
#line 2121 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr).reset();
    }
#line 4292 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 76:
#line 2125 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 4300 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 77:
#line 2130 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(parseInfo->hasDeclaration(ParserContext::ConstructionDecl))
        {
            parseInfo->staticContext->error(prologMessage("declare ordering"),
                                            ReportContext::XQST0067, fromYYLTYPE((yyloc), parseInfo));
        }
        else
        {
            parseInfo->registerDeclaration(ParserContext::ConstructionDecl);
            parseInfo->staticContext->setConstructionMode((yyvsp[-1].enums.constructionMode));
        }
    }
#line 4317 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 78:
#line 2144 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.constructionMode) = StaticContext::CMStrip;
    }
#line 4325 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 79:
#line 2148 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.constructionMode) = StaticContext::CMPreserve;
    }
#line 4333 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 80:
#line 2153 "querytransformparser.ypp" /* yacc.c:1652  */
    {
                (yyval.enums.slot) = parseInfo->currentExpressionSlot() - (yyvsp[-1].functionArguments).count();
              }
#line 4341 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 81:
#line 2157 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(!(yyvsp[-8].enums.Bool))
            allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc), (yyvsp[-8].enums.Bool));

        /* If FunctionBody is null, it is 'external', otherwise the value is the body. */
        const QXmlName::NamespaceCode ns((yyvsp[-7].qName).namespaceURI());

        if(parseInfo->isXSLT() && !(yyvsp[-7].qName).hasPrefix())
        {
            parseInfo->staticContext->error(QtXmlPatterns::tr("A stylesheet function must have a prefixed name."),
                                            ReportContext::XTSE0740,
                                            fromYYLTYPE((yyloc), parseInfo));
        }

        if((yyvsp[-1].expr)) /* We got a function body. */
        {
            if(ns == StandardNamespaces::empty)
            {
                parseInfo->staticContext->error(QtXmlPatterns::tr("The namespace for a user defined function "
                                                   "cannot be empty (try the predefined "
                                                   "prefix %1 which exists for cases "
                                                   "like this)")
                                                   .arg(formatKeyword("local")),
                                                ReportContext::XQST0060, fromYYLTYPE((yyloc), parseInfo));
            }
            else if(XPathHelper::isReservedNamespace(ns))
            {
                parseInfo->staticContext->error(QtXmlPatterns::tr(
                                                   "The namespace %1 is reserved; therefore "
                                                   "user defined functions may not use it. "
                                                   "Try the predefined prefix %2, which "
                                                   "exists for these cases.")
                                                .arg(formatURI(parseInfo->staticContext->namePool(), ns), formatKeyword("local")),
                                                parseInfo->isXSLT() ? ReportContext::XTSE0080 : ReportContext::XQST0045,
                                                fromYYLTYPE((yyloc), parseInfo));
            }
            else if(parseInfo->moduleNamespace != StandardNamespaces::empty &&
                    ns != parseInfo->moduleNamespace)
            {
                parseInfo->staticContext->error(QtXmlPatterns::tr(
                                                   "The namespace of a user defined "
                                                   "function in a library module must be "
                                                   "equivalent to the module namespace. "
                                                   "In other words, it should be %1 instead "
                                                   "of %2")
                                                .arg(formatURI(parseInfo->staticContext->namePool(), parseInfo->moduleNamespace),
                                                     formatURI(parseInfo->staticContext->namePool(), ns)),
                                                ReportContext::XQST0048, fromYYLTYPE((yyloc), parseInfo));
            }
            else
            {
                /* Apply function conversion such that the body matches the declared
                 * return type. */
                const Expression::Ptr checked(TypeChecker::applyFunctionConversion((yyvsp[-1].expr), (yyvsp[-2].sequenceType),
                                                                                   parseInfo->staticContext,
                                                                                   ReportContext::XPTY0004,
                                                                                   TypeChecker::Options(TypeChecker::AutomaticallyConvert |
                                                                                                        TypeChecker::CheckFocus |
                                                                                                        TypeChecker::GeneratePromotion)));

                const int argCount = (yyvsp[-5].functionArguments).count();
                const FunctionSignature::Ptr sign(new FunctionSignature((yyvsp[-7].qName) /* name */,
                                                                        argCount /* minArgs */,
                                                                        argCount /* maxArgs */,
                                                                        (yyvsp[-2].sequenceType) /* returnType */));
                sign->setArguments((yyvsp[-5].functionArguments));
                const UserFunction::List::const_iterator end(parseInfo->userFunctions.constEnd());
                UserFunction::List::const_iterator it(parseInfo->userFunctions.constBegin());

                for(; it != end; ++it)
                {
                    if(*(*it)->signature() == *sign)
                    {
                        parseInfo->staticContext->error(QtXmlPatterns::tr("A function already exists with "
                                                           "the signature %1.")
                                                           .arg(formatFunction(parseInfo->staticContext->namePool(), sign)),
                                                        parseInfo->isXSLT() ? ReportContext::XTSE0770 : ReportContext::XQST0034, fromYYLTYPE((yyloc), parseInfo));
                    }
                }

                VariableDeclaration::List argDecls;

                for(int i = 0; i < argCount; ++i)
                    argDecls.append(parseInfo->variables.at(i));

                if((yyvsp[-3].enums.slot) > -1)
                {
                    /* We have allocated slots, so now push them out of scope. */
                    parseInfo->finalizePushedVariable(argCount);
                }

                parseInfo->userFunctions.append(UserFunction::Ptr(new UserFunction(sign, checked, (yyvsp[-3].enums.slot), argDecls)));
            }
        }
        else /* We got an 'external' declaration. */
        {
            parseInfo->staticContext->error(QtXmlPatterns::tr("No external functions are supported. "
                                               "All supported functions can be used directly, "
                                               "without first declaring them as external"),
                                            ReportContext::XPST0017, fromYYLTYPE((yyloc), parseInfo));
        }
    }
#line 4448 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 82:
#line 2261 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.functionArguments) = FunctionArgument::List();
    }
#line 4456 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 83:
#line 2265 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        FunctionArgument::List l;
        l.append((yyvsp[0].functionArgument));
        (yyval.functionArguments) = l;
    }
#line 4466 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 84:
#line 2271 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        FunctionArgument::List::const_iterator it((yyvsp[-2].functionArguments).constBegin());
        const FunctionArgument::List::const_iterator end((yyvsp[-2].functionArguments).constEnd());

        for(; it != end; ++it)
        {
            if((*it)->name() == (yyvsp[0].functionArgument)->name())
            {
                parseInfo->staticContext->error(QtXmlPatterns::tr("An argument with name %1 has already "
                                                   "been declared. Every argument name "
                                                   "must be unique.")
                                                   .arg(formatKeyword(parseInfo->staticContext->namePool(), (yyvsp[0].functionArgument)->name())),
                                                ReportContext::XQST0039, fromYYLTYPE((yyloc), parseInfo));
            }
        }

        (yyvsp[-2].functionArguments).append((yyvsp[0].functionArgument));
        (yyval.functionArguments) = (yyvsp[-2].functionArguments);
    }
#line 4490 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 85:
#line 2292 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        pushVariable((yyvsp[-1].qName), (yyvsp[0].sequenceType), Expression::Ptr(), VariableDeclaration::FunctionArgument, (yyloc), parseInfo);
        (yyval.functionArgument) = FunctionArgument::Ptr(new FunctionArgument((yyvsp[-1].qName), (yyvsp[0].sequenceType)));
    }
#line 4499 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 86:
#line 2298 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr).reset();
    }
#line 4507 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 88:
#line 2304 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = (yyvsp[-1].expr);
    }
#line 4515 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 91:
#line 2320 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new CombineNodes((yyvsp[-2].expr), CombineNodes::Union, (yyvsp[0].expr)), (yyloc), parseInfo);
    }
#line 4523 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 93:
#line 2326 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        /* We write this into a node test. The spec says, 5.5.3 The Meaning of a Pattern:
         * "Similarly, / matches a document node, and only a document node,
         * because the result of the expression root(.)//(/) returns the root
         * node of the tree containing the context node if and only if it is a
         * document node." */
        (yyval.expr) = create(new AxisStep(QXmlNodeModelIndex::AxisSelf, BuiltinTypes::document), (yyloc), parseInfo);
    }
#line 4536 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 94:
#line 2335 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        /* /axis::node-test
         *       =>
         * axis::node-test[parent::document-node()]
         *
         * In practice it looks like this. $2 is:
         *
         *     TruthPredicate
         *          AxisStep    self::element(c)
         *          TruthPredicate
         *              AxisStep    parent::element(b)
         *              AxisStep    parent::element(a)
         *
         * and we want this:
         *
         *      TruthPredicate
         *          AxisStep    self::element(c)
         *          TruthPredicate
         *              AxisStep    self::element(b)
         *              TruthPredicate
         *                  AxisStep    parent::element(a)
         *                  AxisStep    parent::document()
         *
         * So we want to rewrite the predicate deepest down into a
         * another TruthPredicate containing the AxisStep.
         *
         * The simplest case where $2 is only an axis step is special. When $2 is:
         *
         *  AxisStep self::element(a)
         *
         * we want:
         *
         *  TruthPredicate
         *      AxisStep self::element(a)
         *      AxisStep parent::document()
         */

        /* First, find the target. */
        Expression::Ptr target((yyvsp[0].expr));

        while(isPredicate(target->id()))
        {
            const Expression::Ptr candidate(target->operands().at(1));

            if(isPredicate(candidate->id()))
                target = candidate;
            else
                break; /* target is now the last predicate. */
        }

        if(target->is(Expression::IDAxisStep))
        {
            (yyval.expr) = create(GenericPredicate::create((yyvsp[0].expr), create(new AxisStep(QXmlNodeModelIndex::AxisParent, BuiltinTypes::document), (yyloc), parseInfo),
                                                 parseInfo->staticContext, fromYYLTYPE((yylsp[-1]), parseInfo)), (yylsp[-1]), parseInfo);
        }
        else
        {
            const Expression::List targetOperands(target->operands());
            Expression::List newOps;
            newOps.append(targetOperands.at(0));

            newOps.append(create(GenericPredicate::create(targetOperands.at(1),
                                                          create(new AxisStep(QXmlNodeModelIndex::AxisParent, BuiltinTypes::document), (yyloc), parseInfo),
                                                          parseInfo->staticContext, fromYYLTYPE((yylsp[-1]), parseInfo)), (yylsp[-1]), parseInfo));

            target->setOperands(newOps);
            (yyval.expr) = (yyvsp[0].expr);
        }
    }
#line 4610 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 95:
#line 2405 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        /* //axis::node-test
         *        =>
         * axis::node-test[parent::node()]
         *
         * Spec says: "//para matches any para element that has a parent node."
         */
        (yyval.expr) = create(GenericPredicate::create((yyvsp[0].expr), create(new AxisStep(QXmlNodeModelIndex::AxisParent, BuiltinTypes::node), (yyloc), parseInfo),
                                             parseInfo->staticContext, fromYYLTYPE((yylsp[-1]), parseInfo)), (yylsp[-1]), parseInfo);
    }
#line 4625 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 97:
#line 2417 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        createIdPatternPath((yyvsp[-2].expr), (yyvsp[0].expr), QXmlNodeModelIndex::AxisParent, (yylsp[-1]), parseInfo);
    }
#line 4633 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 98:
#line 2421 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        createIdPatternPath((yyvsp[-2].expr), (yyvsp[0].expr), QXmlNodeModelIndex::AxisAncestor, (yylsp[-1]), parseInfo);
    }
#line 4641 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 99:
#line 2426 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        const Expression::List ands((yyvsp[0].expr)->operands());
        const FunctionSignature::Ptr signature((yyvsp[0].expr)->as<FunctionCall>()->signature());
        const QXmlName name(signature->name());
        const QXmlName key(StandardNamespaces::fn, StandardLocalNames::key);
        const QXmlName id(StandardNamespaces::fn, StandardLocalNames::id);

        if(name == id)
        {
            const Expression::ID id = ands.first()->id();
            if(!isVariableReference(id) && id != Expression::IDStringValue)
            {
                parseInfo->staticContext->error(QtXmlPatterns::tr("When function %1 is used for matching inside a pattern, "
                                                                  "the argument must be a variable reference or a string literal.")
                                                                  .arg(formatFunction(parseInfo->staticContext->namePool(), signature)),
                                                ReportContext::XPST0003,
                                                fromYYLTYPE((yyloc), parseInfo));
            }
        }
        else if(name == key)
        {
            if(ands.first()->id() != Expression::IDStringValue)
            {
                parseInfo->staticContext->error(QtXmlPatterns::tr("In an XSL-T pattern, the first argument to function %1 "
                                                                  "must be a string literal, when used for matching.")
                                                                  .arg(formatFunction(parseInfo->staticContext->namePool(), signature)),
                                                ReportContext::XPST0003,
                                                fromYYLTYPE((yyloc), parseInfo));
            }

            const Expression::ID id2 = ands.at(1)->id();
            if(!isVariableReference(id2) &&
               id2 != Expression::IDStringValue &&
               id2 != Expression::IDIntegerValue &&
               id2 != Expression::IDBooleanValue &&
               id2 != Expression::IDFloat)
            {
                parseInfo->staticContext->error(QtXmlPatterns::tr("In an XSL-T pattern, the first argument to function %1 "
                                                                  "must be a literal or a variable reference, when used for matching.")
                                                                  .arg(formatFunction(parseInfo->staticContext->namePool(), signature)),
                                                ReportContext::XPST0003,
                                                fromYYLTYPE((yyloc), parseInfo));
            }

            if(ands.count() == 3)
            {
                parseInfo->staticContext->error(QtXmlPatterns::tr("In an XSL-T pattern, function %1 cannot have a third argument.")
                                                                  .arg(formatFunction(parseInfo->staticContext->namePool(), signature)),
                                                ReportContext::XPST0003,
                                                fromYYLTYPE((yyloc), parseInfo));
            }

        }
        else
        {
            const FunctionSignature::Hash signs(parseInfo->staticContext->functionSignatures()->functionSignatures());
            parseInfo->staticContext->error(QtXmlPatterns::tr("In an XSL-T pattern, only function %1 "
                                                              "and %2, not %3, can be used for matching.")
                                                              .arg(formatFunction(parseInfo->staticContext->namePool(), signs.value(id)),
                                                                   formatFunction(parseInfo->staticContext->namePool(), signs.value(key)),
                                                                   formatFunction(parseInfo->staticContext->namePool(), signature)),
                                            ReportContext::XPST0003,
                                            fromYYLTYPE((yyloc), parseInfo));
        }

        (yyval.expr) = (yyvsp[0].expr);
    }
#line 4713 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 101:
#line 2496 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = createPatternPath((yyvsp[-2].expr), (yyvsp[0].expr), QXmlNodeModelIndex::AxisParent, (yylsp[-1]), parseInfo);
    }
#line 4721 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 102:
#line 2500 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = createPatternPath((yyvsp[-2].expr), (yyvsp[0].expr), QXmlNodeModelIndex::AxisAncestor, (yylsp[-1]), parseInfo);
    }
#line 4729 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 103:
#line 2505 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        const Expression::Ptr expr(findAxisStep((yyvsp[0].expr)));

        const QXmlNodeModelIndex::Axis axis = expr->as<AxisStep>()->axis();
        AxisStep *const axisStep = expr->as<AxisStep>();

        /* Here we constrain the possible axes, and we rewrite the axes as according
         * to 5.5.3 The Meaning of a Pattern.
         *
         * However, we also rewrite axis child and attribute to axis self. The
         * reason for this is that if we don't, we will match the children of
         * the context node, instead of the context node itself. The formal
         * definition of a pattern, root(.)//EE is insensitive to context,
         * while the way we implement pattern, "the other way of seeing it",
         * e.g from right to left, are very much. */

        if(axisStep->nodeTest() == BuiltinTypes::document
           || axis == QXmlNodeModelIndex::AxisChild)
            axisStep->setAxis(QXmlNodeModelIndex::AxisSelf);
        else if(axis == QXmlNodeModelIndex::AxisAttribute)
        {
            axisStep->setAxis(QXmlNodeModelIndex::AxisSelf);
            /* Consider that the user write attribute::node().  This is
             * semantically equivalent to attribute::attribute(), but since we have changed
             * the axis to axis self, we also need to change the node test, such that we
             * have self::attribute(). */
            if(*axisStep->nodeTest() == *BuiltinTypes::node)
                axisStep->setNodeTest(BuiltinTypes::attribute);
        }
        else
        {
            parseInfo->staticContext->error(QtXmlPatterns::tr("In an XSL-T pattern, axis %1 cannot be used, "
                                                              "only axis %2 or %3 can.")
                                            .arg(formatKeyword(AxisStep::axisName(axis)),
                                                 formatKeyword(AxisStep::axisName(QXmlNodeModelIndex::AxisChild)),
                                                 formatKeyword(AxisStep::axisName(QXmlNodeModelIndex::AxisAttribute))),
                                            ReportContext::XPST0003,
                                            fromYYLTYPE((yyloc), parseInfo));
        }

        (yyval.expr) = (yyvsp[0].expr);
    }
#line 4776 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 105:
#line 2550 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new ExpressionSequence((yyvsp[0].expressionList)), (yyloc), parseInfo);
    }
#line 4784 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 106:
#line 2555 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        Expression::List l;
        l.append((yyvsp[-2].expr));
        l.append((yyvsp[0].expr));
        (yyval.expressionList) = l;
    }
#line 4795 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 107:
#line 2562 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyvsp[-2].expressionList).append((yyvsp[0].expr));
        (yyval.expressionList) = (yyvsp[-2].expressionList);
    }
#line 4804 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 113:
#line 2573 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = createDirAttributeValue((yyvsp[-1].expressionList), parseInfo, (yyloc));
    }
#line 4812 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 114:
#line 2578 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        QVector<QXmlName> result;
        result.append(QXmlName(StandardNamespaces::InternalXSLT, StandardLocalNames::Default));
        (yyval.qNameVector) = result;
    }
#line 4822 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 115:
#line 2584 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.qNameVector) = (yyvsp[0].qNameVector);
    }
#line 4830 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 116:
#line 2589 "querytransformparser.ypp" /* yacc.c:1652  */
    {
            (yyval.qName) = QXmlName(StandardNamespaces::InternalXSLT, StandardLocalNames::Default);
    }
#line 4838 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 117:
#line 2593 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.qName) = (yyvsp[0].qName);
    }
#line 4846 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 118:
#line 2598 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        QVector<QXmlName> result;
        result.append((yyvsp[0].qName));
        (yyval.qNameVector) = result;
    }
#line 4856 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 119:
#line 2604 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyvsp[-2].qNameVector).append((yyvsp[0].qName));
        (yyval.qNameVector) = (yyvsp[-2].qNameVector);
    }
#line 4865 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 120:
#line 2610 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.qName) = (yyvsp[0].qName);
    }
#line 4873 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 121:
#line 2614 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if((yyvsp[0].sval) == QLatin1String("#current"))
            (yyval.qName) = QXmlName(StandardNamespaces::InternalXSLT, StandardLocalNames::current);
        else if((yyvsp[0].sval) == QLatin1String("#default"))
            (yyval.qName) = QXmlName(StandardNamespaces::InternalXSLT, StandardLocalNames::Default);
        else if((yyvsp[0].sval) == QLatin1String("#all"))
            (yyval.qName) = QXmlName(StandardNamespaces::InternalXSLT, StandardLocalNames::all);
        else
        {
            const ReflectYYLTYPE ryy((yyloc), parseInfo);

            if(!QXmlUtils::isNCName((yyvsp[0].sval)))
            {
                parseInfo->staticContext->error(QtXmlPatterns::tr("%1 is an invalid template mode name.")
                                                                  .arg(formatKeyword((yyvsp[0].sval))),
                                                ReportContext::XTSE0550,
                                                fromYYLTYPE((yyloc), parseInfo));
            }

            (yyval.qName) = parseInfo->staticContext->namePool()->allocateQName(StandardNamespaces::empty, (yyvsp[0].sval));
        }
    }
#line 4900 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 124:
#line 2643 "querytransformparser.ypp" /* yacc.c:1652  */
    {
               /* We're pushing the range variable here, not the positional. */
               (yyval.expr) = pushVariable((yyvsp[-4].qName), quantificationType((yyvsp[-3].sequenceType)), (yyvsp[0].expr), VariableDeclaration::RangeVariable, (yyloc), parseInfo);
           }
#line 4909 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 125:
#line 2647 "querytransformparser.ypp" /* yacc.c:1652  */
    {
               /* It is ok this appears after PositionalVar, because currentRangeSlot()
                * uses a different "channel" than currentPositionSlot(), so they can't trash
                * each other. */
               (yyval.enums.slot) = parseInfo->staticContext->currentRangeSlot();
           }
#line 4920 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 126:
#line 2654 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        Q_ASSERT((yyvsp[-3].expr));
        Q_ASSERT((yyvsp[0].expr));

        /* We want the next last pushed variable, since we push the range variable after the
         * positional variable. */
        if((yyvsp[-5].enums.slot) != -1 && parseInfo->variables.at(parseInfo->variables.count() -2)->name == (yyvsp[-7].qName))
        {
            /* Ok, a positional variable is used since its slot is not -1, and its name is equal
             * to our range variable. This is an error. */
            parseInfo->staticContext->error(QtXmlPatterns::tr("The name of a variable bound in a for-expression must be different "
                                               "from the positional variable. Hence, the two variables named %1 collide.")
                                               .arg(formatKeyword(parseInfo->staticContext->namePool(), (yyvsp[-7].qName))),
                                            ReportContext::XQST0089,
                                            fromYYLTYPE((yyloc), parseInfo));

        }

        const Expression::Ptr retBody(create(new ForClause((yyvsp[-1].enums.slot), (yyvsp[-2].expr), (yyvsp[0].expr), (yyvsp[-5].enums.slot)), (yyloc), parseInfo));
        ReturnOrderBy *const rob = locateReturnClause((yyvsp[0].expr));

        if(rob)
            (yyval.expr) = create(new OrderBy(rob->stability(), rob->orderSpecs(), retBody, rob), (yyloc), parseInfo);
        else
            (yyval.expr) = retBody;

        parseInfo->finalizePushedVariable();

        if((yyvsp[-5].enums.slot) != -1) /* We also have a positional variable to remove from the scope. */
            parseInfo->finalizePushedVariable();
    }
#line 4956 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 127:
#line 2688 "querytransformparser.ypp" /* yacc.c:1652  */
    {
             pushVariable((yyvsp[-4].qName), quantificationType((yyvsp[-3].sequenceType)), (yyvsp[0].expr), VariableDeclaration::RangeVariable, (yyloc), parseInfo);
         }
#line 4964 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 128:
#line 2691 "querytransformparser.ypp" /* yacc.c:1652  */
    {
             /* It is ok this appears after PositionalVar, because currentRangeSlot()
              * uses a different "channel" than currentPositionSlot(), so they can't trash
              * each other. */
             (yyval.enums.slot) = parseInfo->staticContext->currentRangeSlot();
         }
#line 4975 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 129:
#line 2698 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new ForClause((yyvsp[-1].enums.slot), (yyvsp[-3].expr), (yyvsp[0].expr), (yyvsp[-5].enums.slot)), (yyloc), parseInfo);

        parseInfo->finalizePushedVariable();

        if((yyvsp[-5].enums.slot) != -1) /* We also have a positional variable to remove from the scope. */
            parseInfo->finalizePushedVariable();
    }
#line 4988 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 133:
#line 2712 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.slot) = -1;
    }
#line 4996 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 134:
#line 2717 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        pushVariable((yyvsp[0].qName), CommonSequenceTypes::ExactlyOneInteger, Expression::Ptr(),
                     VariableDeclaration::PositionalVariable, (yyloc), parseInfo);
        (yyval.enums.slot) = parseInfo->currentPositionSlot();
    }
#line 5006 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 135:
#line 2724 "querytransformparser.ypp" /* yacc.c:1652  */
    {
                (yyval.expr) = pushVariable((yyvsp[-3].qName), quantificationType((yyvsp[-2].sequenceType)), (yyvsp[0].expr), VariableDeclaration::ExpressionVariable, (yyloc), parseInfo);
           }
#line 5014 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 136:
#line 2728 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc), (yyvsp[-7].enums.Bool));

        Q_ASSERT(parseInfo->variables.top()->name == (yyvsp[-5].qName));
        (yyval.expr) = create(new LetClause((yyvsp[-1].expr), (yyvsp[0].expr), parseInfo->variables.top()), (yyloc), parseInfo);
        parseInfo->finalizePushedVariable();
    }
#line 5026 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 137:
#line 2737 "querytransformparser.ypp" /* yacc.c:1652  */
    { (yyval.expr) = pushVariable((yyvsp[-3].qName), quantificationType((yyvsp[-2].sequenceType)), (yyvsp[0].expr), VariableDeclaration::ExpressionVariable, (yyloc), parseInfo);}
#line 5032 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 138:
#line 2739 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        Q_ASSERT(parseInfo->variables.top()->name == (yyvsp[-5].qName));
        (yyval.expr) = create(new LetClause((yyvsp[-1].expr), (yyvsp[0].expr), parseInfo->variables.top()), (yyloc), parseInfo);
        parseInfo->finalizePushedVariable();
    }
#line 5042 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 142:
#line 2750 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if((yyvsp[-2].orderSpecs).isEmpty())
            (yyval.expr) = (yyvsp[0].expr);
        else
            (yyval.expr) = createReturnOrderBy((yyvsp[-2].orderSpecs), (yyvsp[0].expr), parseInfo->orderStability.pop(), (yyloc), parseInfo);
    }
#line 5053 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 143:
#line 2758 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if((yyvsp[-2].orderSpecs).isEmpty())
            (yyval.expr) = create(new IfThenClause((yyvsp[-3].expr), (yyvsp[0].expr), create(new EmptySequence, (yyloc), parseInfo)), (yyloc), parseInfo);
        else
            (yyval.expr) = create(new IfThenClause((yyvsp[-3].expr), createReturnOrderBy((yyvsp[-2].orderSpecs), (yyvsp[0].expr), parseInfo->orderStability.pop(), (yyloc), parseInfo),
                                         create(new EmptySequence, (yyloc), parseInfo)),
                        (yyloc), parseInfo);
    }
#line 5066 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 144:
#line 2768 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.orderSpecs) = OrderSpecTransfer::List();
    }
#line 5074 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 146:
#line 2774 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.orderSpecs) = (yyvsp[0].orderSpecs);
    }
#line 5082 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 147:
#line 2779 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        OrderSpecTransfer::List list;
        list += (yyvsp[-2].orderSpecs);
        list.append((yyvsp[0].orderSpec));
        (yyval.orderSpecs) = list;
    }
#line 5093 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 148:
#line 2786 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        OrderSpecTransfer::List list;
        list.append((yyvsp[0].orderSpec));
        (yyval.orderSpecs) = list;
    }
#line 5103 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 149:
#line 2793 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.orderSpec) = OrderSpecTransfer((yyvsp[-3].expr), OrderBy::OrderSpec((yyvsp[-2].enums.sortDirection), (yyvsp[-1].enums.orderingEmptySequence)));
    }
#line 5111 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 150:
#line 2798 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        /* Where does the specification state the default value is ascending?
         *
         * It is implicit, in the first enumerated list in 3.8.3 Order By and Return Clauses:
         *
         * "If T1 and T2 are two tuples in the tuple stream, and V1 and V2 are the first pair
         *  of values encountered when evaluating their orderspecs from left to right for
         *  which one value is greater-than the other (as defined above), then:
         *
         *      1. If V1 is greater-than V2: If the orderspec specifies descending,
         *         then T1 precedes T2 in the tuple stream; otherwise, T2 precedes T1 in the tuple stream.
         *      2. If V2 is greater-than V1: If the orderspec specifies descending,
         *         then T2 precedes T1 in the tuple stream; otherwise, T1 precedes T2 in the tuple stream."
         *
         * which means that if you don't specify anything, or you
         * specify ascending, you get the same result.
         */
        (yyval.enums.sortDirection) = OrderBy::OrderSpec::Ascending;
    }
#line 5135 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 151:
#line 2819 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.sortDirection) = OrderBy::OrderSpec::Ascending;
    }
#line 5143 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 152:
#line 2824 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.sortDirection) = OrderBy::OrderSpec::Descending;
    }
#line 5151 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 153:
#line 2829 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.orderingEmptySequence) = parseInfo->staticContext->orderingEmptySequence();
    }
#line 5159 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 156:
#line 2836 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(parseInfo->isXSLT())
            resolveAndCheckCollation<ReportContext::XTDE1035>((yyvsp[0].sval), parseInfo, (yyloc));
        else
            resolveAndCheckCollation<ReportContext::XQST0076>((yyvsp[0].sval), parseInfo, (yyloc));
    }
#line 5170 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 157:
#line 2843 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        /* We do nothing. We don't use collations, and we have this non-terminal
         * in order to accept expressions. */
    }
#line 5179 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 158:
#line 2849 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        parseInfo->orderStability.push(OrderBy::StableOrder);
    }
#line 5187 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 159:
#line 2853 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        parseInfo->orderStability.push(OrderBy::UnstableOrder);
    }
#line 5195 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 162:
#line 2861 "querytransformparser.ypp" /* yacc.c:1652  */
    {
                            pushVariable((yyvsp[-3].qName), quantificationType((yyvsp[-2].sequenceType)), (yyvsp[0].expr),
                                         VariableDeclaration::RangeVariable, (yyloc), parseInfo);
                        }
#line 5204 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 163:
#line 2865 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.slot) = parseInfo->staticContext->currentRangeSlot();}
#line 5210 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 164:
#line 2867 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(new QuantifiedExpression((yyvsp[-1].enums.slot),
                                             QuantifiedExpression::Some, (yyvsp[-3].expr), (yyvsp[0].expr)), (yyloc), parseInfo);
        parseInfo->finalizePushedVariable();
    }
#line 5221 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 165:
#line 2875 "querytransformparser.ypp" /* yacc.c:1652  */
    {
                            (yyval.expr) = pushVariable((yyvsp[-3].qName), quantificationType((yyvsp[-2].sequenceType)), (yyvsp[0].expr),
                                                    VariableDeclaration::RangeVariable, (yyloc), parseInfo);
                        }
#line 5230 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 166:
#line 2879 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.slot) = parseInfo->staticContext->currentRangeSlot();}
#line 5236 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 167:
#line 2881 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new QuantifiedExpression((yyvsp[-1].enums.slot),
                                             QuantifiedExpression::Some, (yyvsp[-2].expr), (yyvsp[0].expr)), (yyloc), parseInfo);
        parseInfo->finalizePushedVariable();
    }
#line 5246 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 169:
#line 2890 "querytransformparser.ypp" /* yacc.c:1652  */
    {
                            pushVariable((yyvsp[-3].qName), quantificationType((yyvsp[-2].sequenceType)), (yyvsp[0].expr),
                                         VariableDeclaration::RangeVariable, (yyloc), parseInfo);
                         }
#line 5255 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 170:
#line 2894 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.slot) = parseInfo->staticContext->currentRangeSlot();}
#line 5261 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 171:
#line 2896 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(new QuantifiedExpression((yyvsp[-1].enums.slot),
                                             QuantifiedExpression::Every, (yyvsp[-3].expr), (yyvsp[0].expr)), (yyloc), parseInfo);
        parseInfo->finalizePushedVariable();
    }
#line 5272 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 172:
#line 2904 "querytransformparser.ypp" /* yacc.c:1652  */
    {
                            (yyval.expr) = pushVariable((yyvsp[-3].qName), quantificationType((yyvsp[-2].sequenceType)), (yyvsp[0].expr),
                                                    VariableDeclaration::RangeVariable, (yyloc), parseInfo);
                         }
#line 5281 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 173:
#line 2908 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.slot) = parseInfo->staticContext->currentRangeSlot();}
#line 5287 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 174:
#line 2910 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new QuantifiedExpression((yyvsp[-1].enums.slot),
                                             QuantifiedExpression::Every, (yyvsp[-2].expr), (yyvsp[0].expr)), (yyloc), parseInfo);
        parseInfo->finalizePushedVariable();
    }
#line 5297 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 176:
#line 2919 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 5305 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 177:
#line 2946 "querytransformparser.ypp" /* yacc.c:1652  */
    {
                    parseInfo->typeswitchSource.push((yyvsp[-1].expr));
                }
#line 5313 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 178:
#line 2950 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc));
        parseInfo->typeswitchSource.pop();
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 5323 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 179:
#line 2957 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(!(yyvsp[-1].qName).isNull())
        {
            pushVariable((yyvsp[-1].qName), (yyvsp[0].sequenceType), parseInfo->typeswitchSource.top(),
                         VariableDeclaration::ExpressionVariable, (yyloc), parseInfo, false);
        }
    }
#line 5335 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 180:
#line 2965 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        /* The variable shouldn't be in-scope for other case branches. */
        if(!(yyvsp[-4].qName).isNull())
            parseInfo->finalizePushedVariable();
    }
#line 5345 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 181:
#line 2971 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        const Expression::Ptr instanceOf(create(new InstanceOf(parseInfo->typeswitchSource.top(), (yyvsp[-5].sequenceType)), (yyloc), parseInfo));
        (yyval.expr) = create(new IfThenClause(instanceOf, (yyvsp[-2].expr), (yyvsp[0].expr)), (yyloc), parseInfo);
    }
#line 5354 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 184:
#line 2980 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.qName) = QXmlName();
    }
#line 5362 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 185:
#line 2985 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.qName) = (yyvsp[-1].qName);
    }
#line 5370 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 186:
#line 2990 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 5378 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 187:
#line 2994 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(!(yyvsp[0].qName).isNull())
        {
            pushVariable((yyvsp[0].qName), parseInfo->typeswitchSource.top()->staticType(),
                         parseInfo->typeswitchSource.top(),
                         VariableDeclaration::ExpressionVariable, (yyloc), parseInfo, false);
        }
    }
#line 5391 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 188:
#line 3003 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(!(yyvsp[-3].qName).isNull())
            parseInfo->finalizePushedVariable();
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 5401 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 189:
#line 3010 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(new IfThenClause((yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr)), (yyloc), parseInfo);
    }
#line 5410 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 191:
#line 3017 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(new OrExpression((yyvsp[-2].expr), (yyvsp[0].expr)), (yyloc), parseInfo);
    }
#line 5419 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 193:
#line 3024 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(new AndExpression((yyvsp[-2].expr), (yyvsp[0].expr)), (yyloc), parseInfo);
    }
#line 5428 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 199:
#line 3036 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(new RangeExpression((yyvsp[-2].expr), (yyvsp[0].expr)), (yyloc), parseInfo);
    }
#line 5437 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 201:
#line 3043 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(new ArithmeticExpression((yyvsp[-2].expr), (yyvsp[-1].enums.mathOperator), (yyvsp[0].expr)), (yyloc), parseInfo);
    }
#line 5446 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 202:
#line 3048 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.mathOperator) = AtomicMathematician::Add;}
#line 5452 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 203:
#line 3049 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.mathOperator) = AtomicMathematician::Substract;}
#line 5458 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 205:
#line 3053 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(new ArithmeticExpression((yyvsp[-2].expr), (yyvsp[-1].enums.mathOperator), (yyvsp[0].expr)), (yyloc), parseInfo);
    }
#line 5467 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 206:
#line 3058 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.mathOperator) = AtomicMathematician::Multiply;}
#line 5473 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 207:
#line 3059 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.mathOperator) = AtomicMathematician::Div;}
#line 5479 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 208:
#line 3060 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.mathOperator) = AtomicMathematician::IDiv;}
#line 5485 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 209:
#line 3061 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.mathOperator) = AtomicMathematician::Mod;}
#line 5491 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 211:
#line 3065 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10
                                 | QXmlQuery::XPath20
                                 | QXmlQuery::XmlSchema11IdentityConstraintField
                                 | QXmlQuery::XmlSchema11IdentityConstraintSelector),
                  parseInfo, (yyloc));
        (yyval.expr) = create(new CombineNodes((yyvsp[-2].expr), CombineNodes::Union, (yyvsp[0].expr)), (yyloc), parseInfo);
    }
#line 5504 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 213:
#line 3076 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(new CombineNodes((yyvsp[-2].expr), (yyvsp[-1].enums.combinedNodeOp), (yyvsp[0].expr)), (yyloc), parseInfo);
    }
#line 5513 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 216:
#line 3085 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.combinedNodeOp) = CombineNodes::Intersect;
    }
#line 5521 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 217:
#line 3089 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.combinedNodeOp) = CombineNodes::Except;
    }
#line 5529 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 219:
#line 3095 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(new InstanceOf((yyvsp[-3].expr),
                    SequenceType::Ptr((yyvsp[0].sequenceType))), (yyloc), parseInfo);
    }
#line 5539 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 221:
#line 3103 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(new TreatAs((yyvsp[-3].expr), (yyvsp[0].sequenceType)), (yyloc), parseInfo);
    }
#line 5548 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 223:
#line 3110 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(new CastableAs((yyvsp[-3].expr), (yyvsp[0].sequenceType)), (yyloc), parseInfo);
    }
#line 5557 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 225:
#line 3117 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(new CastAs((yyvsp[-3].expr), (yyvsp[0].sequenceType)), (yyloc), parseInfo);
    }
#line 5566 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 227:
#line 3124 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(new UnaryExpression((yyvsp[-1].enums.mathOperator), (yyvsp[0].expr), parseInfo->staticContext), (yyloc), parseInfo);
    }
#line 5575 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 228:
#line 3130 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.mathOperator) = AtomicMathematician::Add;
    }
#line 5583 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 229:
#line 3134 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.mathOperator) = AtomicMathematician::Substract;
    }
#line 5591 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 233:
#line 3143 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(new GeneralComparison((yyvsp[-2].expr), (yyvsp[-1].enums.valueOperator), (yyvsp[0].expr), parseInfo->isBackwardsCompat.top()), (yyloc), parseInfo);
    }
#line 5600 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 234:
#line 3148 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.valueOperator) = AtomicComparator::OperatorEqual;}
#line 5606 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 235:
#line 3149 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.valueOperator) = AtomicComparator::OperatorNotEqual;}
#line 5612 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 236:
#line 3150 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.valueOperator) = AtomicComparator::OperatorGreaterOrEqual;}
#line 5618 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 237:
#line 3151 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.valueOperator) = AtomicComparator::OperatorGreaterThan;}
#line 5624 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 238:
#line 3152 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.valueOperator) = AtomicComparator::OperatorLessOrEqual;}
#line 5630 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 239:
#line 3153 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.valueOperator) = AtomicComparator::OperatorLessThan;}
#line 5636 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 240:
#line 3156 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new ValueComparison((yyvsp[-2].expr), (yyvsp[-1].enums.valueOperator), (yyvsp[0].expr)), (yyloc), parseInfo);
    }
#line 5644 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 241:
#line 3160 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.valueOperator) = AtomicComparator::OperatorEqual;}
#line 5650 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 242:
#line 3161 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.valueOperator) = AtomicComparator::OperatorNotEqual;}
#line 5656 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 243:
#line 3162 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.valueOperator) = AtomicComparator::OperatorGreaterOrEqual;}
#line 5662 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 244:
#line 3163 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.valueOperator) = AtomicComparator::OperatorGreaterThan;}
#line 5668 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 245:
#line 3164 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.valueOperator) = AtomicComparator::OperatorLessOrEqual;}
#line 5674 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 246:
#line 3165 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.valueOperator) = AtomicComparator::OperatorLessThan;}
#line 5680 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 247:
#line 3168 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new NodeComparison((yyvsp[-2].expr), (yyvsp[-1].enums.nodeOperator), (yyvsp[0].expr)), (yyloc), parseInfo);
    }
#line 5688 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 248:
#line 3172 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.nodeOperator) = QXmlNodeModelIndex::Is;}
#line 5694 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 249:
#line 3173 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.nodeOperator) = QXmlNodeModelIndex::Precedes;}
#line 5700 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 250:
#line 3174 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.nodeOperator) = QXmlNodeModelIndex::Follows;}
#line 5706 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 251:
#line 3177 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc));
        parseInfo->staticContext->error(QtXmlPatterns::tr("The Schema Validation Feature is not supported. "
                                                          "Hence, %1-expressions may not be used.")
                                           .arg(formatKeyword("validate")),
                                        ReportContext::XQST0075, fromYYLTYPE((yyloc), parseInfo));
        /*
        $$ = Validate::create($2, $1, parseInfo->staticContext);
        */
    }
#line 5721 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 252:
#line 3190 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.validationMode) = Validate::Strict;}
#line 5727 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 253:
#line 3191 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.validationMode) = Validate::Strict;}
#line 5733 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 254:
#line 3192 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.validationMode) = Validate::Lax;}
#line 5739 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 255:
#line 3195 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc));
        /* We don't support any pragmas, so we only do the
         * necessary validation and use the fallback expression. */

        if((yyvsp[0].expr))
            (yyval.expr) = (yyvsp[0].expr);
        else
        {
            parseInfo->staticContext->error(QtXmlPatterns::tr("None of the pragma expressions are supported. "
                                               "Therefore, a fallback expression "
                                               "must be present"),
                                            ReportContext::XQST0079, fromYYLTYPE((yyloc), parseInfo));
        }
    }
#line 5759 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 256:
#line 3212 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr).reset();
    }
#line 5767 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 257:
#line 3216 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = (yyvsp[-1].expr);
    }
#line 5775 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 260:
#line 3224 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc));
    }
#line 5783 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 263:
#line 3232 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        /* This is "/step". That is, fn:root(self::node()) treat as document-node()/RelativePathExpr. */
        (yyval.expr) = create(new Path(createRootExpression(parseInfo, (yyloc)), (yyvsp[0].expr)), (yyloc), parseInfo);
    }
#line 5792 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 264:
#line 3238 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = createSlashSlashPath(createRootExpression(parseInfo, (yyloc)), (yyvsp[0].expr), (yyloc), parseInfo);
    }
#line 5800 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 265:
#line 3242 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        /* This is "/". That is, fn:root(self::node()) treat as document-node(). */
        (yyval.expr) = createRootExpression(parseInfo, (yyloc));
    }
#line 5809 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 268:
#line 3252 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new Path((yyvsp[-2].expr), (yyvsp[0].expr), (yyvsp[-1].enums.pathKind)), (yyloc), parseInfo);
    }
#line 5817 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 269:
#line 3256 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        const Expression::Ptr orderBy(createReturnOrderBy((yyvsp[-3].orderSpecs), (yyvsp[-1].expr), parseInfo->orderStability.pop(), (yyloc), parseInfo));

        ReturnOrderBy *const rob = orderBy->as<ReturnOrderBy>();
        const Expression::Ptr path(create(new Path((yyvsp[-6].expr), orderBy, (yyvsp[-5].enums.pathKind)), (yyloc), parseInfo));

        (yyval.expr) = create(new OrderBy(rob->stability(), rob->orderSpecs(), path, rob), (yyloc), parseInfo);
    }
#line 5830 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 270:
#line 3265 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = createSlashSlashPath((yyvsp[-2].expr), (yyvsp[0].expr), (yyloc), parseInfo);
    }
#line 5838 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 271:
#line 3270 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = NodeSortExpression::wrapAround((yyvsp[0].expr), parseInfo->staticContext);
    }
#line 5846 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 273:
#line 3275 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new CurrentItemStore((yyvsp[0].expr)), (yyloc), parseInfo);
    }
#line 5854 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 274:
#line 3279 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        const xsDouble version = (yyvsp[0].sval).toDouble();

        parseInfo->isBackwardsCompat.push(version != 2);

        (yyval.enums.Double) = version;
    }
#line 5866 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 275:
#line 3287 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if((yyvsp[-1].enums.Double) < 2)
            (yyval.expr) = createCompatStore((yyvsp[0].expr), (yyloc), parseInfo);
        else
            (yyval.expr) = (yyvsp[0].expr);
    }
#line 5877 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 276:
#line 3294 "querytransformparser.ypp" /* yacc.c:1652  */
    {
    allowedIn(QXmlQuery::XSLT20, parseInfo, (yyloc));
    Q_ASSERT(!(yyvsp[-3].sval).isEmpty());
    (yyval.expr) = create(new StaticBaseURIStore((yyvsp[-3].sval), (yyvsp[-1].expr)), (yyloc), parseInfo);
}
#line 5887 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 277:
#line 3301 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XSLT20), parseInfo, (yyloc));
        parseInfo->resolvers.push(parseInfo->staticContext->namespaceBindings());
        const NamespaceResolver::Ptr resolver(new DelegatingNamespaceResolver(parseInfo->staticContext->namespaceBindings()));
        resolver->addBinding(QXmlName(parseInfo->staticContext->namePool()->allocateNamespace((yyvsp[-1].sval)),
                                      StandardLocalNames::empty,
                                      parseInfo->staticContext->namePool()->allocatePrefix((yyvsp[-3].sval))));
        parseInfo->staticContext->setNamespaceBindings(resolver);
    }
#line 5901 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 278:
#line 3312 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        parseInfo->staticContext->setNamespaceBindings(parseInfo->resolvers.pop());
        (yyval.expr) = (yyvsp[-1].expr);
    }
#line 5910 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 279:
#line 3317 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new CallTemplate((yyvsp[-3].qName), parseInfo->templateWithParams), (yyloc), parseInfo);
        parseInfo->templateWithParametersHandled();
        parseInfo->templateCalls.append((yyval.expr));
    }
#line 5920 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 280:
#line 3324 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        parseInfo->startParsingWithParam();
    }
#line 5928 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 281:
#line 3328 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        parseInfo->endParsingWithParam();
    }
#line 5936 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 282:
#line 3333 "querytransformparser.ypp" /* yacc.c:1652  */
    {
    }
#line 5943 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 283:
#line 3336 "querytransformparser.ypp" /* yacc.c:1652  */
    {
    }
#line 5950 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 284:
#line 3339 "querytransformparser.ypp" /* yacc.c:1652  */
    {
    }
#line 5957 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 285:
#line 3343 "querytransformparser.ypp" /* yacc.c:1652  */
    {
    }
#line 5964 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 286:
#line 3346 "querytransformparser.ypp" /* yacc.c:1652  */
    {
    }
#line 5971 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 287:
#line 3350 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        /* Note, this grammar rule is invoked for @c xsl:param @em and @c
         * xsl:with-param. */
        const bool isParsingWithParam = parseInfo->isParsingWithParam();

        /**
         * @c xsl:param doesn't make life easy:
         *
         * If it only has @c name, it's default value is an empty
         * string(hence has type @c xs:string), but the value that
         * (maybe) is supplied can be anything, typically a node.
         *
         * Therefore, for that very common case we can't rely on
         * the Expression's type, but have to force it to item()*.
         *
         * So if we're supplied the type item()*, we pass a null
         * SequenceType. TemplateParameterReference recognizes this
         * and has item()* as its static type, regardless of if the
         * expression has a more specific type.
         */
        SequenceType::Ptr type;

        if(!(yyvsp[-1].sequenceType)->is(CommonSequenceTypes::ZeroOrMoreItems))
            type = (yyvsp[-1].sequenceType);

        Expression::Ptr expr;

        /* The default value is an empty sequence. */
        if(!(yyvsp[0].expr) && ((type && (yyvsp[-1].sequenceType)->cardinality().allowsEmpty())
                   || isParsingWithParam))
            expr = create(new EmptySequence, (yyloc), parseInfo);
        else
            expr = (yyvsp[0].expr);

        /* We ensure we have some type, so CallTemplate, Template and friends
         * are happy. */
        if(!isParsingWithParam && !type)
            type = CommonSequenceTypes::ZeroOrMoreItems;

        if((yyvsp[-4].enums.Bool))
            /* TODO, handle tunnel parameters. */;
        else
        {
            if((!isParsingWithParam && VariableDeclaration::contains(parseInfo->templateParameters, (yyvsp[-2].qName))) ||
               (isParsingWithParam && parseInfo->templateWithParams.contains((yyvsp[-2].qName))))
            {
                parseInfo->staticContext->error(QtXmlPatterns::tr("Each name of a template parameter must be unique; %1 is duplicated.")
                                                                 .arg(formatKeyword(parseInfo->staticContext->namePool(), (yyvsp[-2].qName))),
                                                isParsingWithParam ? ReportContext::XTSE0670 : ReportContext::XTSE0580, fromYYLTYPE((yyloc), parseInfo));
            }
            else
            {
                if(isParsingWithParam)
                    parseInfo->templateWithParams[(yyvsp[-2].qName)] = WithParam::Ptr(new WithParam((yyvsp[-2].qName), (yyvsp[-1].sequenceType), expr));
                else
                {
                    Q_ASSERT(type);
                    pushVariable((yyvsp[-2].qName), type, expr, VariableDeclaration::TemplateParameter, (yyloc), parseInfo);
                    parseInfo->templateParameters.append(parseInfo->variables.top());
                }
            }
        }
    }
#line 6039 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 288:
#line 3415 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.Bool) = false;
    }
#line 6047 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 289:
#line 3419 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.Bool) = true;
    }
#line 6055 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 290:
#line 3424 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = Expression::Ptr();
    }
#line 6063 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 291:
#line 3428 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 6071 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 292:
#line 3437 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.pathKind) = Path::RegularPath;
    }
#line 6079 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 293:
#line 3441 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.pathKind) = Path::XSLTForEach;
    }
#line 6087 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 294:
#line 3445 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.pathKind) = Path::ForApplyTemplate;
    }
#line 6095 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 296:
#line 3451 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(GenericPredicate::create((yyvsp[-3].expr), (yyvsp[-1].expr), parseInfo->staticContext, fromYYLTYPE((yyloc), parseInfo)), (yyloc), parseInfo);
    }
#line 6103 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 299:
#line 3459 "querytransformparser.ypp" /* yacc.c:1652  */
    {
                if((yyvsp[0].enums.axis) == QXmlNodeModelIndex::AxisAttribute)
                    parseInfo->nodeTestSource = BuiltinTypes::attribute;
             }
#line 6112 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 300:
#line 3464 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if((yyvsp[0].itemType))
        {
            /* A node test was explicitly specified. The un-abbreviated syntax was used. */
            (yyval.expr) = create(new AxisStep((yyvsp[-2].enums.axis), (yyvsp[0].itemType)), (yyloc), parseInfo);
        }
        else
        {
            /* Quote from 3.2.1.1 Axes
             *
             * [Definition: Every axis has a principal node kind. If an axis
             *  can contain elements, then the principal node kind is element;
             *  otherwise, it is the kind of nodes that the axis can contain.] Thus:
             * - For the attribute axis, the principal node kind is attribute.
             * - For all other axes, the principal node kind is element. */

            if((yyvsp[-2].enums.axis) == QXmlNodeModelIndex::AxisAttribute)
                (yyval.expr) = create(new AxisStep(QXmlNodeModelIndex::AxisAttribute, BuiltinTypes::attribute), (yyloc), parseInfo);
            else
                (yyval.expr) = create(new AxisStep((yyvsp[-2].enums.axis), BuiltinTypes::element), (yyloc), parseInfo);
        }

        parseInfo->restoreNodeTestSource();
    }
#line 6141 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 304:
#line 3494 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if((yyvsp[-1].enums.axis) == QXmlNodeModelIndex::AxisNamespace)
        {
            /* We don't raise XPST0010 here because the namespace axis isn't an optional
             * axis. It simply is not part of the XQuery grammar. */
            parseInfo->staticContext->error(QtXmlPatterns::tr("The %1-axis is unsupported in XQuery")
                                               .arg(formatKeyword("namespace")),
                                            ReportContext::XPST0003, fromYYLTYPE((yyloc), parseInfo));
        }
        else
            (yyval.enums.axis) = (yyvsp[-1].enums.axis);

        switch((yyvsp[-1].enums.axis))
        {
            case QXmlNodeModelIndex::AxisAttribute:
            {
                allowedIn(QueryLanguages(  QXmlQuery::XPath20
                                         | QXmlQuery::XQuery10
                                         | QXmlQuery::XmlSchema11IdentityConstraintField
                                         | QXmlQuery::XSLT20),
                          parseInfo, (yyloc));
                break;
            }
            case QXmlNodeModelIndex::AxisChild:
            {
                allowedIn(QueryLanguages(  QXmlQuery::XPath20
                                         | QXmlQuery::XQuery10
                                         | QXmlQuery::XmlSchema11IdentityConstraintField
                                         | QXmlQuery::XmlSchema11IdentityConstraintSelector
                                         | QXmlQuery::XSLT20),
                          parseInfo, (yyloc));
                break;
            }
            default:
            {
                allowedIn(QueryLanguages(  QXmlQuery::XPath20
                                         | QXmlQuery::XQuery10
                                         | QXmlQuery::XSLT20),
                          parseInfo, (yyloc));
            }
        }
    }
#line 6188 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 305:
#line 3537 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.axis) = QXmlNodeModelIndex::AxisAncestorOrSelf  ;}
#line 6194 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 306:
#line 3538 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.axis) = QXmlNodeModelIndex::AxisAncestor        ;}
#line 6200 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 307:
#line 3539 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.axis) = QXmlNodeModelIndex::AxisAttribute       ;}
#line 6206 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 308:
#line 3540 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.axis) = QXmlNodeModelIndex::AxisChild           ;}
#line 6212 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 309:
#line 3541 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.axis) = QXmlNodeModelIndex::AxisDescendantOrSelf;}
#line 6218 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 310:
#line 3542 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.axis) = QXmlNodeModelIndex::AxisDescendant      ;}
#line 6224 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 311:
#line 3543 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.axis) = QXmlNodeModelIndex::AxisFollowing       ;}
#line 6230 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 312:
#line 3544 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.axis) = QXmlNodeModelIndex::AxisPreceding       ;}
#line 6236 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 313:
#line 3545 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.axis) = QXmlNodeModelIndex::AxisFollowingSibling;}
#line 6242 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 314:
#line 3546 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.axis) = QXmlNodeModelIndex::AxisPrecedingSibling;}
#line 6248 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 315:
#line 3547 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.axis) = QXmlNodeModelIndex::AxisParent          ;}
#line 6254 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 316:
#line 3548 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.enums.axis) = QXmlNodeModelIndex::AxisSelf            ;}
#line 6260 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 317:
#line 3551 "querytransformparser.ypp" /* yacc.c:1652  */
    {
                        parseInfo->nodeTestSource = BuiltinTypes::attribute;
                   }
#line 6268 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 318:
#line 3555 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XSLT20 | QXmlQuery::XmlSchema11IdentityConstraintField), parseInfo, (yyloc));
        (yyval.expr) = create(new AxisStep(QXmlNodeModelIndex::AxisAttribute, (yyvsp[0].itemType)), (yyloc), parseInfo);

        parseInfo->restoreNodeTestSource();
    }
#line 6279 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 319:
#line 3562 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        ItemType::Ptr nodeTest;

        if(parseInfo->isParsingPattern && *(yyvsp[0].itemType) == *BuiltinTypes::node)
            nodeTest = BuiltinTypes::xsltNodeTest;
        else
            nodeTest = (yyvsp[0].itemType);

        (yyval.expr) = create(new AxisStep(QXmlNodeModelIndex::AxisChild, nodeTest), (yyloc), parseInfo);
    }
#line 6294 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 320:
#line 3573 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new AxisStep(QXmlNodeModelIndex::AxisAttribute, (yyvsp[0].itemType)), (yyloc), parseInfo);
    }
#line 6302 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 322:
#line 3580 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new AxisStep(QXmlNodeModelIndex::AxisParent, BuiltinTypes::node), (yyloc), parseInfo);
    }
#line 6310 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 324:
#line 3586 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
    }
#line 6318 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 325:
#line 3591 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.itemType) = QNameTest::create(parseInfo->nodeTestSource, (yyvsp[0].qName));
    }
#line 6326 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 327:
#line 3597 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.itemType) = parseInfo->nodeTestSource;
    }
#line 6334 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 328:
#line 3601 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        const NamePool::Ptr np(parseInfo->staticContext->namePool());
        const ReflectYYLTYPE ryy((yyloc), parseInfo);

        const QXmlName::NamespaceCode ns(QNameConstructor::namespaceForPrefix(np->allocatePrefix((yyvsp[0].sval)), parseInfo->staticContext, &ryy));

        (yyval.itemType) = NamespaceNameTest::create(parseInfo->nodeTestSource, ns);
    }
#line 6347 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 329:
#line 3610 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        const QXmlName::LocalNameCode c = parseInfo->staticContext->namePool()->allocateLocalName((yyvsp[0].sval));
        (yyval.itemType) = LocalNameTest::create(parseInfo->nodeTestSource, c);
    }
#line 6357 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 331:
#line 3618 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(GenericPredicate::create((yyvsp[-3].expr), (yyvsp[-1].expr), parseInfo->staticContext, fromYYLTYPE((yylsp[0]), parseInfo)), (yyloc), parseInfo);
    }
#line 6366 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 339:
#line 3631 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new ApplyTemplate(parseInfo->modeFor((yyvsp[-3].qName)),
                                      parseInfo->templateWithParams,
                                      parseInfo->modeFor(QXmlName(StandardNamespaces::InternalXSLT,
                                                                  StandardLocalNames::Default))),
                    (yylsp[-4]), parseInfo);
        parseInfo->templateWithParametersHandled();
    }
#line 6379 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 341:
#line 3642 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new Literal(AtomicString::fromValue((yyvsp[0].sval))), (yyloc), parseInfo);
    }
#line 6387 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 342:
#line 3647 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = createNumericLiteral<Double>((yyvsp[0].sval), (yyloc), parseInfo);
    }
#line 6396 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 343:
#line 3652 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = createNumericLiteral<Numeric>((yyvsp[0].sval), (yyloc), parseInfo);
    }
#line 6405 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 344:
#line 3658 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = resolveVariable((yyvsp[0].qName), (yyloc), parseInfo, false);
    }
#line 6414 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 345:
#line 3664 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        /* See: http://www.w3.org/TR/xpath20/#id-variables */
        (yyval.qName) = parseInfo->staticContext->namePool()->allocateQName(QString(), (yyvsp[0].sval));
    }
#line 6423 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 346:
#line 3669 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.qName) = (yyvsp[0].qName);
    }
#line 6431 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 347:
#line 3674 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = (yyvsp[-1].expr);
    }
#line 6440 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 348:
#line 3679 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        (yyval.expr) = create(new EmptySequence, (yyloc), parseInfo);
    }
#line 6449 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 349:
#line 3685 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new ContextItem(), (yyloc), parseInfo);
    }
#line 6457 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 350:
#line 3690 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 6465 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 351:
#line 3695 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
        if(XPathHelper::isReservedNamespace((yyvsp[-3].qName).namespaceURI()) || (yyvsp[-3].qName).namespaceURI() == StandardNamespaces::InternalXSLT)
        { /* We got a call to a builtin function. */
            const ReflectYYLTYPE ryy((yyloc), parseInfo);

            const Expression::Ptr
                func(parseInfo->staticContext->
                functionSignatures()->createFunctionCall((yyvsp[-3].qName), (yyvsp[-1].expressionList), parseInfo->staticContext, &ryy));

            if(func)
                (yyval.expr) = create(func, (yyloc), parseInfo);
            else
            {
                parseInfo->staticContext->error(QtXmlPatterns::tr("No function with name %1 is available.")
                                                   .arg(formatKeyword(parseInfo->staticContext->namePool(), (yyvsp[-3].qName))),
                                                ReportContext::XPST0017, fromYYLTYPE((yyloc), parseInfo));
            }
        }
        else /* It's a call to a function created with 'declare function'.*/
        {
            (yyval.expr) = create(new UserFunctionCallsite((yyvsp[-3].qName), (yyvsp[-1].expressionList).count()), (yyloc), parseInfo);

            (yyval.expr)->setOperands((yyvsp[-1].expressionList));
            parseInfo->userFunctionCallsites.append((yyval.expr));
        }
    }
#line 6497 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 352:
#line 3724 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expressionList) = Expression::List();
    }
#line 6505 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 353:
#line 3729 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        Expression::List list;
        list.append((yyvsp[0].expr));
        (yyval.expressionList) = list;
    }
#line 6515 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 355:
#line 3738 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc));
    }
#line 6523 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 360:
#line 3782 "querytransformparser.ypp" /* yacc.c:1652  */
    {
                        (yyval.enums.tokenizerPosition) = parseInfo->tokenizer->commenceScanOnly();
                        parseInfo->scanOnlyStack.push(true);
                    }
#line 6532 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 361:
#line 3791 "querytransformparser.ypp" /* yacc.c:1652  */
    {
                        ++parseInfo->elementConstructorDepth;
                        Expression::List constructors;

                        parseInfo->resolvers.push(parseInfo->staticContext->namespaceBindings());

                        /* Fix up attributes and namespace declarations. */
                        const NamespaceResolver::Ptr resolver(new DelegatingNamespaceResolver(parseInfo->staticContext->namespaceBindings()));
                        const NamePool::Ptr namePool(parseInfo->staticContext->namePool());
                        const int len = (yyvsp[0].attributeHolders).size();
                        QSet<QXmlName::PrefixCode> usedDeclarations;

                        /* Whether xmlns="" has been encountered. */
                        bool hasDefaultDeclaration = false;

                        /* For each attribute & namespace declaration, do: */
                        for(int i = 0; i < len; ++i)
                        {
                            QString strLocalName;
                            QString strPrefix;

                            XPathHelper::splitQName((yyvsp[0].attributeHolders).at(i).first, strPrefix, strLocalName);
                            const QXmlName::PrefixCode prefix = namePool->allocatePrefix(strPrefix);

                            /* This can seem a bit weird. However, this name is ending up in a QXmlName
                             * which consider its prefix a... prefix. So, a namespace binding name can in some cases
                             * be a local name, but that's just as the initial syntactical construct. */
                            const QXmlName::LocalNameCode localName = namePool->allocatePrefix(strLocalName);

                            /* Not that localName is "foo" in "xmlns:foo" and that prefix is "xmlns". */

                            if(prefix == StandardPrefixes::xmlns ||
                               (prefix == StandardPrefixes::empty && localName == StandardPrefixes::xmlns))
                            {
                                if(localName == StandardPrefixes::xmlns)
                                    hasDefaultDeclaration = true;

                                /* We have a namespace declaration. */

                                const Expression::Ptr nsExpr((yyvsp[0].attributeHolders).at(i).second);

                                const QString strNamespace(nsExpr->is(Expression::IDEmptySequence) ? QString() : nsExpr->as<Literal>()->item().stringValue());

                                const QXmlName::NamespaceCode ns = namePool->allocateNamespace(strNamespace);

                                if(ns == StandardNamespaces::empty)
                                {
                                    if(localName != StandardPrefixes::xmlns)
                                    {
                                        parseInfo->staticContext->error(QtXmlPatterns::tr("The namespace URI cannot be the empty string when binding to a prefix, %1.")
                                                                           .arg(formatURI(strPrefix)),
                                                                        ReportContext::XQST0085, fromYYLTYPE((yyloc), parseInfo));
                                    }
                                }
                                else if(!AnyURI::isValid(strNamespace))
                                {
                                    parseInfo->staticContext->error(QtXmlPatterns::tr("%1 is an invalid namespace URI.").arg(formatURI(strNamespace)),
                                                                    ReportContext::XQST0022, fromYYLTYPE((yyloc), parseInfo));
                                }

                                if(prefix == StandardPrefixes::xmlns && localName == StandardPrefixes::xmlns)
                                {
                                    parseInfo->staticContext->error(QtXmlPatterns::tr("It is not possible to bind to the prefix %1")
                                                                       .arg(formatKeyword("xmlns")),
                                                                    ReportContext::XQST0070, fromYYLTYPE((yyloc), parseInfo));
                                }

                                if(ns == StandardNamespaces::xml && localName != StandardPrefixes::xml)
                                {
                                    parseInfo->staticContext->error(QtXmlPatterns::tr("Namespace %1 can only be bound to %2 (and it is, in either case, pre-declared).")
                                                                       .arg(formatURI(namePool->stringForNamespace(StandardNamespaces::xml)))
                                                                       .arg(formatKeyword("xml")),
                                                                    ReportContext::XQST0070, fromYYLTYPE((yyloc), parseInfo));
                                }

                                if(localName == StandardPrefixes::xml && ns != StandardNamespaces::xml)
                                {
                                    parseInfo->staticContext->error(QtXmlPatterns::tr("Prefix %1 can only be bound to %2 (and it is, in either case, pre-declared).")
                                                                       .arg(formatKeyword("xml"))
                                                                       .arg(formatURI(namePool->stringForNamespace(StandardNamespaces::xml))),
                                                                    ReportContext::XQST0070, fromYYLTYPE((yyloc), parseInfo));
                                }

                                QXmlName nb;

                                if(localName == StandardPrefixes::xmlns)
                                    nb = QXmlName(ns, StandardLocalNames::empty);
                                else
                                    nb = QXmlName(ns, StandardLocalNames::empty, localName);

                                if(usedDeclarations.contains(nb.prefix()))
                                {
                                    parseInfo->staticContext->error(QtXmlPatterns::tr("Two namespace declaration attributes have the same name: %1.")
                                                                       .arg(formatKeyword(namePool->stringForPrefix(nb.prefix()))),
                                                                    ReportContext::XQST0071, fromYYLTYPE((yyloc), parseInfo));

                                }
                                else
                                    usedDeclarations.insert(nb.prefix());

                                /* If the user has bound the XML namespace correctly, we in either
                                 * case don't want to output it.
                                 *
                                 * We only have to check the namespace parts since the above checks has ensured
                                 * consistency in the prefix parts. */
                                if(ns != StandardNamespaces::xml)
                                {
                                    /* We don't want default namespace declarations when the
                                     * default namespace already is empty. */
                                    if(!(ns == StandardNamespaces::empty          &&
                                         localName == StandardNamespaces::xmlns   &&
                                         resolver->lookupNamespaceURI(StandardPrefixes::empty) == StandardNamespaces::empty))
                                    {
                                        constructors.append(create(new NamespaceConstructor(nb), (yyloc), parseInfo));
                                        resolver->addBinding(nb);
                                    }
                                }
                            }
                        }

                        if(parseInfo->elementConstructorDepth == 1 && !hasDefaultDeclaration)
                        {
                            /* TODO But mostly this isn't needed, since the default element
                             * namespace is empty? How does this at all work? */
                            const QXmlName def(resolver->lookupNamespaceURI(StandardPrefixes::empty), StandardLocalNames::empty);
                            constructors.append(create(new NamespaceConstructor(def), (yyloc), parseInfo));
                        }

                        parseInfo->staticContext->setNamespaceBindings(resolver);
                        (yyval.expressionList) = constructors;

                        /* Resolve the name of the element, now that the namespace attributes are read. */
                        {
                            const ReflectYYLTYPE ryy((yyloc), parseInfo);

                            const QXmlName ele = QNameConstructor::expandQName<StaticContext::Ptr,
                                                                               ReportContext::XPST0081,
                                                                               ReportContext::XPST0081>((yyvsp[-2].sval), parseInfo->staticContext, resolver, &ryy);
                            parseInfo->tagStack.push(ele);
                        }

                        parseInfo->tokenizer->resumeTokenizationFrom((yyvsp[-1].enums.tokenizerPosition));
                    }
#line 6680 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 362:
#line 3937 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        /* We add the content constructor after the attribute constructors. This might result
         * in nested ExpressionSequences, but it will be optimized away later on. */

        Expression::List attributes((yyvsp[-3].expressionList));
        const NamePool::Ptr namePool(parseInfo->staticContext->namePool());
        const int len = (yyvsp[-1].attributeHolders).size();
        QSet<QXmlName> declaredAttributes;
        declaredAttributes.reserve(len);

        /* For each namespace, resolve its name(now that we have resolved the namespace declarations) and
         * turn it into an attribute constructor. */
        for(int i = 0; i < len; ++i)
        {
            QString strLocalName;
            QString strPrefix;

            XPathHelper::splitQName((yyvsp[-1].attributeHolders).at(i).first, strPrefix, strLocalName);
            const QXmlName::PrefixCode prefix = namePool->allocatePrefix(strPrefix);
            const QXmlName::LocalNameCode localName = namePool->allocateLocalName(strLocalName);

            if(prefix == StandardPrefixes::xmlns ||
               (prefix == StandardPrefixes::empty && localName == StandardLocalNames::xmlns))
            {
                const Expression::ID id = (yyvsp[-1].attributeHolders).at(i).second->id();

                if(id == Expression::IDStringValue || id == Expression::IDEmptySequence)
                {
                    /* It's a namespace declaration, and we've already handled those above. */
                    continue;
                }
                else
                {
                    parseInfo->staticContext->error(QtXmlPatterns::tr("The namespace URI must be a constant and cannot "
                                                       "use enclosed expressions."),
                                                    ReportContext::XQST0022, fromYYLTYPE((yyloc), parseInfo));
                }

            }
            else
            {
                const ReflectYYLTYPE ryy((yyloc), parseInfo);
                const QXmlName att = QNameConstructor::expandQName<StaticContext::Ptr,
                                                                   ReportContext::XPST0081,
                                                                   ReportContext::XPST0081>((yyvsp[-1].attributeHolders).at(i).first, parseInfo->staticContext,
                                                                                            parseInfo->staticContext->namespaceBindings(),
                                                                                            &ryy, true);
                if(declaredAttributes.contains(att))
                {
                    parseInfo->staticContext->error(QtXmlPatterns::tr("An attribute with name %1 has already appeared on this element.")
                                                      .arg(formatKeyword(parseInfo->staticContext->namePool(), att)),
                                            ReportContext::XQST0040, fromYYLTYPE((yyloc), parseInfo));

                }
                else
                    declaredAttributes.insert(att);

                /* wrapLiteral() needs the SourceLocationReflection of the AttributeConstructor, but
                 * it's unknown inside the arguments to its constructor. Hence we have to do this workaround of setting
                 * it twice.
                 *
                 * The AttributeConstructor's arguments are just dummies. */
                const Expression::Ptr ctor(create(new AttributeConstructor((yyvsp[-1].attributeHolders).at(i).second, (yyvsp[-1].attributeHolders).at(i).second), (yyloc), parseInfo));

                Expression::List ops;
                ops.append(wrapLiteral(toItem(QNameValue::fromValue(namePool, att)), parseInfo->staticContext, ctor.data()));
                ops.append((yyvsp[-1].attributeHolders).at(i).second);
                ctor->setOperands(ops);

                attributes.append(ctor);
            }
        }

        Expression::Ptr contentOp;

        if(attributes.isEmpty())
            contentOp = (yyvsp[0].expr);
        else
        {
            attributes.append((yyvsp[0].expr));
            contentOp = create(new ExpressionSequence(attributes), (yyloc), parseInfo);
        }

        const Expression::Ptr name(create(new Literal(toItem(QNameValue::fromValue(parseInfo->staticContext->namePool(), parseInfo->tagStack.top()))), (yyloc), parseInfo));
        (yyval.expr) = create(new ElementConstructor(name, contentOp, parseInfo->isXSLT()), (yyloc), parseInfo);

        /* Restore the old context. We don't want the namespaces
         * to be in-scope for expressions appearing after the
         * element they appeared on. */
        parseInfo->staticContext->setNamespaceBindings(parseInfo->resolvers.pop());
        parseInfo->tagStack.pop();

        --parseInfo->elementConstructorDepth;
    }
#line 6779 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 363:
#line 4033 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new EmptySequence(), (yyloc), parseInfo);
    }
#line 6787 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 364:
#line 4037 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(!(yyvsp[-1].qName).isLexicallyEqual(parseInfo->tagStack.top()))
        {
            parseInfo->staticContext->error(QtXmlPatterns::tr("A direct element constructor is not "
                                               "well-formed. %1 is ended with %2.")
                                               .arg(formatKeyword(parseInfo->staticContext->namePool()->toLexical(parseInfo->tagStack.top())),
                                                    formatKeyword(parseInfo->staticContext->namePool()->toLexical((yyvsp[-1].qName)))),
                                            ReportContext::XPST0003, fromYYLTYPE((yyloc), parseInfo));
        }

        if((yyvsp[-3].expressionList).isEmpty())
            (yyval.expr) = create(new EmptySequence(), (yyloc), parseInfo);
        else if((yyvsp[-3].expressionList).size() == 1)
            (yyval.expr) = (yyvsp[-3].expressionList).first();
        else
            (yyval.expr) = create(new ExpressionSequence((yyvsp[-3].expressionList)), (yyloc), parseInfo);
    }
#line 6809 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 365:
#line 4056 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.attributeHolders) = AttributeHolderVector();
    }
#line 6817 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 366:
#line 4060 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyvsp[-1].attributeHolders).append((yyvsp[0].attributeHolder));
        (yyval.attributeHolders) = (yyvsp[-1].attributeHolders);
    }
#line 6826 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 367:
#line 4066 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.attributeHolder) = qMakePair((yyvsp[-2].sval), (yyvsp[0].expr));
    }
#line 6834 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 368:
#line 4071 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = createDirAttributeValue((yyvsp[-1].expressionList), parseInfo, (yyloc));
    }
#line 6842 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 369:
#line 4076 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = createDirAttributeValue((yyvsp[-1].expressionList), parseInfo, (yyloc));
    }
#line 6850 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 370:
#line 4081 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expressionList) = Expression::List();
    }
#line 6858 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 371:
#line 4085 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        Expression::Ptr content((yyvsp[-1].expr));

        if(parseInfo->isBackwardsCompat.top())
            content = create(GenericPredicate::createFirstItem(content), (yyloc), parseInfo);

        (yyvsp[0].expressionList).prepend(createSimpleContent(content, (yyloc), parseInfo));
        (yyval.expressionList) = (yyvsp[0].expressionList);
    }
#line 6872 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 372:
#line 4095 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyvsp[0].expressionList).prepend(create(new Literal(AtomicString::fromValue((yyvsp[-1].sval))), (yyloc), parseInfo));
        (yyval.expressionList) = (yyvsp[0].expressionList);
    }
#line 6881 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 373:
#line 4101 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expressionList) = Expression::List();
        parseInfo->isPreviousEnclosedExpr = false;
    }
#line 6890 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 374:
#line 4106 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyvsp[-1].expressionList).append((yyvsp[0].expr));
        (yyval.expressionList) = (yyvsp[-1].expressionList);
        parseInfo->isPreviousEnclosedExpr = false;
    }
#line 6900 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 375:
#line 4112 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(parseInfo->staticContext->boundarySpacePolicy() == StaticContext::BSPStrip &&
           XPathHelper::isWhitespaceOnly((yyvsp[0].sval)))
        {
            (yyval.expressionList) = (yyvsp[-1].expressionList);
        }
        else
        {
            (yyvsp[-1].expressionList).append(create(new TextNodeConstructor(create(new Literal(AtomicString::fromValue((yyvsp[0].sval))), (yyloc), parseInfo)), (yyloc), parseInfo));
            (yyval.expressionList) = (yyvsp[-1].expressionList);
            parseInfo->isPreviousEnclosedExpr = false;
        }
    }
#line 6918 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 376:
#line 4126 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyvsp[-1].expressionList).append(create(new TextNodeConstructor(create(new Literal(AtomicString::fromValue((yyvsp[0].sval))), (yyloc), parseInfo)), (yyloc), parseInfo));
        (yyval.expressionList) = (yyvsp[-1].expressionList);
        parseInfo->isPreviousEnclosedExpr = false;
    }
#line 6928 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 377:
#line 4132 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        /* We insert a text node constructor that send an empty text node between
         * the two enclosed expressions, in order to ensure that no space is inserted.
         *
         * However, we only do it when we have no node constructors. */
        if(parseInfo->isPreviousEnclosedExpr &&
           BuiltinTypes::xsAnyAtomicType->xdtTypeMatches((yyvsp[0].expr)->staticType()->itemType()) &&
           BuiltinTypes::xsAnyAtomicType->xdtTypeMatches((yyvsp[-1].expressionList).last()->staticType()->itemType()))
            (yyvsp[-1].expressionList).append(create(new TextNodeConstructor(create(new Literal(AtomicString::fromValue(QString())), (yyloc), parseInfo)), (yyloc), parseInfo));
        else
            parseInfo->isPreviousEnclosedExpr = true;

        (yyvsp[-1].expressionList).append(createCopyOf((yyvsp[0].expr), parseInfo, (yyloc)));
        (yyval.expressionList) = (yyvsp[-1].expressionList);
    }
#line 6948 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 378:
#line 4149 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new CommentConstructor(create(new Literal(AtomicString::fromValue((yyvsp[0].sval))), (yyloc), parseInfo)), (yyloc), parseInfo);
    }
#line 6956 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 379:
#line 4154 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        const ReflectYYLTYPE ryy((yyloc), parseInfo);
        NCNameConstructor::validateTargetName<StaticContext::Ptr,
                                              ReportContext::XPST0003,
                                              ReportContext::XPST0003>((yyvsp[-1].sval),
                                                                       parseInfo->staticContext, &ryy);

        (yyval.expr) = create(new ProcessingInstructionConstructor(
                             create(new Literal(AtomicString::fromValue((yyvsp[-1].sval))), (yyloc), parseInfo),
                             create(new Literal(AtomicString::fromValue((yyvsp[0].sval))), (yyloc), parseInfo)), (yyloc), parseInfo);
    }
#line 6972 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 387:
#line 4175 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc), (yyvsp[-1].enums.Bool));

        (yyval.expr) = create(new DocumentConstructor((yyvsp[0].expr)), (yyloc), parseInfo);
    }
#line 6982 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 388:
#line 4182 "querytransformparser.ypp" /* yacc.c:1652  */
    {
                        /* This value is incremented before the action below is executed. */
                        ++parseInfo->elementConstructorDepth;
                     }
#line 6991 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 389:
#line 4187 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        Q_ASSERT(5);
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc), (yyvsp[-3].enums.Bool));

        Expression::Ptr effExpr;

        if((yyvsp[0].expr))
            effExpr = createCopyOf((yyvsp[0].expr), parseInfo, (yyloc));
        else
            effExpr = create(new EmptySequence(), (yyloc), parseInfo);

        const QXmlName::NamespaceCode ns = parseInfo->resolvers.top()->lookupNamespaceURI(StandardPrefixes::empty);

        /* Ensure the default namespace gets counted as an in-scope binding, if such a one exists. If we're
         * a child of another constructor, it has already been done. */
        if(parseInfo->elementConstructorDepth == 1 && ns != StandardNamespaces::empty)
        {
            Expression::List exprList;

            /* We append the namespace constructor before the body, in order to
             * comply with QAbstractXmlPushHandler's contract. */
            const QXmlName def(parseInfo->resolvers.top()->lookupNamespaceURI(StandardPrefixes::empty), StandardLocalNames::empty);
            exprList.append(create(new NamespaceConstructor(def), (yyloc), parseInfo));

            exprList.append(effExpr);

            effExpr = create(new ExpressionSequence(exprList), (yyloc), parseInfo);
        }

        --parseInfo->elementConstructorDepth;
        (yyval.expr) = create(new ElementConstructor((yyvsp[-2].expr), effExpr, parseInfo->isXSLT()), (yyloc), parseInfo);
    }
#line 7028 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 390:
#line 4221 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.Bool) = false;
    }
#line 7036 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 391:
#line 4225 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.enums.Bool) = true;
    }
#line 7044 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 392:
#line 4233 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc), (yyvsp[-2].enums.Bool));

        const Expression::Ptr name(create(new AttributeNameValidator((yyvsp[-1].expr)), (yyloc), parseInfo));

        if((yyvsp[0].expr))
            (yyval.expr) = create(new AttributeConstructor(name, createSimpleContent((yyvsp[0].expr), (yyloc), parseInfo)), (yyloc), parseInfo);
        else
            (yyval.expr) = create(new AttributeConstructor(name, create(new EmptySequence(), (yyloc), parseInfo)), (yyloc), parseInfo);
    }
#line 7059 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 393:
#line 4245 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new TextNodeConstructor(createSimpleContent((yyvsp[0].expr), (yyloc), parseInfo)), (yyloc), parseInfo);
    }
#line 7067 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 394:
#line 4250 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc), (yyvsp[-1].enums.Bool));

        (yyval.expr) = create(new CommentConstructor(createSimpleContent((yyvsp[0].expr), (yyloc), parseInfo)), (yyloc), parseInfo);
    }
#line 7077 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 395:
#line 4257 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QXmlQuery::XQuery10, parseInfo, (yyloc), (yyvsp[-1].expr));

        if((yyvsp[0].expr))
        {
            (yyval.expr) = create(new ProcessingInstructionConstructor((yyvsp[-1].expr), createSimpleContent((yyvsp[0].expr), (yyloc), parseInfo)), (yyloc), parseInfo);
        }
        else
            (yyval.expr) = create(new ProcessingInstructionConstructor((yyvsp[-1].expr), create(new EmptySequence(), (yyloc), parseInfo)), (yyloc), parseInfo);
    }
#line 7092 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 396:
#line 4268 "querytransformparser.ypp" /* yacc.c:1652  */
    {
                        parseInfo->nodeTestSource = BuiltinTypes::attribute;
                   }
#line 7100 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 397:
#line 4272 "querytransformparser.ypp" /* yacc.c:1652  */
    {
                        parseInfo->restoreNodeTestSource();
                   }
#line 7108 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 398:
#line 4275 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new Literal(toItem(QNameValue::fromValue(parseInfo->staticContext->namePool(), (yyvsp[-1].qName)))), (yyloc), parseInfo);
    }
#line 7116 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 400:
#line 4281 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new Literal(toItem(QNameValue::fromValue(parseInfo->staticContext->namePool(), (yyvsp[0].qName)))), (yyloc), parseInfo);
    }
#line 7124 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 402:
#line 4287 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(BuiltinTypes::xsQName->xdtTypeMatches((yyvsp[0].expr)->staticType()->itemType()))
            (yyval.expr) = (yyvsp[0].expr);
        else
        {
            (yyval.expr) = create(new QNameConstructor((yyvsp[0].expr),
                                             parseInfo->staticContext->namespaceBindings()),
                        (yyloc), parseInfo);
        }
    }
#line 7139 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 403:
#line 4302 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new NCNameConstructor(create(new Literal(AtomicString::fromValue((yyvsp[0].sval))), (yyloc), parseInfo)), (yyloc), parseInfo);
    }
#line 7147 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 404:
#line 4306 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.expr) = create(new NCNameConstructor((yyvsp[0].expr)), (yyloc), parseInfo);
    }
#line 7155 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 405:
#line 4315 "querytransformparser.ypp" /* yacc.c:1652  */
    {
    (yyval.expr) = create(new ComputedNamespaceConstructor((yyvsp[-1].expr), (yyvsp[0].expr)), (yyloc), parseInfo);
}
#line 7163 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 406:
#line 4320 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.sequenceType) = makeGenericSequenceType((yyvsp[0].itemType), Cardinality::exactlyOne());
    }
#line 7171 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 407:
#line 4324 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.sequenceType) = makeGenericSequenceType((yyvsp[-1].itemType), Cardinality::zeroOrOne());
    }
#line 7179 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 408:
#line 4329 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.sequenceType) = CommonSequenceTypes::ZeroOrMoreItems;
    }
#line 7187 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 409:
#line 4333 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.sequenceType) = (yyvsp[0].sequenceType);
    }
#line 7195 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 410:
#line 4338 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.sequenceType) = makeGenericSequenceType((yyvsp[-1].itemType), (yyvsp[0].cardinality));
    }
#line 7203 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 411:
#line 4343 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.sequenceType) = CommonSequenceTypes::Empty;
    }
#line 7211 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 412:
#line 4347 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.cardinality) = Cardinality::exactlyOne();}
#line 7217 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 413:
#line 4348 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.cardinality) = Cardinality::oneOrMore();}
#line 7223 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 414:
#line 4349 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.cardinality) = Cardinality::zeroOrMore();}
#line 7229 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 415:
#line 4350 "querytransformparser.ypp" /* yacc.c:1652  */
    {(yyval.cardinality) = Cardinality::zeroOrOne();}
#line 7235 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 419:
#line 4356 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.itemType) = BuiltinTypes::item;
    }
#line 7243 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 420:
#line 4361 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        const SchemaType::Ptr t(parseInfo->staticContext->schemaDefinitions()->createSchemaType((yyvsp[0].qName)));

        if(!t)
        {
            parseInfo->staticContext->error(QtXmlPatterns::tr("The name %1 does not refer to any schema type.")
                                               .arg(formatKeyword(parseInfo->staticContext->namePool(), (yyvsp[0].qName))), ReportContext::XPST0051, fromYYLTYPE((yyloc), parseInfo));
        }
        else if(BuiltinTypes::xsAnyAtomicType->wxsTypeMatches(t))
            (yyval.itemType) = AtomicType::Ptr(t);
        else
        {
            /* Try to give an intelligent message. */
            if(t->isComplexType())
            {
                parseInfo->staticContext->error(QtXmlPatterns::tr("%1 is an complex type. Casting to complex "
                                                   "types is not possible. However, casting "
                                                   "to atomic types such as %2 works.")
                                                   .arg(formatType(parseInfo->staticContext->namePool(), t))
                                                   .arg(formatType(parseInfo->staticContext->namePool(), BuiltinTypes::xsInteger)),
                                                ReportContext::XPST0051, fromYYLTYPE((yyloc), parseInfo));
            }
            else
            {
                parseInfo->staticContext->error(QtXmlPatterns::tr("%1 is not an atomic type. Casting "
                                                   "is only possible to atomic types.")
                                                   .arg(formatType(parseInfo->staticContext->namePool(), t)),
                                                ReportContext::XPST0051, fromYYLTYPE((yyloc), parseInfo));
            }
        }
    }
#line 7279 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 428:
#line 4405 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.itemType) = BuiltinTypes::node;
    }
#line 7287 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 429:
#line 4410 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.itemType) = BuiltinTypes::document;
    }
#line 7295 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 430:
#line 4415 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        // TODO support for document element testing
        (yyval.itemType) = BuiltinTypes::document;
    }
#line 7304 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 433:
#line 4424 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.itemType) = BuiltinTypes::text;
    }
#line 7312 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 434:
#line 4429 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.itemType) = BuiltinTypes::comment;
    }
#line 7320 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 435:
#line 4434 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.itemType) = BuiltinTypes::pi;
    }
#line 7328 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 436:
#line 4439 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.itemType) = LocalNameTest::create(BuiltinTypes::pi, parseInfo->staticContext->namePool()->allocateLocalName((yyvsp[-1].sval)));
    }
#line 7336 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 437:
#line 4444 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(QXmlUtils::isNCName((yyvsp[-1].sval)))
        {
            (yyval.itemType) = LocalNameTest::create(BuiltinTypes::pi, parseInfo->staticContext->namePool()->allocateLocalName((yyvsp[-1].sval)));
        }
        else
        {
            parseInfo->staticContext->error(QtXmlPatterns::tr("%1 is not a valid name for a "
                                                              "processing-instruction.")
                                                 .arg(formatKeyword((yyvsp[-1].sval))),
                                            ReportContext::XPTY0004,
                                            fromYYLTYPE((yyloc), parseInfo));
        }
    }
#line 7355 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 440:
#line 4463 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.itemType) = BuiltinTypes::attribute;
    }
#line 7363 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 441:
#line 4468 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.itemType) = BuiltinTypes::attribute;
    }
#line 7371 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 442:
#line 4473 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.itemType) = QNameTest::create(BuiltinTypes::attribute, (yyvsp[-1].qName));
    }
#line 7379 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 443:
#line 4477 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        const SchemaType::Ptr t(parseInfo->staticContext->schemaDefinitions()->createSchemaType((yyvsp[-1].qName)));

        if(t)
            (yyval.itemType) = BuiltinTypes::attribute;
        else
        {
            parseInfo->staticContext->error(unknownType().arg(formatKeyword(parseInfo->staticContext->namePool(), (yyvsp[-1].qName))),
                                            ReportContext::XPST0008, fromYYLTYPE((yyloc), parseInfo));
        }
    }
#line 7395 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 444:
#line 4489 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        const SchemaType::Ptr t(parseInfo->staticContext->schemaDefinitions()->createSchemaType((yyvsp[-1].qName)));

        if(t)
            (yyval.itemType) = BuiltinTypes::attribute;
        else
        {
            parseInfo->staticContext->error(unknownType().arg(formatKeyword(parseInfo->staticContext->namePool(), (yyvsp[-1].qName))),
                                            ReportContext::XPST0008, fromYYLTYPE((yyloc), parseInfo));
        }
    }
#line 7411 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 445:
#line 4502 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        parseInfo->staticContext->error(QtXmlPatterns::tr("%1 is not in the in-scope attribute "
                                           "declarations. Note that the schema import "
                                           "feature is not supported.")
                                           .arg(formatKeyword(parseInfo->staticContext->namePool(), (yyvsp[-1].qName))),
                                        ReportContext::XPST0008, fromYYLTYPE((yyloc), parseInfo));
        (yyval.itemType).reset();
    }
#line 7424 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 446:
#line 4512 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.itemType) = BuiltinTypes::element;
    }
#line 7432 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 447:
#line 4517 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.itemType) = BuiltinTypes::element;
    }
#line 7440 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 448:
#line 4522 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.itemType) = QNameTest::create(BuiltinTypes::element, (yyvsp[-1].qName));
    }
#line 7448 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 449:
#line 4527 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        const SchemaType::Ptr t(parseInfo->staticContext->schemaDefinitions()->createSchemaType((yyvsp[-2].qName)));

        if(t)
            (yyval.itemType) = BuiltinTypes::element;
        else
        {
            parseInfo->staticContext->error(unknownType()
                                               .arg(formatKeyword(parseInfo->staticContext->namePool(), (yyvsp[-2].qName))),
                                            ReportContext::XPST0008, fromYYLTYPE((yyloc), parseInfo));
        }
    }
#line 7465 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 450:
#line 4541 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        const SchemaType::Ptr t(parseInfo->staticContext->schemaDefinitions()->createSchemaType((yyvsp[-2].qName)));

        if(t)
            (yyval.itemType) = BuiltinTypes::element;
        else
        {
            parseInfo->staticContext->error(QtXmlPatterns::tr("%1 is an unknown schema type.")
                                               .arg(formatKeyword(parseInfo->staticContext->namePool(), (yyvsp[-2].qName))),
                                            ReportContext::XPST0008, fromYYLTYPE((yyloc), parseInfo));
        }
    }
#line 7482 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 453:
#line 4558 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        parseInfo->staticContext->error(QtXmlPatterns::tr("%1 is not in the in-scope attribute "
                                           "declarations. Note that the schema import "
                                           "feature is not supported.")
                                           .arg(formatKeyword(parseInfo->staticContext->namePool(), (yyvsp[-1].qName))),
                                        ReportContext::XPST0008, fromYYLTYPE((yyloc), parseInfo));
        (yyval.itemType).reset();
    }
#line 7495 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 455:
#line 4570 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.qName) = parseInfo->staticContext->namePool()->allocateQName(StandardNamespaces::empty, (yyvsp[0].sval));
    }
#line 7503 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 457:
#line 4582 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        if(parseInfo->nodeTestSource == BuiltinTypes::element)
            (yyval.qName) = parseInfo->staticContext->namePool()->allocateQName(parseInfo->staticContext->namespaceBindings()->lookupNamespaceURI(StandardPrefixes::empty), (yyvsp[0].sval));
        else
            (yyval.qName) = parseInfo->staticContext->namePool()->allocateQName(StandardNamespaces::empty, (yyvsp[0].sval));
    }
#line 7514 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 462:
#line 4596 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.qName) = parseInfo->staticContext->namePool()->allocateQName(parseInfo->staticContext->defaultFunctionNamespace(), (yyvsp[0].sval));
    }
#line 7522 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 463:
#line 4600 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.qName) = parseInfo->staticContext->namePool()->allocateQName(StandardNamespaces::InternalXSLT, (yyvsp[0].sval));
    }
#line 7530 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 466:
#line 4608 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        parseInfo->staticContext->error(QtXmlPatterns::tr("The name of an extension expression must be in "
                                                          "a namespace."),
                                        ReportContext::XPST0081, fromYYLTYPE((yyloc), parseInfo));
    }
#line 7540 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 469:
#line 4618 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
    }
#line 7548 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 470:
#line 4622 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        allowedIn(QueryLanguages(QXmlQuery::XQuery10 | QXmlQuery::XPath20), parseInfo, (yyloc));
    }
#line 7556 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 471:
#line 4627 "querytransformparser.ypp" /* yacc.c:1652  */
    {

        const ReflectYYLTYPE ryy((yyloc), parseInfo);

        (yyval.qName) = QNameConstructor::
             expandQName<StaticContext::Ptr,
                         ReportContext::XPST0081,
                         ReportContext::XPST0081>((yyvsp[0].sval), parseInfo->staticContext,
                                                  parseInfo->staticContext->namespaceBindings(), &ryy);

    }
#line 7572 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;

  case 472:
#line 4639 "querytransformparser.ypp" /* yacc.c:1652  */
    {
        (yyval.qName) = parseInfo->staticContext->namePool()->fromClarkName((yyvsp[0].sval));
    }
#line 7580 "qquerytransformparser.cpp" /* yacc.c:1652  */
    break;


#line 7584 "qquerytransformparser.cpp" /* yacc.c:1652  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, parseInfo, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, parseInfo, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, parseInfo);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, parseInfo);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, parseInfo, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, parseInfo);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, parseInfo);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 4643 "querytransformparser.ypp" /* yacc.c:1918  */


QString Tokenizer::tokenToString(const Token &token)
{
    switch(token.type)
    {
        case T_NCNAME:
        case T_QNAME:
        case T_NUMBER:
        case T_XPATH2_NUMBER:
            return token.value;
        case T_STRING_LITERAL:
            return QLatin1Char('"') + token.value + QLatin1Char('"');
        default:
        {
            const QString raw(QString::fromLatin1(yytname[YYTRANSLATE(token.type)]));

            /* Remove the quotes. */
            if(raw.at(0) == QLatin1Char('"') && raw.length() > 1)
                return raw.mid(1, raw.length() - 2);
            else
                return raw;
        }
    }
}

} /* namespace Patternist */

QT_END_NAMESPACE

// vim: et:ts=4:sw=4:sts=4:syntax=yacc
