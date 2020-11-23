CXX_MODULE = webview
TARGET  = declarative_webview
TARGETPATH = QtWebView
IMPORT_VERSION = 1.$$QT_MINOR_VERSION

QT += qml quick webview-private
SOURCES += \
    $$PWD/webview.cpp

DYNAMIC_QMLDIR = \
    "module QtWebView" \
    "plugin declarative_webview" \
    "typeinfo plugins.qmltypes" \
    "classname QWebViewModule"
qtHaveModule(webengine):DYNAMIC_QMLDIR += "depends QtWebEngine 1.0"
load(qml_plugin)
webview_qrc = \
   "<!DOCTYPE RCC><RCC version=\"1.0\">" \
   "<qresource prefix=\"/qt-project.org/imports/QtWebView\">" \
   "<file alias=\"qmldir\">$$OUT_PWD/qmldir</file>" \
   "</qresource>" \
   "</RCC>"

write_file($$OUT_PWD/qmake_QtWebView.qrc, webview_qrc)|error()
RESOURCES = $$OUT_PWD/qmake_QtWebView.qrc
OTHER_FILES += qmldir
