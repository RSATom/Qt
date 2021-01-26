/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the tools applications of the Qt Toolkit.
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

#include <QAxObject>
#include <QFile>
#include <QMetaObject>
#include <QMetaEnum>
#include <QTextStream>
#include <QSettings>
#include <QStringList>
#include <QTemporaryFile>
#include <QUuid>
#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QWidget>
#include <QFileInfo>
#include <qt_windows.h>
#include <ocidl.h>
#include <private/qmetaobject_p.h>
QT_BEGIN_NAMESPACE

static ITypeInfo *currentTypeInfo = nullptr;

enum ProgramMode {
    GenerateMode,
    TypeLibID
};

enum ObjectCategory
{
    DefaultObject    = 0x00,
    SubObject        = 0x001,
    ActiveX          = 0x002,
    NoMetaObject     = 0x004,
    NoImplementation = 0x008,
    NoDeclaration    = 0x010,
    NoInlines        = 0x020,
    OnlyInlines      = 0x040,
    Licensed         = 0x100,
};

Q_DECLARE_FLAGS(ObjectCategories, ObjectCategory)
Q_DECLARE_OPERATORS_FOR_FLAGS(ObjectCategories)

extern QMetaObject *qax_readEnumInfo(ITypeLib *typeLib, const QMetaObject *parentObject);
extern QMetaObject *qax_readClassInfo(ITypeLib *typeLib, ITypeInfo *typeInfo, const QMetaObject *parentObject);
extern QMetaObject *qax_readInterfaceInfo(ITypeLib *typeLib, ITypeInfo *typeInfo, const QMetaObject *parentObject);
extern QByteArrayList qax_qualified_usertypes;
extern QString qax_docuFromName(ITypeInfo *typeInfo, const QString &name);
extern bool qax_dispatchEqualsIDispatch;
extern void qax_deleteMetaObject(QMetaObject *mo);

static QMap<QByteArray, QByteArray> namespaceForType;
static QVector<QByteArray> strings;
static QHash<QByteArray, int> stringIndex; // Optimization, speeds up generation
static QByteArrayList vTableOnlyStubs;

void writeEnums(QTextStream &out, const QMetaObject *mo)
{
    // enums
    for (int ienum = mo->enumeratorOffset(); ienum < mo->enumeratorCount(); ++ienum) {
        QMetaEnum metaEnum = mo->enumerator(ienum);
        out << "    enum " << metaEnum.name() << " {" << Qt::endl;
        for (int k = 0; k < metaEnum.keyCount(); ++k) {
            QByteArray key(metaEnum.key(k));
            out << "        " << key.leftJustified(24) << "= " << metaEnum.value(k);
            if (k < metaEnum.keyCount() - 1)
                out << ',';
            out << Qt::endl;
        }
        out << "    };" << Qt::endl;
        out << Qt::endl;
    }
}

void writeHeader(QTextStream &out, const QString &nameSpace, const QString &outFileName)
{
    out << "#ifndef QAX_DUMPCPP_" << outFileName.toUpper() << "_H" << Qt::endl;
    out << "#define QAX_DUMPCPP_" << outFileName.toUpper() << "_H" << Qt::endl;
    out << Qt::endl;
    out << "// Define this symbol to __declspec(dllexport) or __declspec(dllimport)" << Qt::endl;
    out << "#ifndef " << nameSpace.toUpper() << "_EXPORT" << Qt::endl;
    out << "#define " << nameSpace.toUpper() << "_EXPORT" << Qt::endl;
    out << "#endif" << Qt::endl;
    out << Qt::endl;
    out << "#include <qaxobject.h>" << Qt::endl;
    out << "#include <qaxwidget.h>" << Qt::endl;
    out << "#include <qdatetime.h>" << Qt::endl;
    out << "#include <qpixmap.h>" << Qt::endl;
    out << Qt::endl;
    out << "struct IDispatch;" << Qt::endl;
    out << Qt::endl;
}

void generateNameSpace(QTextStream &out, const QMetaObject *mo, const QByteArray &nameSpace)
{
    out << "namespace " << nameSpace << " {" << Qt::endl;
    out << Qt::endl;
    writeEnums(out, mo);

    // don't close on purpose
}

static QByteArray joinParameterNames(const QByteArrayList &parameterNames)
{
    QByteArray slotParameters;
    for (int p = 0; p < parameterNames.count(); ++p) {
        slotParameters += parameterNames.at(p);
        if (p < parameterNames.count() - 1)
            slotParameters += ',';
    }

    return slotParameters;
}

QByteArray constRefify(const QByteArray &type)
{
    QByteArray ctype(type);
    if (type == "QString" || type == "QPixmap"
        || type == "QVariant" || type == "QDateTime"
        || type == "QColor" || type == "QFont"
        || type == "QByteArray" || type == "QValueList<QVariant>"
        || type == "QStringList")
        ctype = "const " + ctype + '&';

    return ctype;
}

static void formatConstructorSignature(QTextStream &out, ObjectCategories category,
                                       bool declaration)
{
    out << '(';
    if (category & Licensed) {
        out << "const QString &licenseKey, ";
        if (declaration)
            out << " = QString()";
        out << ", ";
    }
    if (category & ActiveX) {
        out << "QWidget *parent";
        if (declaration)
            out << " = nullptr";
        out << ", Qt::WindowFlags f";
        if (declaration)
            out << " = {}";
    } else if (category & SubObject) {
        out << "IDispatch *subobject";
        if (declaration)
            out << " = nullptr";
        out << ", QAxObject *parent";
        if (declaration)
            out << " = nullptr";
    } else {
        out << "QObject *parent";
        if (declaration)
            out << " = nullptr";
    }
    out << ')';
}

static void formatConstructorBody(QTextStream &out, const QByteArray &className,
                                  const QString &controlID, ObjectCategories category)
{
    out << className << "::" << className;
    formatConstructorSignature(out, category, false);
    out << " :" << Qt::endl << "    ";
    if (category & ActiveX)
        out << "QAxWidget(parent, f";
    else if (category & SubObject)
        out << "QAxObject(subobject, parent";
    else
        out << "QAxObject(parent";
    out << ')' << Qt::endl << '{' << Qt::endl;
    if (category & SubObject) {
        out << "    internalRelease();" << Qt::endl;
    } else if (category & Licensed) {
        out << "    if (licenseKey.isEmpty())" << Qt::endl;
        out << "        setControl(QStringLiteral(\"" << controlID << "\"));" << Qt::endl;
        out << "    else" << Qt::endl;
        out << "        setControl(QStringLiteral(\"" << controlID << ":\") + licenseKey);" << Qt::endl;
    } else {
        out << "    setControl(QStringLiteral(\"" << controlID << "\"));" << Qt::endl;
    }
    out << '}' << Qt::endl << Qt::endl;
}

