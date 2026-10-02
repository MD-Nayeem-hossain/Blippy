# Blippy Project
# Cross-platform text expander

QT += core gui widgets network qml quick quickcontrols2 svg

greaterThan(QT_MAJOR_VERSION, 4):CONFIG += c++17

CONFIG += c++14 debug

# The following define makes your messages fail to compile
# if you use the deprecated API.
DEFINES += QT_DISABLE_DEPRECATED_UP_TO=0x060000

# Input path
INCLUDEPATH += $$PWD/src \
               $$PWD/src/types \
               $$PWD/src/core \
               $$PWD/src/core/input \
               $$PWD/src/core/placeholder \
               $$PWD/src/core/placeholder \
               $$PWD/src/core/substitution \
               $$PWD/src/models \
               $$PWD/src/storage \
               $$PWD/src/network \
               $$PWD/src/ui

# Source files
SOURCES += \
    src/main.cpp \
    src/core/input/InputManager.cpp \
    src/core/input/InputHook.cpp \
    src/core/placeholder/PlaceholderEngine.cpp \
    src/core/substitution/SubstitutionEngine.cpp \
    src/models/ComboModel.cpp \
    src/storage/StorageManager.cpp \
    src/storage/ImportManager.cpp \
    src/network/SyncManager.cpp \
    src/network/UpdateManager.cpp

# Header files
HEADERS += \
    src/types/Types.h \
    src/core/input/InputManager.h \
    src/core/input/InputHook.h \
    src/core/placeholder/PlaceholderEngine.h \
    src/core/substitution/SubstitutionEngine.h \
    src/models/ComboModel.h \
    src/storage/StorageManager.h \
    src/storage/ImportManager.h \
    src/network/SyncManager.h \
    src/network/UpdateManager.h

# Resource collection
RESOURCES += \
    src/Blippy.qrc

# Platform-specific configurations
win32 {
    LIBS += -luser32 -lpsapi
    DEFINES += WIN32_LEAN_AND_MEAN
}

macos {
    LIBS += -framework ApplicationServices -framework Carbon -framework Cocoa
    DEFINES += MACOS
}

linux {
    LIBS += -lX11
    DEFINES += LINUX
}