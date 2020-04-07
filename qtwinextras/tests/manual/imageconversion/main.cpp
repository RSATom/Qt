/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
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

#include <QtWinExtras/QtWin>

#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QShortcut>
#include <QtWidgets/QVBoxLayout>

#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtCore/qt_windows.h>

static void formatData(QDebug d, const void *data, int size)
{
    QDebugStateSaver saver(d);
    d.noquote();
    d.nospace();
    d << "\nData: " << QByteArray(reinterpret_cast<const char *>(data), qMin(20, size)).toHex();
    if (size > 20)
        d << "...";
    d << "\n      0000000011111111222222223333333344444444";
}

QDebug operator<<(QDebug d, const BITMAP &b)
{
    QDebugStateSaver saver(d);
    d.nospace();
    d << "BITMAP(type=" << b.bmType << ", " << b.bmWidth << 'x' << b.bmHeight
        << ", widthBytes=" << b.bmWidthBytes << ", planes=" << b.bmPlanes
        << ", bitsPixel=" << b.bmBitsPixel << ", bits=" << b.bmBits << ')';
    return d;
}

QDebug operator<<(QDebug d, const BITMAPINFOHEADER &bih)
{
    QDebugStateSaver saver(d);
    d.nospace();
    d << "BITMAPINFOHEADER(" << bih.biWidth << 'x' << qAbs(bih.biHeight)
      << (bih.biHeight < 0 ? ", top-down" : ", bottom-up")
      << ", planes=" << bih.biPlanes << ", bitCount=" << bih.biBitCount
      << ", compression="  << bih.biCompression << ", size="
      << bih.biSizeImage << ')';
    return d;
}

static void formatImage(QDebug d, const QImage &image)
{
    QDebugStateSaver s(d);
    d.noquote();
    d.nospace();
    d << image;
    if (const int colorTableSize = image.colorCount()) {
        QVector<QRgb> colorTable = image.colorTable();
        d << " Color table: " << colorTableSize << " (" << showbase << hex; // 256 by standard
        int c = 0;
        for ( ; c < qMin(8, colorTableSize); ++c) {
            if (c)
                d << ", ";
            d << colorTable[c];
        }
        if (c < colorTableSize)
            d << "...";
        d << ')' << noshowbase << dec;
    }
    formatData(d, image.constBits(), image.byteCount());
}

enum ParseOptionResult {
    OptionError,
    OptionUnset,
    OptionSet
};

static ParseOptionResult parseIntOption(const QCommandLineParser &parser, const QCommandLineOption &option,
                                        int minValue, int maxValue, int *target)
{
    if (!parser.isSet(option))
        return OptionUnset;

    const QString spec = parser.value(option);
    bool ok;
    const int value = spec.toInt(&ok);
    if (!ok || value < minValue || value > maxValue) {
        qWarning() << "Invalid value" << spec << "for" << option.names();
        return OptionError;
    }
    *target = value;
    return OptionSet;
}

template <typename Enum>
static ParseOptionResult parseEnumOption(const QCommandLineParser &parser, const QCommandLineOption &option,
                                         Enum minValue, Enum maxValue, Enum *target)
{
    int intValue;
    const ParseOptionResult result = parseIntOption(parser, option, int(minValue), int(maxValue), &intValue);
    if (result == OptionSet)
        *target = static_cast<Enum>(intValue);
    return result;
}

// Display a QImage in a dialog.
class PreviewDialog : public QDialog
{
public:
    explicit PreviewDialog(const QImage &image, QWidget *parent = nullptr);
};

PreviewDialog::PreviewDialog(const QImage &image, QWidget *parent) : QDialog(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QString description;
    QDebug(&description) << image.size() << ", format=" << image.format();
    QLabel *descriptionLabel = new QLabel(description, this);
    descriptionLabel->setWordWrap(true);
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(descriptionLabel);
    auto *hLayout = new QHBoxLayout;
    QLabel *label = new QLabel(this);
    label->setFrameShape(QFrame::Box);
    label->setPixmap(QPixmap::fromImage(image));
    hLayout->addStretch();
    hLayout->addWidget(label);
    hLayout->addStretch();
    layout->addLayout(hLayout);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);
}