void generateClassDecl(QTextStream &out, const QMetaObject *mo,
                       const QByteArray &className, const QByteArray &nameSpace,
                       ObjectCategories category)
{
    QByteArrayList functions;

    QByteArray indent;
    if (!(category & OnlyInlines))
        indent = "    ";

    if (!(category & OnlyInlines)) {
        // constructors
        out << "class " << nameSpace.toUpper() << "_EXPORT " << className << " : public ";
        if (category & ActiveX)
            out << "QAxWidget";
        else
            out << "QAxObject";
        out << Qt::endl;

        out << '{' << Qt::endl;
        out << "public:" << Qt::endl << "    explicit " << className;
        formatConstructorSignature(out, category, true);
        out << ';' << Qt::endl;
        for (int ci = mo->classInfoOffset(); ci < mo->classInfoCount(); ++ci) {
            QMetaClassInfo info = mo->classInfo(ci);
            QByteArray iface_name = info.name();
            if (iface_name.startsWith("Event "))
                continue;

            QByteArray iface_class = info.value();

            out << "    " << className << '(' << iface_class << " *iface)" << Qt::endl;

            if (category & ActiveX)
                out << "    : QAxWidget()" << Qt::endl;
            else
                out << "    : QAxObject()" << Qt::endl;
            out << "    {" << Qt::endl;
            out << "        initializeFrom(iface);" << Qt::endl;
            out << "        delete iface;" << Qt::endl;
            out << "    }" << Qt::endl;
            out << Qt::endl;
        }
    }

    functions << className;

    // enums
    if (nameSpace.isEmpty() && !(category & OnlyInlines)) {
        for (int ienum = mo->enumeratorOffset(); ienum < mo->enumeratorCount(); ++ienum) {
            QMetaEnum metaEnum = mo->enumerator(ienum);
            out << "    enum " << metaEnum.name() << " {" << Qt::endl;
            for (int k = 0; k < metaEnum.keyCount(); ++k) {
                QByteArray key(metaEnum.key(k));
                out << "        " << key.leftJustified(24) << "= " << metaEnum.value(k);
                if (k < metaEnum.keyCount() - 1)
                    out << ',';
                out << Qt::endl;
            }
            out << "    };" << Qt::endl;
            out << Qt::endl;
        }
    }
    // QAxBase public virtual functions.
    QByteArrayList axBase_vfuncs;
    axBase_vfuncs.append("metaObject");
    axBase_vfuncs.append("qObject");
    axBase_vfuncs.append("className");
    axBase_vfuncs.append("propertyWritable");
    axBase_vfuncs.append("setPropertyWritable");

    // properties
    for (int iprop = mo->propertyOffset(); iprop < mo->propertyCount(); ++iprop) {
        QMetaProperty property = mo->property(iprop);
        if (!property.isReadable())
            continue;

        QByteArray propertyName(property.name());
        if (propertyName == "control" || propertyName == className)
            continue;

        if (!(category & OnlyInlines)) {
            out << indent << "/*" << Qt::endl << indent << "Property " << propertyName << Qt::endl;
            QString documentation = qax_docuFromName(currentTypeInfo, QString::fromLatin1(propertyName.constData()));
            if (!documentation.isEmpty()) {
                out << Qt::endl;
                out << indent << documentation << Qt::endl;
            }
            out << indent << "*/" << Qt::endl;
        }

        // Check whether the new function conflicts with any of QAxBase public virtual functions.
        // If so, prepend the function name with '<classname>_'. Since all internal metaobject magic
        // remains the same, we have to use the original name when used with QObject::connect or QMetaObject
        QByteArray propertyFunctionName(propertyName);
        if (axBase_vfuncs.contains(propertyFunctionName)) {
            propertyFunctionName = className + '_' + propertyName;
            qWarning("property conflits with QAXBase: %s changed to %s", propertyName.constData(), propertyFunctionName.constData());
        }

        QByteArray propertyType(property.typeName());

        QByteArray simplePropType = propertyType;
        simplePropType.replace('*', "");

        out << indent << "inline ";
        bool foreignNamespace = true;
        if (!propertyType.contains("::") &&
            (qax_qualified_usertypes.contains(simplePropType) || qax_qualified_usertypes.contains("enum "+ simplePropType))
           ) {
            propertyType.prepend(nameSpace + "::");
            foreignNamespace = false;
        }

        out << propertyType << ' ';

        if (category & OnlyInlines)
            out << className << "::";
        out << propertyFunctionName << "() const";

        if (!(category & NoInlines)) {
            out << Qt::endl << indent << '{' << Qt::endl;
            if (qax_qualified_usertypes.contains(simplePropType)) {
                if (foreignNamespace)
                    out << "#ifdef QAX_DUMPCPP_" << propertyType.left(propertyType.indexOf("::")).toUpper() << "_H" << Qt::endl;
                out << indent << "    " << propertyType << " qax_pointer = 0;" << Qt::endl;
                QByteArray simplePropTypeWithNamespace = propertyType;
                simplePropTypeWithNamespace.replace('*', "");
                out << indent << "    qRegisterMetaType<" << propertyType << ">(\"" << property.typeName() << "\", &qax_pointer);" << Qt::endl;
                out << indent << "    qRegisterMetaType<" << simplePropTypeWithNamespace << ">(\"" << simplePropType << "\", qax_pointer);" << Qt::endl;
            }
            out << indent << "    QVariant qax_result = property(\"" << propertyName << "\");" << Qt::endl;
            if (propertyType.length() && propertyType.at(propertyType.length()-1) == '*')
                out << indent << "    if (!qax_result.constData()) return 0;" << Qt::endl;
            out << indent << "    Q_ASSERT(qax_result.isValid());" << Qt::endl;
            if (qax_qualified_usertypes.contains(simplePropType)) {
                simplePropType = propertyType;
                simplePropType.replace('*', "");
                out << indent << "    return *(" << propertyType << "*)qax_result.constData();" << Qt::endl;
                if (foreignNamespace) {
                    out << "#else" << Qt::endl;
                    out << indent << "    return 0; // foreign namespace not included" << Qt::endl;
                    out << "#endif" << Qt::endl;
                }

            } else {
                out << indent << "    return *(" << propertyType << "*)qax_result.constData();" << Qt::endl;
            }
            out << indent << '}' << Qt::endl;
        } else {
            out << "; //Returns the value of " << propertyName << Qt::endl;
        }

        functions << propertyName;

        if (property.isWritable()) {
            QByteArray setter(propertyName);
            if (isupper(setter.at(0))) {
                setter = "Set" + setter;
            } else {
                setter[0] = char(toupper(setter[0]));
                setter = "set" + setter;
            }

            out << indent << "inline " << "void ";
            if (category & OnlyInlines)
                out << className << "::";
            out << setter << '(' << constRefify(propertyType) << " value)";

            if (!(category & NoInlines)) {
                if (propertyType.endsWith('*')) {
                    out << '{' << Qt::endl;
                    out << "    int typeId = qRegisterMetaType<" << propertyType << ">(\"" << propertyType << "\", &value);" << Qt::endl;
                    out << "    setProperty(\"" << propertyName << "\", QVariant(typeId, &value));" << Qt::endl;
                    out << '}' << Qt::endl;
                } else {
                    out << "{ setProperty(\"" << propertyName << "\", QVariant(value)); }" << Qt::endl;
                }
            } else {
                out << "; //Sets the value of the " << propertyName << " property" << Qt::endl;
            }

            functions << setter;
        }

        out << Qt::endl;
    }

    // slots - but not property setters
    int defaultArguments = 0;
    for (int islot = mo->methodOffset(); islot < mo->methodCount(); ++islot) {
        const QMetaMethod slot(mo->method(islot));
        if (slot.methodType() != QMetaMethod::Slot)
            continue;

#if 0
        // makes not sense really to respect default arguments...
        if (slot.attributes() & Cloned) {
            ++defaultArguments;
            continue;
        }
#endif

        QByteArray slotSignature(slot.methodSignature());
        QByteArray slotName = slotSignature.left(slotSignature.indexOf('('));
        if (functions.contains(slotName))
            continue;

        if (!(category & OnlyInlines)) {
            out << indent << "/*" << Qt::endl << indent << "Method " << slotName << Qt::endl;
            QString documentation = qax_docuFromName(currentTypeInfo, QString::fromLatin1(slotName.constData()));
            if (!documentation.isEmpty()) {
                out << Qt::endl;
                out << indent << documentation << Qt::endl;
            }
            out << indent << "*/" << Qt::endl;
        }

        QByteArray slotParameters(joinParameterNames(slot.parameterNames()));
        QByteArray slotTag(slot.tag());
        QByteArray slotType(slot.typeName());

        QByteArray simpleSlotType = slotType;
        simpleSlotType.replace('*', "");
        if (!slotType.contains("::") && qax_qualified_usertypes.contains(simpleSlotType))
            slotType.prepend(nameSpace + "::");


        QByteArray slotNamedSignature;
        if (slotSignature.endsWith("()")) { // no parameters - no names
            slotNamedSignature = slotSignature;
        } else {
            slotNamedSignature = slotSignature.left(slotSignature.indexOf('(') + 1);
            QByteArray slotSignatureTruncated(slotSignature.mid(slotNamedSignature.length()));
            slotSignatureTruncated.truncate(slotSignatureTruncated.length() - 1);

            const auto signatureSplit = slotSignatureTruncated.split(',');
            QByteArrayList parameterSplit;
            if (slotParameters.isEmpty()) { // generate parameter names
                for (int i = 0; i < signatureSplit.count(); ++i)
                    parameterSplit << QByteArray("p") + QByteArray::number(i);
            } else {
                parameterSplit = slotParameters.split(',');
            }

            for (int i = 0; i < signatureSplit.count(); ++i) {
                QByteArray parameterType = signatureSplit.at(i);
                if (!parameterType.contains("::") && namespaceForType.contains(parameterType))
                    parameterType.prepend(namespaceForType.value(parameterType) + "::");

                QByteArray arraySpec; // transform array method signature "foo(int[4])" ->"foo(int p[4])"
                const int arrayPos = parameterType.lastIndexOf('[');
                if (arrayPos != -1) {
                    arraySpec = parameterType.right(parameterType.size() - arrayPos);
                    parameterType.truncate(arrayPos);
                }
                slotNamedSignature += constRefify(parameterType);
                slotNamedSignature += ' ';
                slotNamedSignature += parameterSplit.at(i);
                slotNamedSignature += arraySpec;
                if (defaultArguments >= signatureSplit.count() - i) {
                    slotNamedSignature += " = ";
                    slotNamedSignature += parameterType + "()";
                }
                if (i + 1 < signatureSplit.count())
                    slotNamedSignature += ", ";
            }
            slotNamedSignature += ')';
        }

        out << indent << "inline ";

        if (!slotTag.isEmpty())
            out << slotTag << ' ';
        else
            out << slotType << ' ';
        if (category & OnlyInlines)
            out << className << "::";

        // Update function name in case of conflicts with QAxBase public virtual functions.
        int parnIdx = slotNamedSignature.indexOf('(');
        QByteArray slotOriginalName =  slotNamedSignature.left(parnIdx);
        if (axBase_vfuncs.contains(slotOriginalName)) {
            QByteArray newSignature = className + '_' + slotOriginalName;
            newSignature += slotNamedSignature.mid(parnIdx);
            qWarning("function name conflits with QAXBase %s changed to %s", slotNamedSignature.constData(), newSignature.constData());
            slotNamedSignature = newSignature;
        }

        out << slotNamedSignature;

        if (category & NoInlines) {
            out << ';' << Qt::endl;
        } else {
            out << Qt::endl;
            out << indent << '{' << Qt::endl;

            if (slotType != QByteArrayLiteral("void")) {
                out << indent << "    " << slotType << " qax_result";
                if (slotType.endsWith('*'))
                    out << " = 0";
                out << ';' << Qt::endl;
                if (qax_qualified_usertypes.contains(simpleSlotType)) {
                    bool foreignNamespace = simpleSlotType.contains("::");
                    if (foreignNamespace)
                        out << "#ifdef QAX_DUMPCPP_" << simpleSlotType.left(simpleSlotType.indexOf(':')).toUpper() << "_H" << Qt::endl;
                    QByteArray simpleSlotTypeWithNamespace = slotType;
                    simpleSlotTypeWithNamespace.replace('*', "");
                    out << indent << "    qRegisterMetaType<" << simpleSlotTypeWithNamespace << "*>(\"" << simpleSlotType << "*\", &qax_result);" << Qt::endl;
                    if (!vTableOnlyStubs.contains(simpleSlotTypeWithNamespace))
                        out << indent << "    qRegisterMetaType<" << simpleSlotTypeWithNamespace << ">(\"" << simpleSlotType << "\", qax_result);" << Qt::endl;
                    if (foreignNamespace)
                        out << "#endif" << Qt::endl;
                }
            }
            out << indent << "    void *_a[] = {";
            if (slotType != QByteArrayLiteral("void"))
                out << "(void*)&qax_result";
            else
                out << '0';
            if (!slotParameters.isEmpty()) {
                out << ", (void*)&";
                out << slotParameters.replace(",", ", (void*)&");
            }
            out << "};" << Qt::endl;

            out << indent << "    qt_metacall(QMetaObject::InvokeMetaMethod, " << islot << ", _a);" << Qt::endl;
            if (slotType != QByteArrayLiteral("void"))
                out << indent << "    return qax_result;" << Qt::endl;
            out << indent << '}' << Qt::endl;
        }

        out << Qt::endl;
        defaultArguments = 0;
    }

    if (!(category & OnlyInlines)) {
        if (!(category & NoMetaObject)) {
            out << "// meta object functions" << Qt::endl;
            out << "    static const QMetaObject staticMetaObject;" << Qt::endl;
            out << "    const QMetaObject *metaObject() const override { return &staticMetaObject; }" << Qt::endl;
            out << "    void *qt_metacast(const char *) override;" << Qt::endl;
        }

        out << "};" << Qt::endl;
    }
}

