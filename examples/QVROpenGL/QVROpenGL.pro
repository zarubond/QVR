# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = QVROpenGL

CONFIG += sailfishapp

QT += sensors

include(../../qvr.pri)

QMAKE_CXXFLAGS += -std=c++11

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
        src/cube.cpp

HEADERS  += src/mainwindow.h \
        src/cube.h

OTHER_FILES += rpm/QVROpenGL.changes.in \
    rpm/QVROpenGL.spec \
    rpm/QVROpenGL.yaml \
    QVROpenGL.desktop


