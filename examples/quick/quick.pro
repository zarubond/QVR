TEMPLATE = app

QT += qml quick sensors

SOURCES += main.cpp \
    item.cpp \
    itemrenderer.cpp \
    cube.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
include(../../qvr.pri)

HEADERS += \
    item.h \
    itemrenderer.h \
    cube.h