#define addStringIdx(string) \
    out << stridx(string) << ", ";

// The following functions were copied from moc generator with only some minor changes
void strreg(const QByteArray &s)
{
    if (!stringIndex.contains(s)) {
        stringIndex.insert(s, strings.size());
        strings.append(s);
    }
}

void strDetachAndRegister(QByteArray s)
{
    s.detach();
    strreg(s);
}

int stridx(const QByteArray &s)
{
    int i = stringIndex.value(s);
    Q_ASSERT_X(i != -1, Q_FUNC_INFO, "We forgot to register some strings");
    return i;
}

const char *metaTypeEnumValueString(int type)
{
#define RETURN_METATYPENAME_STRING(MetaTypeName, MetaTypeId, RealType) \
    case QMetaType::MetaTypeName: return #MetaTypeName;

    switch (type) {
QT_FOR_EACH_STATIC_TYPE(RETURN_METATYPENAME_STRING)
    }
#undef RETURN_METATYPENAME_STRING
    return nullptr;
}

int nameToBuiltinType(const QByteArray &name)
{
    if (name.isEmpty())
        return 0;

    const int tp = QMetaType::type(name.constData());
    return tp < QMetaType::User ? tp : QMetaType::UnknownType;
}

void copyFileToStream(QFile *file, QTextStream *stream)
{
    file->seek(0);
    QByteArray buffer;
    const int bufferSize = 4096 * 1024;
    buffer.resize(bufferSize);
    while (!file->atEnd()) {
        const int bytesRead = static_cast<int>(file->read(buffer.data(), bufferSize));
        if (bytesRead < bufferSize) {
            buffer.resize(bytesRead);
            *stream << buffer;
            buffer.resize(bufferSize);
        } else {
            *stream << buffer;
        }
    }
}

void generateTypeInfo(QTextStream &out, const QByteArray &typeName)
{
    if (QtPrivate::isBuiltinType(typeName)) {
        int type;
        QByteArray valueString;
        if (typeName == "qreal") {
            type = QMetaType::UnknownType;
            valueString = "QReal";
        } else {
            type = nameToBuiltinType(typeName);
            valueString = metaTypeEnumValueString(type);
        }
        if (!valueString.isEmpty()) {
            out << "QMetaType::" << valueString;
        } else {
            Q_ASSERT(type != QMetaType::UnknownType);
            out << type;
        }
    } else {
        Q_ASSERT(!typeName.isEmpty());
        out << "0x80000000 | " << stridx(typeName);
    }
}
// End functions copied from moc generator

