TEMPLATE = app
TARGET = host-switcher
include(qxtglobalshortcut/qxtglobalshortcut.pri)
QT += core \
    gui \
    network
HEADERS += host_switcher.h \
    host_config.h \
    load_config_dialog.h \
    set_hotkey_dialog.h \
    hotkey_edit.h \
    about_dialog.h \
    load_preview_dialog.h
SOURCES += host_switcher.cpp \
    host_config.cpp \
    main.cpp \
    load_config_dialog.cpp \
    set_hotkey_dialog.cpp \
    hotkey_edit.cpp \
    about_dialog.cpp \
    load_preview_dialog.cpp
FORMS += host_switcher.ui \
    load_config_dialog.ui \
    set_hotkey_dialog.ui \
    about_dialog.ui \
    about_dialog.ui \
    load_preview_dialog.ui
RESOURCES += host_switcher.qrc
OTHER_FILES += main_icon.rc \
    hs-launcher \
    images/switch.png \
    install/hostswitcher.desktop \
    images/switch.icns \
    images/switch.ico \
    host-switcher.plist
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
mac { 
    QMAKE_INFO_PLIST = host-switcher.plist
    ICON = images/switch.icns
    LIBS += -framework Security
}

