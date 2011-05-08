TEMPLATE = app
TARGET = host_switcher
QT += core \
    gui
HEADERS += host_switcher.h \
    host_config.h
SOURCES += host_switcher.cpp \
    host_config.cpp \
    main.cpp
FORMS += host_switcher.ui
RESOURCES += host_switcher.qrc

OTHER_FILES += \
    main_icon.rc

win32:RC_FILE = main_icon.rc