void generateMethods(QTextStream &out, const QMetaObject *mo, const QMetaMethod::MethodType funcType, int &paramsIndex)
{
    out << "// ";
    MethodFlags funcTypeFlag;
    if (funcType == QMetaMethod::Signal) {
        out << "signal";
        funcTypeFlag = MethodSignal;
    } else {
        out << "slot";
        funcTypeFlag = MethodSlot;
    }
    out  << ": name, argc, parameters, tag, flags" << Qt::endl;

    int methodCount = mo->methodCount();
    for (int i = mo->methodOffset(); i < methodCount; ++i) {
        const QMetaMethod method(mo->method(i));
        if (method.methodType() != funcType)
            continue;
        out << "    ";
        addStringIdx(method.name());
        out << method.parameterCount() << ", ";
        out << paramsIndex << ", ";
        addStringIdx(method.tag());
        out << (AccessProtected | method.attributes() | funcTypeFlag) << ',' << Qt::endl;
        paramsIndex += 1 + method.parameterCount() * 2;
    }
    out << Qt::endl;
}

void generateMethodParameters(QTextStream &out, const QMetaObject *mo, const QMetaMethod::MethodType funcType)
{
    out << "// ";
    if (funcType == QMetaMethod::Signal)
        out << "signal";
    else if (funcType == QMetaMethod::Slot)
        out << "slot";
    out  << ": parameters" << Qt::endl;

    int methodCount = mo->methodCount();
    for (int i = mo->methodOffset(); i < methodCount; ++i) {
        const QMetaMethod method(mo->method(i));
        if (method.methodType() != funcType)
            continue;

        out << "    ";

        int argsCount = method.parameterCount();

        // Return type
        generateTypeInfo(out, method.typeName());
        out << ',';

        // Parameter types
        const auto parameterTypes = method.parameterTypes();
        for (int j = 0; j < argsCount; ++j) {
            out << ' ';
            generateTypeInfo(out, parameterTypes.at(j));
            out << ',';
        }

        // Parameter names
        const auto parameterNames = method.parameterNames();
        for (int j = 0; j < argsCount; ++j)
            out << ' ' << stridx(parameterNames.at(j)) << ',';

        out << Qt::endl;
    }
    out << Qt::endl;
}

void generateClassImpl(QTextStream &out, const QMetaObject *mo, const QByteArray &className,
                       const QString &controlID,
                       const QByteArray &nameSpace, ObjectCategories category)
{
    Q_STATIC_ASSERT_X(QMetaObjectPrivate::OutputRevision == 8, "dumpcpp should generate the same version as moc");

    QByteArray qualifiedClassName;
    if (!nameSpace.isEmpty())
        qualifiedClassName = nameSpace + "::";
    qualifiedClassName += className;
    QByteArray qualifiedClassNameIdentifier = qualifiedClassName;
    qualifiedClassNameIdentifier.replace(':', '_');

    int allClassInfoCount = mo->classInfoCount();
    int allMethodCount = mo->methodCount();
    int allPropertyCount = mo->propertyCount();
    int allEnumCount = mo->enumeratorCount();

    int thisClassInfoCount = allClassInfoCount - mo->classInfoOffset();
    int thisEnumCount = allEnumCount - mo->enumeratorOffset();
    int thisMethodCount = allMethodCount - mo->methodOffset();
    int thisPropertyCount = allPropertyCount - mo->propertyOffset();

    int signalCount = 0;
    int slotCount = 0;
    int combinedParameterCount = 0;
    int enumStart = MetaObjectPrivateFieldCount;

    // Register strings
    strreg(qualifiedClassName);
    for (int i = mo->classInfoOffset(); i < allClassInfoCount; ++i) {
        const QMetaClassInfo classInfo = mo->classInfo(i);
        strreg(classInfo.name());
        strreg(classInfo.value());
    }
    for (int i = mo->methodOffset(); i < allMethodCount; ++i) {
        const QMetaMethod method(mo->method(i));
        if (method.methodType() == QMetaMethod::Signal)
            signalCount++;
        if (method.methodType() == QMetaMethod::Slot)
            slotCount++;
        int argsCount = method.parameterCount();
        combinedParameterCount += argsCount;

        strDetachAndRegister(method.name());
        QByteArray typeName = method.typeName();
        if (!QtPrivate::isBuiltinType(typeName))
            strreg(typeName);
        strreg(method.tag());

        const auto parameterNames = method.parameterNames();
        const auto parameterTypes = method.parameterTypes();
        for (int j = 0; j < argsCount; ++j) {
            if (!QtPrivate::isBuiltinType(parameterTypes.at(j)))
                strDetachAndRegister(parameterTypes.at(j));
            strDetachAndRegister(parameterNames.at(j));
        }
    }
    for (int i = mo->propertyOffset(); i < allPropertyCount; ++i) {
        const QMetaProperty property = mo->property(i);
        strreg(property.name());
        if (!QtPrivate::isBuiltinType(property.typeName()))
            strreg(property.typeName());
    }
    for (int i = mo->enumeratorOffset(); i < allEnumCount; ++i) {
        const QMetaEnum enumerator = mo->enumerator(i);
        strreg(enumerator.name());
        for (int j = 0; j < enumerator.keyCount(); ++j)
            strreg(enumerator.key(j));
    }

    // Build data array
    out << "static const uint qt_meta_data_" << qualifiedClassNameIdentifier << "[] = {" << Qt::endl;
    out << Qt::endl;
    out << " // content:" << Qt::endl;
    out << "    7, // revision" << Qt::endl;
    out << "    ";
    addStringIdx(qualifiedClassName);
    out << " // classname" << Qt::endl;
    out << "    " << thisClassInfoCount << ", " << (thisClassInfoCount ? enumStart : 0) << ", // classinfo" << Qt::endl;
    enumStart += thisClassInfoCount * 2;
    out << "    " << thisMethodCount << ", " << (thisMethodCount ? enumStart : 0) << ", // methods" << Qt::endl;
    enumStart += thisMethodCount * 5;
    int paramsIndex = enumStart;
    enumStart += (combinedParameterCount * 2); // parameter types + names
    enumStart += thisMethodCount; // return types
    out << "    " << thisPropertyCount << ", " << (thisPropertyCount ? enumStart : 0) << ", // properties" << Qt::endl;
    enumStart += thisPropertyCount * 3;
    out << "    " << thisEnumCount << ", " << (thisEnumCount ? enumStart : 0) << ", // enums/sets" << Qt::endl;
    out << "    0, 0, // constructors" << Qt::endl;
    out << "    0, // flags" << Qt::endl;
    out << "    " << signalCount << ", // signal count" << Qt::endl;
    out << Qt::endl;

    if (thisClassInfoCount) {
        out << " // classinfo: key, value" << Qt::endl;
        for (int i = mo->classInfoOffset(); i < allClassInfoCount; ++i) {
            QMetaClassInfo classInfo = mo->classInfo(i);
            out << "    ";
            addStringIdx(classInfo.name());
            addStringIdx(classInfo.value());
            out << Qt::endl;
        }
        out << Qt::endl;
    }

    // Signal/Slot arrays
    if (signalCount)
        generateMethods(out, mo, QMetaMethod::Signal, paramsIndex);
    if (slotCount)
        generateMethods(out, mo, QMetaMethod::Slot, paramsIndex);

    // Method parameter arrays
    if (signalCount)
        generateMethodParameters(out, mo, QMetaMethod::Signal);
    if (slotCount)
        generateMethodParameters(out, mo, QMetaMethod::Slot);

    if (thisPropertyCount) {
        out << " // properties: name, type, flags" << Qt::endl;
        for (int i = mo->propertyOffset(); i < allPropertyCount; ++i) {
            QMetaProperty property = mo->property(i);
            out << "    ";
            addStringIdx(property.name());
            generateTypeInfo(out, property.typeName());
            out << ", ";

            uint flags = 0;
            const auto vartype = property.type();
            if (vartype != QVariant::Invalid && vartype != QVariant::UserType)
                flags = uint(vartype) << 24;

            if (property.isReadable())
                flags |= Readable;
            if (property.isWritable())
                flags |= Writable;
            if (property.isEnumType())
                flags |= EnumOrFlag;
            if (property.isDesignable())
                flags |= Designable;
            if (property.isScriptable())
                flags |= Scriptable;
            if (property.isStored())
                flags |= Stored;
            if (property.isEditable())
                flags |= Editable;

            out << "0x" << QString::number(flags, 16).rightJustified(8, QLatin1Char('0'))
                << ", \t\t // " << property.typeName() << ' ' << property.name()
                << Qt::endl;
        }
        out << Qt::endl;
    }

    if (thisEnumCount) {
        out << " // enums: name, flags, count, data" << Qt::endl;
        enumStart += thisEnumCount * 4;
        for (int i = mo->enumeratorOffset(); i < allEnumCount; ++i) {
            QMetaEnum enumerator = mo->enumerator(i);
            out << "    ";
            addStringIdx(enumerator.name());
            out << (enumerator.isFlag() ? "0x1" : "0x0") << ", " << enumerator.keyCount() << ", " << enumStart << ", " << Qt::endl;
            enumStart += enumerator.keyCount() * 2;
        }
        out << Qt::endl;

        out << " // enum data: key, value" << Qt::endl;
        for (int i = mo->enumeratorOffset(); i < allEnumCount; ++i) {
            QMetaEnum enumerator = mo->enumerator(i);
            for (int j = 0; j < enumerator.keyCount(); ++j) {
                out << "    ";
                addStringIdx(enumerator.key(j));
                out << "uint(";
                if (nameSpace.isEmpty())
                    out << className << "::";
                else
                    out << nameSpace << "::";
                out << enumerator.key(j) << ")," << Qt::endl;
            }
        }
    }
    out << "    0 // eod" << Qt::endl;
    out << "};" << Qt::endl;
    out << Qt::endl;

    formatConstructorBody(out, className, controlID, category);

    out << "const QMetaObject " << className << "::staticMetaObject = {" << Qt::endl;
    if (category & ActiveX)
        out << "{ &QWidget::staticMetaObject," << Qt::endl;
    else
        out << "{ &QObject::staticMetaObject," << Qt::endl;
    out << "qt_meta_stringdata_all.data," << Qt::endl;
    out << "qt_meta_data_" << qualifiedClassNameIdentifier << ", nullptr, nullptr, nullptr }" << Qt::endl;
    out << "};" << Qt::endl;
    out << Qt::endl;

    out << "void *" << className << "::qt_metacast(const char *_clname)" << Qt::endl;
    out << '{' << Qt::endl;
    out << "    if (!_clname) return nullptr;" << Qt::endl;
    out << "    if (!strcmp(_clname, \"" << qualifiedClassName << "\"))" << Qt::endl;
    out << "        return static_cast<void*>(const_cast<" << className << "*>(this));" << Qt::endl;
    if (category & ActiveX)
        out << "    return QAxWidget::qt_metacast(_clname);" << Qt::endl;
    else
        out << "    return QAxObject::qt_metacast(_clname);" << Qt::endl;
    out << '}' << Qt::endl;
}

