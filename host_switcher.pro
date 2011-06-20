TEMPLATE = app
TARGET = host-switcher
include(qxtglobalshortcut/qxtglobalshortcut.pri)
QT += core \
    gui
HEADERS += host_switcher.h \
    host_config.h
SOURCES += host_switcher.cpp \
    host_config.cpp \
    main.cpp
FORMS += host_switcher.ui
RESOURCES += host_switcher.qrc
OTHER_FILES += main_icon.rc \
    hs-launcher \
    images/switch.png \
    install/hostswitcher.desktop \
    images/switch.icns \
    images/switch.ico
win32:RC_FILE = main_icon.rc
unix { 
    images.files = images/*
    images.path = /usr/local/host-switcher/images
    INSTALLS += images
	desktop.files = install/hostswitcher.desktop
    desktop.path = /usr/share/applications
    INSTALLS += desktop
    launcher.files = hs-launcher
    launcher.path = /usr/local/host-switcher
    INSTALLS += launcher
    target.source = $$TARGET
    target.path = /usr/local/host-switcher
    INSTALLS += target
}
mac:ICON = images/switch.icns