// Widget that paints a HBITMAP using GDI API in WM_PAINT.
class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(HBITMAP hBitmap, QWidget *p = nullptr) : QWidget(p), m_hBitmap(hBitmap) { }

    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

public slots:
    void saveBitmap();
    void convertBack();

protected:
    void contextMenuEvent(QContextMenuEvent *) override;

private:
    const HBITMAP m_hBitmap;
};

bool PaintWidget::nativeEvent(const QByteArray &eventType, void *messageIn, long *result)
{
    MSG *message = reinterpret_cast<MSG *>(messageIn);
    if (message->message != WM_PAINT)
        return QWidget::nativeEvent(eventType, message, result);

    PAINTSTRUCT paintStruct;
    BITMAP bitmap;

    const HDC hdc = BeginPaint(message->hwnd, &paintStruct);
    SelectObject(hdc, GetStockObject(BLACK_PEN));
    Rectangle(hdc, 1, 1, width() - 1, height() - 1);

    const HDC hdcMem = CreateCompatibleDC(hdc);
    const HGDIOBJ oldBitmap = SelectObject(hdcMem, m_hBitmap);

    GetObject(m_hBitmap, sizeof(bitmap), &bitmap);
    {
        QDebug d = qDebug();
        d << __FUNCTION__ << bitmap;
        formatData(d, bitmap.bmBits, bitmap.bmHeight * bitmap.bmWidthBytes);
    }
    BitBlt(hdc, 5, 5, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, oldBitmap);
    DeleteDC(hdcMem);

    EndPaint(message->hwnd, &paintStruct);
    return true;
}

void PaintWidget::convertBack()
{
    QImage image = QtWin::imageFromHBITMAP(m_hBitmap);
    formatImage(qDebug(), image);
    auto *dialog = new PreviewDialog(image, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setModal(false);
    dialog->setWindowTitle(QLatin1String("QImage - Qt ") + QLatin1String(QT_VERSION_STR));
    dialog->show();
}

void PaintWidget::saveBitmap()
{
    QImage image = QtWin::imageFromHBITMAP(m_hBitmap);
    formatImage(qDebug(), image);
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setMimeTypeFilters(QStringList(QStringLiteral("image/png")));
    fileDialog.setDefaultSuffix(QStringLiteral("png"));
    fileDialog.selectFile(QStringLiteral("test.png"));
    while (fileDialog.exec() == QDialog::Accepted) {
        const QString fileName = fileDialog.selectedFiles().first();
        if (image.save(fileName)) {
            qDebug().noquote() << "saved" << QDir::toNativeSeparators(fileName);
            break;
        }
        qWarning().noquote() << "Could not save" << QDir::toNativeSeparators(fileName);
    }
}

void PaintWidget::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu contextMenu;
    contextMenu.addAction(QStringLiteral("Convert into QImage"), this, &PaintWidget::convertBack);
    QAction *saveAction = contextMenu.addAction(QStringLiteral("Save"), this, &PaintWidget::saveBitmap);
    saveAction->setShortcut(Qt::CTRL + Qt::Key_S);
    contextMenu.exec(e->globalPos());
}