static void formatCommentBlockFooter(const QString &typeLibFile, QTextStream &str)
{
    str << " generated by dumpcpp v" << QT_VERSION_STR << " using\n**";
    const QStringList arguments = QCoreApplication::arguments();
    for (const QString &arg : arguments)
        str << ' ' << arg;
    str << "\n** from the type library " << typeLibFile << "\n**\n"
        << "****************************************************************************/\n\n";
}

static QByteArray classNameFromTypeInfo(ITypeInfo *typeinfo)
{
    BSTR bstr;
    QByteArray result;
    if (SUCCEEDED(typeinfo->GetDocumentation(-1, &bstr, nullptr, nullptr, nullptr))) {
        result = QString::fromWCharArray(bstr).toLatin1();
        SysFreeString(bstr);
    }
    return result;
}

// Extract a list of VTable only stubs from a ITypeLib.
static QByteArrayList vTableOnlyStubsFromTypeLib(ITypeLib *typelib, const QString &nameSpace)
{
    QByteArrayList result;
    const QByteArray nameSpacePrefix = nameSpace.toLatin1() + "::";
    for (UINT i = 0, typeCount = typelib->GetTypeInfoCount(); i < typeCount; ++i) {
        TYPEKIND typekind;
        if (SUCCEEDED(typelib->GetTypeInfoType(i, &typekind)) && typekind == TKIND_INTERFACE) {
            ITypeInfo *typeinfo = nullptr;
            if (SUCCEEDED(typelib->GetTypeInfo(i, &typeinfo) && typeinfo)) {
                result.append(nameSpacePrefix + classNameFromTypeInfo(typeinfo));
                typeinfo->Release();
            }
        }
    }
    return result;
}

static void writeForwardDeclaration(QTextStream &declOut, const QByteArray &className)
{
    if (className.startsWith("enum ")) {
        declOut << "#ifndef Q_CC_MINGW\n"
                << "    " << className << ';' << Qt::endl // Only MSVC accepts this
                << "#else\n"
                << "    " << className << " {};" << Qt::endl
                << "#endif\n";
    } else {
        declOut << "    " << className << ';' << Qt::endl;
    }
}

