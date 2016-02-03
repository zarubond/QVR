QT += sensors

#https://www.google.com/get/cardboard/viewerprofilegenerator/

contains(QT, quick ) {
SOURCES += \
    $$PWD/vritem.cpp \
    $$PWD/vritemrenderer.cpp

HEADERS += \
    $$PWD/vritem.h \
    $$PWD/vritemrenderer.h
}

android{
    QT += androidextras
}

sailfishapp{
    #QMAKE_CXXFLAGS -= -std=c++11
    #QMAKE_CXXFLAGS += -std=c++0x
    DEFINES += Q_OS_SAILFISH
}

SOURCES += \
    $$PWD/vrdistortionmesh.cpp \
    $$PWD/vrrendergoggles.cpp \
    $$PWD/vrmagnetbutton.cpp \
    $$PWD/vrgoggles.cpp \
    $$PWD/vrrendersplit2d.cpp \
    $$PWD/vrrotationsensor.cpp \
    $$PWD/vrrenderfullscreen.cpp \
    $$PWD/vrprojection.cpp \
    $$PWD/vrdevices.cpp

    
HEADERS += \
    $$PWD/vrdistortionmesh.h \
    $$PWD/vrrendergoggles.h \
    $$PWD/vrmagnetbutton.h \
    $$PWD/vrgoggles.h \
    $$PWD/vrrendersplit2d.h \
    $$PWD/vrrotationsensor.h \
    $$PWD/vrrenderfullscreen.h \
    $$PWD/vrprojection.h \
    $$PWD/vrdevices.h

