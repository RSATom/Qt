TARGET = qtwebview_winrt
QT += core gui webview-private
LIBS_PRIVATE += -lurlmon

PLUGIN_TYPE = webview
PLUGIN_CLASS_NAME = QWinrtWebViewPlugin

load(qt_plugin)

NO_PCH_SOURCES += \
    qwinrtwebview.cpp

SOURCES += \
    qwinrtwebviewplugin.cpp

HEADERS += \
    qwinrtwebview_p.h

WINDOWS_SDK_VERSION_STRING = $$(WindowsSDKVersion)
WINDOWS_SDK_VERSION = $$member($$list($$split(WINDOWS_SDK_VERSION_STRING, .)), 2)

lessThan(WINDOWS_SDK_VERSION, 16299) {
    DEFINES += QT_WINRT_URLMKGETSESSIONOPTION_NOT_AVAILABLE
    DEFINES += QT_WINRT_URLMKSETSESSIONOPTION_NOT_AVAILABLE
    DEFINES += QT_UCRTVERSION=$$WINDOWS_SDK_VERSION
}

OTHER_FILES +=

DISTFILES += \
    winrt.json