bool generateTypeLibrary(QString typeLibFile, QString outname,
                         const QString &nameSpace, ObjectCategories category)
{
    typeLibFile.replace(QLatin1Char('/'), QLatin1Char('\\'));

    ITypeLib *typelib;
    LoadTypeLibEx(reinterpret_cast<const wchar_t *>(typeLibFile.utf16()), REGKIND_NONE, &typelib);
    if (!typelib) {
        qWarning("dumpcpp: loading '%s' as a type library failed", qPrintable(typeLibFile));
        return false;
    }

    QString libName = nameSpace;
    if (libName.isEmpty()) {
        BSTR nameString = nullptr;
        if (SUCCEEDED(typelib->GetDocumentation(-1, &nameString, nullptr, nullptr, nullptr))) {
            libName = QString::fromWCharArray(nameString);
            SysFreeString(nameString);
        }
    }
    const QByteArray libNameBa = libName.toLatin1();
    vTableOnlyStubs = vTableOnlyStubsFromTypeLib(typelib, libName);

    QString libVersion(QLatin1String("1.0"));

    TLIBATTR *tlibattr = nullptr;
    typelib->GetLibAttr(&tlibattr);
    if (tlibattr) {
        libVersion = QString::fromLatin1("%1.%2").arg(tlibattr->wMajorVerNum).arg(tlibattr->wMinorVerNum);
        typelib->ReleaseTLibAttr(tlibattr);
    }

    if (outname.isEmpty())
        outname = libName.toLower();

    if (outname.isEmpty()) {
        qWarning("dumpcpp: no output filename provided, and cannot deduce output filename");
        return false;
    }

    QMetaObject *namespaceObject = qax_readEnumInfo(typelib, nullptr);

    QTemporaryFile classImplFile;
    if (!classImplFile.open()) {
        qWarning("dumpcpp: Cannot open temporary file.");
        return false;
    }
    QTextStream classImplOut(&classImplFile);
    QFile implFile(outname + QLatin1String(".cpp"));
    QTextStream implOut(&implFile);
    if (!(category & (NoMetaObject|NoImplementation))) {
        if (!implFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning("dumpcpp: Could not open output file '%s'", qPrintable(implFile.fileName()));
            return false;
        }

        implOut << "/****************************************************************************\n"
                   "**\n** Metadata for " << libName;
        formatCommentBlockFooter(typeLibFile, implOut);

        implOut << "#define QAX_DUMPCPP_" << libName.toUpper() << "_NOINLINES" << Qt::endl;

        implOut << "#include \"" << outname << ".h\"" << Qt::endl;
        implOut << "#include <OAIdl.h>" << Qt::endl; // For IDispatch
        implOut << Qt::endl;
        implOut << "using namespace " << libName << ';' << Qt::endl;
        implOut << Qt::endl;
    }

    QFile declFile(outname + QLatin1String(".h"));
    QTextStream declOut(&declFile);
    QByteArray classes;
    QTextStream classesOut(&classes, QIODevice::WriteOnly);
    QByteArray inlines;
    QTextStream inlinesOut(&inlines, QIODevice::WriteOnly);

    QMap<QByteArray, QByteArrayList> namespaces;

    if(!(category & NoDeclaration)) {
        if (!declFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning("dumpcpp: Could not open output file '%s'", qPrintable(declFile.fileName()));
            return false;
        }

        declOut << "/****************************************************************************\n"
                   "**\n** Namespace " << libName;
        formatCommentBlockFooter(typeLibFile, declOut);

        QFileInfo cppFileInfo(outname);
        writeHeader(declOut, libName, cppFileInfo.fileName());

        UINT typeCount = typelib->GetTypeInfoCount();
        if (declFile.isOpen()) {
            QByteArrayList opaquePointerTypes;
            declOut << Qt::endl;
            declOut << "// Referenced namespace" << Qt::endl;
            for (UINT index = 0; index < typeCount; ++index) {
                ITypeInfo *typeinfo = nullptr;
                typelib->GetTypeInfo(index, &typeinfo);
                if (!typeinfo)
                    continue;

                TYPEATTR *typeattr;
                typeinfo->GetTypeAttr(&typeattr);
                if (!typeattr) {
                    typeinfo->Release();
                    continue;
                }

                TYPEKIND typekind;
                typelib->GetTypeInfoType(index, &typekind);

                QMetaObject *metaObject = nullptr;

                // trigger meta object to collect references to other type libraries
                switch (typekind) {
                case TKIND_COCLASS:
                    if (category & ActiveX)
                        metaObject = qax_readClassInfo(typelib, typeinfo, &QWidget::staticMetaObject);
                    else
                        metaObject = qax_readClassInfo(typelib, typeinfo, &QObject::staticMetaObject);
                    break;
                case TKIND_DISPATCH:
                    if (category & ActiveX)
                        metaObject = qax_readInterfaceInfo(typelib, typeinfo, &QWidget::staticMetaObject);
                    else
                        metaObject = qax_readInterfaceInfo(typelib, typeinfo, &QObject::staticMetaObject);
                    break;
                case TKIND_RECORD:
                case TKIND_ENUM:
                case TKIND_INTERFACE: // only for forward declarations
                    {
                        QByteArray className = classNameFromTypeInfo(typeinfo);
                        switch (typekind) {
                        case TKIND_RECORD:
                            className.prepend("struct ");
                            break;
                        case TKIND_ENUM:
                            className.prepend("enum ");
                            break;
                        default:
                            break;
                        }
                        namespaces[libNameBa].append(className);
                        if (!qax_qualified_usertypes.contains(className))
                            qax_qualified_usertypes << className;
                    }
                    break;
                default:
                    break;
                }

                qax_deleteMetaObject(metaObject);
                typeinfo->ReleaseTypeAttr(typeattr);
                typeinfo->Release();
            }

            for (int i = 0; i < qax_qualified_usertypes.count(); ++i) {
                QByteArray refType = qax_qualified_usertypes.at(i);
                QByteArray refTypeLib;
                if (refType.contains("::")) {
                    refTypeLib = refType;
                    refType.remove(0, refType.lastIndexOf("::") + 2);
                    if (refTypeLib.contains(' ')) {
                        refType = refTypeLib.left(refTypeLib.indexOf(' ')) + ' ' + refType;
                    }
                    refTypeLib.truncate(refTypeLib.indexOf("::"));
                    refTypeLib.remove(0, refTypeLib.lastIndexOf(' ') + 1);
                    namespaces[refTypeLib].append(refType);
                } else {
                    namespaces[libNameBa].append(refType);
                }
            }

            for (auto it = namespaces.cbegin(), end  = namespaces.cend(); it != end; ++it) {
                const QByteArray &nspace = it.key();
                if (libName != QLatin1String(nspace)) {
                    declOut << "namespace " << nspace << " {" << Qt::endl;
                    for (const auto &className : it.value()) {
                        if (className.contains(' ')) {
                            writeForwardDeclaration(declOut, className);
                            namespaceForType.insert(className.mid(className.indexOf(' ') + 1), nspace);
                        } else {
                            declOut << "    class " << className << ';' << Qt::endl;
                            opaquePointerTypes.append(nspace + "::" + className);
                            namespaceForType.insert(className, nspace);
                            namespaceForType.insert(className + '*', nspace);
                            namespaceForType.insert(className + "**", nspace);
                        }
                    }
                    declOut << '}' << Qt::endl << Qt::endl;
                }
            }
            for (const QByteArray &opaquePointerType : qAsConst(opaquePointerTypes))
                declOut << "Q_DECLARE_OPAQUE_POINTER(" << opaquePointerType << "*)" << Qt::endl;
            declOut << Qt::endl;
        }
        generateNameSpace(declOut, namespaceObject, libNameBa);

        auto nspIt = namespaces.constFind(libNameBa);
        if (nspIt != namespaces.constEnd() && !nspIt.value().isEmpty()) {
            declOut << "// forward declarations" << Qt::endl;
            for (const auto &className : nspIt.value()) {
                if (className.contains(' ')) {
                    declOut << "    " << className << ';' << Qt::endl;
                    namespaceForType.insert(className.mid(className.indexOf(' ') + 1), libNameBa);
                } else {
                    declOut << "    class " << className << ';' << Qt::endl;
                    namespaceForType.insert(className, libNameBa);
                    namespaceForType.insert(className + '*', libNameBa);
                    namespaceForType.insert(className + "**", libNameBa);
                }
            }
        }

        declOut << Qt::endl;
    }

    QByteArrayList subtypes;

    UINT typeCount = typelib->GetTypeInfoCount();
    for (UINT index = 0; index < typeCount; ++index) {
        ITypeInfo *typeinfo = nullptr;
        typelib->GetTypeInfo(index, &typeinfo);
        if (!typeinfo)
            continue;

        TYPEATTR *typeattr;
        typeinfo->GetTypeAttr(&typeattr);
        if (!typeattr) {
            typeinfo->Release();
            continue;
        }

        TYPEKIND typekind;
        typelib->GetTypeInfoType(index, &typekind);

        ObjectCategories object_category = category;
        if (!(typeattr->wTypeFlags & TYPEFLAG_FCANCREATE))
            object_category |= SubObject;
        else if (typeattr->wTypeFlags & TYPEFLAG_FCONTROL)
            object_category |= ActiveX;

        QMetaObject *metaObject = nullptr;
        QUuid guid(typeattr->guid);

        if (!(object_category & ActiveX)) {
            QSettings settings(QLatin1String("HKEY_LOCAL_MACHINE\\Software\\Classes\\CLSID\\") + guid.toString(), QSettings::NativeFormat);
            if (settings.childGroups().contains(QLatin1String("Control"))) {
                object_category |= ActiveX;
                object_category &= ~SubObject;
            }
        }

        switch (typekind) {
        case TKIND_COCLASS:
            if (object_category & ActiveX)
                metaObject = qax_readClassInfo(typelib, typeinfo, &QWidget::staticMetaObject);
            else
                metaObject = qax_readClassInfo(typelib, typeinfo, &QObject::staticMetaObject);
            break;
        case TKIND_DISPATCH:
            if (object_category & ActiveX)
                metaObject = qax_readInterfaceInfo(typelib, typeinfo, &QWidget::staticMetaObject);
            else
                metaObject = qax_readInterfaceInfo(typelib, typeinfo, &QObject::staticMetaObject);
            break;
        case TKIND_INTERFACE: { // only stub: QTBUG-27792, explicitly disable copy in inherited
                                // class to make related error messages clearer
                const QByteArray className = classNameFromTypeInfo(typeinfo);
                declOut << "// stub for vtable-only interface\n"
                    << "class " << className << " : public QAxObject { Q_DISABLE_COPY_MOVE("
                    << className << ") };\n\n";
            }
            break;
        default:
            break;
        }

        if (metaObject) {
            currentTypeInfo = typeinfo;
            QByteArray className(metaObject->className());
            if (!(typeattr->wTypeFlags & TYPEFLAG_FDUAL)
                && (metaObject->propertyCount() - metaObject->propertyOffset()) == 1
                && className.contains("Events")) {
                declOut << "// skipping event interface " << className << Qt::endl << Qt::endl;
            } else {
                if (declFile.isOpen()) {
                    if (typeattr->wTypeFlags & TYPEFLAG_FLICENSED)
                        object_category |= Licensed;
                    if (typekind == TKIND_COCLASS) { // write those later...
                        generateClassDecl(classesOut, metaObject, className, libNameBa,
                                          object_category | NoInlines);
                        classesOut << Qt::endl;
                    } else {
                        generateClassDecl(declOut, metaObject, className, libNameBa,
                                          object_category | NoInlines);
                        declOut << Qt::endl;
                    }
                    subtypes << className;
                    generateClassDecl(inlinesOut, metaObject, className, libNameBa,
                                      object_category | OnlyInlines);
                    inlinesOut << Qt::endl;
                }
                if (implFile.isOpen())
                    generateClassImpl(classImplOut, metaObject, className, guid.toString(), libNameBa,
                                      object_category);
            }
            currentTypeInfo = nullptr;
        }

        qax_deleteMetaObject(metaObject);

        typeinfo->ReleaseTypeAttr(typeattr);
        typeinfo->Release();
    }

    // String table generation logic was ported from moc generator, with some modifications
    // required to split large stringdata arrays.
    if (!strings.isEmpty() && implFile.isOpen()) {
        //
        // Build stringdata struct
        //
        implOut << "struct qt_meta_stringdata_all_t {" << Qt::endl;
        implOut << "    QByteArrayData data[" << strings.size() << "];" << Qt::endl;

        QVector<QByteArrayList> listVector;
        QByteArrayList currentList;

        int currentTableLen = 0;
        for (const auto &s : strings) {
            currentTableLen += s.length() + 1;
            currentList.append(s);
            // Split strings into chunks less than 64k to work around compiler limits.
            if (currentTableLen > 60000) {
                implOut << "    char stringdata" << listVector.size() << '[' << currentTableLen + 1 << "];" << Qt::endl;
                listVector.append(currentList);
                currentList.clear();
                currentTableLen = 0;
            }
        }
        implOut << "    char stringdata" << listVector.size() << '[' << currentTableLen + 1 << "];" << Qt::endl;
        implOut << "};" << Qt::endl;
        listVector.append(currentList);

        // Macro that expands into a QByteArrayData. The offset member is
        // calculated from 1) the offset of the actual characters in the
        // stringdata.stringdata member, and 2) the stringdata.data index of the
        // QByteArrayData being defined. This calculation relies on the
        // QByteArrayData::data() implementation returning simply "this + offset".
        implOut << "#define QT_MOC_LITERAL(idx, ofs, len, table) \\" << Qt::endl
            << "    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \\" << Qt::endl
            << "    offsetof(qt_meta_stringdata_all_t, stringdata##table) + ofs \\" << Qt::endl
            << "        - idx * sizeof(QByteArrayData) \\" << Qt::endl
            << "    )" << Qt::endl;

        implOut << "static const qt_meta_stringdata_all_t qt_meta_stringdata_all = {" << Qt::endl;
        implOut << "    {" << Qt::endl;

        int totalStringCount = 0;
        for (int i = 0; i < listVector.size(); ++i) {
            int idx = 0;
            for (int j = 0; j < listVector[i].size(); j++) {
                if (totalStringCount)
                    implOut << ',' << Qt::endl;
                const QByteArray &str = listVector[i].at(j);
                implOut << "QT_MOC_LITERAL(" << totalStringCount++ << ", " << idx << ", " << str.length() << ", " << i << ')';
                idx += str.length() + 1;
            }
        }
        implOut << Qt::endl << "    }";

        //
        // Build stringdata arrays
        //
        for (const auto &l : listVector) {
            int col = 0;
            int len = 0;
            implOut << ',' << Qt::endl;
            implOut << "    \"";
            for (const auto &s : l) {
                len = s.length();
                if (col && col + len >= 150) {
                    implOut << '"' << Qt::endl << "    \"";
                    col = 0;
                } else if (len && s.at(0) >= '0' && s.at(0) <= '9') {
                    implOut << "\"\"";
                    len += 2;
                }
                int idx = 0;
                while (idx < s.length()) {
                    if (idx > 0) {
                        col = 0;
                        implOut << '"' << Qt::endl << "    \"";
                    }
                    int spanLen = qMin(150, s.length() - idx);
                    implOut << s.mid(idx, spanLen);
                    idx += spanLen;
                    col += spanLen;
                }

                implOut << "\\0";
                col += len + 2;
            }
            implOut << '"';
        }
        // Terminate stringdata struct
        implOut << Qt::endl << "};" << Qt::endl;

        implOut << "#undef QT_MOC_LITERAL" << Qt::endl << Qt::endl;

        classImplOut.flush();
        copyFileToStream(&classImplFile, &implOut);
        implOut << Qt::endl;
    }

    qax_deleteMetaObject(namespaceObject);

    classesOut.flush();
    inlinesOut.flush();

    if (declFile.isOpen()) {
        if (classes.size()) {
            declOut << "// Actual coclasses" << Qt::endl;
            declOut << classes;
        }
        if (inlines.size()) {
            declOut << "// member function implementation" << Qt::endl;
            declOut << "#ifndef QAX_DUMPCPP_" << libName.toUpper() << "_NOINLINES" << Qt::endl;
            declOut << inlines << Qt::endl;
            declOut << "#endif" << Qt::endl << Qt::endl;
        }
        // close namespace
        declOut << '}' << Qt::endl;
        declOut << Qt::endl;

        // partial template specialization for qMetaTypeCreateHelper and qMetaTypeConstructHelper
        declOut << "QT_BEGIN_NAMESPACE" << Qt::endl << Qt::endl;
        declOut << "namespace QtMetaTypePrivate {" << Qt::endl;
        for (int t = 0; t < subtypes.count(); ++t) {
            QByteArray subType(subtypes.at(t));

            declOut << "template<>" << Qt::endl;
            declOut << "struct QMetaTypeFunctionHelper<" << libName << "::" << subType << ", /* Accepted */ true> {" << Qt::endl;

            declOut << "    static void Destruct(void *t)" << Qt::endl;
            declOut << "    {" << Qt::endl;
            declOut << "        Q_UNUSED(t)" << Qt::endl; // Silence MSVC that warns for POD types.
            declOut << "        static_cast<" << libName << "::" << subType << "*>(t)->" << libName << "::" << subType << "::~" << subType << "();" << Qt::endl;
            declOut << "    }" << Qt::endl;

            declOut << "    static void *Construct(void *where, const void *t)" << Qt::endl;
            declOut << "    {" << Qt::endl;
            declOut << "        Q_ASSERT(!t);" << Qt::endl;
            declOut << "        Q_UNUSED(t)" << Qt::endl; // Silence warnings for release builds
            declOut << "        return new (where) " << libName << "::" << subType << ';' << Qt::endl;
            declOut << "    }" << Qt::endl;

            declOut << "#ifndef QT_NO_DATASTREAM" << Qt::endl;

            declOut << "    static void Save(QDataStream &stream, const void *t) { stream << *static_cast<const " << libName << "::" << subType << "*>(t); }" << Qt::endl;
            declOut << "    static void Load(QDataStream &stream, void *t) { stream >> *static_cast<" << libName << "::" << subType << "*>(t); }" << Qt::endl;

            declOut << "#endif // QT_NO_DATASTREAM" << Qt::endl;

            declOut << "};" << Qt::endl << Qt::endl;
        }
        declOut << "} // namespace QtMetaTypePrivate" << Qt::endl;
        declOut << "QT_END_NAMESPACE" << Qt::endl << Qt::endl;
        declOut << "#endif" << Qt::endl;
        declOut << Qt::endl;
    }

    typelib->Release();
    return true;
}