static const char description[] =
"\nCreates a HBITMAP from a QImage either passed as file name or by drawing in a\n"
"format determined by a command line option and draws it onto a native window\n"
"for comparison. Provides a context menu for converting the HBITMAP back to a\n"
"QImage and saving that for checking the reverse conversion.";

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("imageconversion");
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.setApplicationDescription("Qt Windows Extras Image Conversion Tester");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription(description);

    const QCommandLineOption formatOption(QStringList{"format", "f"},
                                          "QImage format", "format");
    parser.addOption(formatOption);
    const QCommandLineOption colorOption(QStringList{"color", "c"},
                                         "Fill color", "color-spec");
    parser.addOption(colorOption);
    const QCommandLineOption globalColorOption(QStringList{"globalColor", "g"},
                                               "Fill color (global color enum value)", "global-color");
    parser.addOption(globalColorOption);
    const QCommandLineOption widthOption(QStringList{"width", "w"},
                                         "Width", "width");
    parser.addOption(widthOption);
    const QCommandLineOption heightOption(QStringList{"height", "h"},
                                          "Height", "height");
    parser.addOption(heightOption);
    const QCommandLineOption previewOption(QStringList{"preview", "p"},
                                           "Show a preview");
    parser.addOption(previewOption);

    parser.addPositionalArgument("file", "The image file to open.");
    parser.process(app);

    QColor defaultColor(Qt::red);
    if (parser.isSet(colorOption)) {
        const QString spec = parser.value(colorOption);
        defaultColor = QColor(spec);
        if (!defaultColor.isValid()) {
            qWarning() << "Invalid color specification" << spec;
            return -1;
        }
    } else {
        // Color 0: color0, 1: color1, 2: black, 3: white, 7:red, 9: blue, 8: green
        Qt::GlobalColor globalColor = Qt::color0;
        if (!parseEnumOption(parser, globalColorOption, Qt::black, Qt::transparent, &globalColor))
            return -1;
        if (globalColor != Qt::color0)
            defaultColor = QColor(defaultColor);
    }

    // Format: 1: mono, 3: Indexed8, 7: RGB 16, 11: RGB555, 13: RGB888
    QImage::Format targetFormat = QImage::Format_ARGB32_Premultiplied;
    if (!parseEnumOption(parser, formatOption, QImage::Format_Mono, QImage::Format_Grayscale8, &targetFormat))
        return -1;
    // Can't paint on indexed nor mono, need transform?
    const QImage::Format drawFormat = targetFormat == QImage::Format_Indexed8
        || targetFormat == QImage::Format_Mono || targetFormat == QImage::Format_MonoLSB
        ? QImage::Format_ARGB32_Premultiplied : targetFormat;

    if (targetFormat == QImage::Format_Mono || targetFormat == QImage::Format_MonoLSB)
        defaultColor = Qt::white;

    int width = 73;
    int height = 57;
    if (!parseIntOption(parser, widthOption, 1, 2000, &width) || !parseIntOption(parser, heightOption, 1, 2000, &height))
        return -1;

    const bool preview = parser.isSet(previewOption);

    QImage image;
    if (!parser.positionalArguments().isEmpty()) {
        QString fileName = parser.positionalArguments().constFirst();
        image = QImage(fileName);
        if (image.isNull() || image.size().isEmpty()) {
            qWarning().noquote() << "Image load fail" << QDir::toNativeSeparators(fileName);
            return -1;
        }
    }

    if (image.isNull()) {
        qDebug() << "Default image color=" << defaultColor
            << showbase << hex << defaultColor.rgba() << noshowbase << dec
            << ", format=" << drawFormat;
        image = QImage(width, height, drawFormat);
        image.fill(defaultColor);
        QPainter painter(&image);
        painter.drawLine(0, 0, image.width(), image.height());
    }

    if (image.format() != targetFormat) {
        qDebug() << "Converting " << image.format() << targetFormat;
        image = image.convertToFormat(targetFormat);
    }

    formatImage(qDebug(), image);

    const HBITMAP bitmap = QtWin::imageToHBITMAP(image);
    if (!bitmap) {
        qWarning() << "Failed to create HBITMAP";
        return -1;
    }

    int exitCode = 0;
    {
        PaintWidget paintWidget(bitmap);
        auto *quitShortcut = new QShortcut(&paintWidget);
        quitShortcut->setKey(Qt::CTRL + Qt::Key_Q);
        quitShortcut->setContext(Qt::ApplicationShortcut);
        QObject::connect(quitShortcut, &QShortcut::activated, qApp, &QCoreApplication::quit);
        paintWidget.setWindowTitle(QLatin1String("HBITMAP - Qt ") + QLatin1String(QT_VERSION_STR));
        paintWidget.show();
        if (preview) {
            auto *dialog = new PreviewDialog(image);
            dialog->setModal(false);
            dialog->setWindowTitle(QLatin1String("QImage - Qt ") + QLatin1String(QT_VERSION_STR));
            dialog->move(paintWidget.frameGeometry().topRight() + QPoint(50, 0));
            dialog->show();
        }
        exitCode = app.exec();
    }

    DeleteObject(bitmap);

    return exitCode;
}

#include "main.moc"