QT_END_NAMESPACE

QT_USE_NAMESPACE

struct Options
{
    Options() = default;

    ProgramMode mode = GenerateMode;
    ObjectCategories category = DefaultObject;
    bool dispatchEqualsIDispatch = false;

    QString outname;
    QString typeLib;
    QString nameSpace;
};

static void parseOptions(Options *options)
{
    const char helpText[] =
        "\nGenerate a C++ namespace from a type library.\n\n"
        "Examples:\n"
        "   dumpcpp -o ieframe %WINDIR%\\system32\\ieframe.dll\n"
        "   dumpcpp -o outlook Outlook.Application\n"
        "   dumpcpp {3B756301-0075-4E40-8BE8-5A81DE2426B7}\n"
        "   dumpcpp -getfile {21D6D480-A88B-11D0-83DD-00AA003CCABD}\n";

    const char outputOptionC[] = "-o";
    const char nameSpaceOptionC[] = "-n";
    const char getfileOptionC[] = "-getfile";

    QStringList args = QCoreApplication::arguments();
    // Convert Windows-style '/option' into '-option'.
    for (int i = 1; i < args.size(); ) {
        QString &arg = args[i];
        if (arg.startsWith(QLatin1Char('/')))
            arg[0] = QLatin1Char('-');
        const bool takesOptionValue = arg == QLatin1String(outputOptionC)
            || arg == QLatin1String(nameSpaceOptionC)
            || arg == QLatin1String(getfileOptionC);
        i += takesOptionValue ? 2 : 1;
    }

    QCommandLineParser parser;
    QCoreApplication::setApplicationVersion(QLatin1String(QT_VERSION_STR));
    parser.setApplicationDescription(QLatin1String(helpText));


    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption outputOption(QLatin1String(outputOptionC + 1),
                                    QStringLiteral("Write output to file."),
                                    QStringLiteral("file"));
    parser.addOption(outputOption);
    QCommandLineOption nameSpaceOption(QLatin1String(nameSpaceOptionC + 1),
                                       QStringLiteral("The name of the generated C++ namespace."),
                                       QStringLiteral("namespace"));
    parser.addOption(nameSpaceOption);
    QCommandLineOption noMetaObjectOption(QStringLiteral("nometaobject"),
                                          QStringLiteral("Don't generate meta object information (no .cpp file). The meta object is then generated in runtime."));
    parser.addOption(noMetaObjectOption);
    QCommandLineOption noDeclarationOption(QStringLiteral("impl"),
                                           QStringLiteral("Only generate the .cpp file."));
    parser.addOption(noDeclarationOption);
    QCommandLineOption noImplementationOption(QStringLiteral("decl"),
                                              QStringLiteral("Only generate the .h file."));
    parser.addOption(noImplementationOption);
    QCommandLineOption compatOption(QStringLiteral("compat"),
                                    QStringLiteral("Treat all coclass parameters as IDispatch."));
    parser.addOption(compatOption);
    QCommandLineOption getFileOption(QLatin1String(getfileOptionC + 1),
                                     QStringLiteral("Print the filename for the type library it to standard output."),
                                     QStringLiteral("id"));
    parser.addOption(getFileOption);
    parser.addPositionalArgument(QStringLiteral("input"),
                                 QStringLiteral("A type library file, type library ID, ProgID or CLSID."));
    parser.process(args);

    if (parser.isSet(outputOption))
        options->outname = parser.value(outputOption);
    if (parser.isSet(nameSpaceOption))
        options->nameSpace = parser.value(nameSpaceOption);
    if (parser.isSet(noMetaObjectOption))
         options->category |= NoMetaObject;
    if (parser.isSet(noDeclarationOption))
        options->category |= NoDeclaration;
    if (parser.isSet(noImplementationOption))
        options->category |= NoImplementation;
    options->dispatchEqualsIDispatch = parser.isSet(compatOption);
    if (parser.isSet(getFileOption)) {
        options->typeLib = parser.value(getFileOption);
        options->mode = TypeLibID;
    }
    if (!parser.positionalArguments().isEmpty())
        options->typeLib = parser.positionalArguments().first();

    if (options->mode == GenerateMode && options->typeLib.isEmpty()) {
        qWarning("dumpcpp: No object class or type library name provided.\n");
        parser.showHelp(1);
    }
}

int main(int argc, char **argv)
{
    if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED))) {
        qErrnoWarning("CoInitializeEx() failed.");
        return -1;
    }
    QCoreApplication app(argc, argv);

    Options options;
    parseOptions(&options);
    qax_dispatchEqualsIDispatch = options.dispatchEqualsIDispatch;
    QString typeLib = options.typeLib;

    if (options.mode == TypeLibID) {
        QSettings settings(QLatin1String("HKEY_LOCAL_MACHINE\\Software\\Classes\\TypeLib\\") +
                           typeLib, QSettings::NativeFormat);
        typeLib.clear();
        QStringList codes = settings.childGroups();
        for (int c = 0; c < codes.count(); ++c) {
            const QString keyPrefix = QLatin1Char('/') + codes.at(c) + QLatin1String("/0/");
            if (QT_POINTER_SIZE == 8) {
                typeLib = settings.value(keyPrefix + QLatin1String("win64/.")).toString();
                if (QFile::exists(typeLib))
                    break;
            }
            typeLib = settings.value(keyPrefix + QLatin1String("win32/.")).toString();
            if (QFile::exists(typeLib))
                break;
        }

        if (!typeLib.isEmpty())
            fprintf(stdout, "\"%s\"\n", qPrintable(typeLib));
        return 0;
    }

    if (typeLib.isEmpty()) {
        qWarning("dumpcpp: No object class or type library name provided.\n"
            "         Use -h for help.");
        return -1;
    }

    // not a file - search registry
    if (!QFile::exists(typeLib)) {
        bool isObject = false;
        QSettings settings(QLatin1String("HKEY_LOCAL_MACHINE\\Software\\Classes"), QSettings::NativeFormat);

        // regular string and not a file - must be ProgID
        if (typeLib.at(0) != QLatin1Char('{')) {
            CLSID clsid;
            if (CLSIDFromProgID(reinterpret_cast<const wchar_t *>(typeLib.utf16()), &clsid) != S_OK) {
                qWarning("dumpcpp: '%s' is not a type library and not a registered ProgID",
                         qPrintable(typeLib));
                return -2;
            }
            typeLib = QUuid(clsid).toString();
            isObject = true;
        }

        // check if CLSID
        if (!isObject) {
            QVariant test = settings.value(QLatin1String("/CLSID/") + typeLib + QLatin1String("/."));
            isObject = test.isValid();
        }

        // search typelib ID for CLSID
        if (isObject)
            typeLib = settings.value(QLatin1String("/CLSID/") + typeLib
                                     + QLatin1String("/Typelib/.")).toString();

        // interpret input as type library ID
        QString key = QLatin1String("/TypeLib/") + typeLib;
        settings.beginGroup(key);
        QStringList versions = settings.childGroups();
        QStringList codes;
        if (versions.count()) {
            settings.beginGroup(QLatin1Char('/') + versions.last());
            codes = settings.childGroups();
            key += QLatin1Char('/') + versions.last();
            settings.endGroup();
        }
        settings.endGroup();

        for (int c = 0; c < codes.count(); ++c) {
            const QString keyPrefix = key + QLatin1Char('/') + codes.at(c) + QLatin1Char('/');
            if (QT_POINTER_SIZE == 8) {
                typeLib = settings.value(keyPrefix + QLatin1String("win64/.")).toString();
                if (QFile::exists(typeLib))
                    break;
            }
            typeLib = settings.value(keyPrefix + QLatin1String("win32/.")).toString();
            if (QFile::exists(typeLib))
                break;
        }
    }

    if (!QFile::exists(typeLib)) {
        qWarning("dumpcpp: type library '%s' not found", qPrintable(typeLib));
        return -2;
    }

    if (!generateTypeLibrary(typeLib, options.outname, options.nameSpace, options.category)) {
        qWarning("dumpcpp: error processing type library '%s'", qPrintable(typeLib));
        return -1;
    }

    return 0;
}
