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
TEMPLATE = app

TARGET = Calibration

#Sailfish
packagesExist(sailfishapp) {
    CONFIG += sailfishapp
}

QT += sensors quick

QMAKE_CXXFLAGS += -std=c++11

include(../../qvr.pri)

SOURCES += \
    src/main.cpp \
    src/cube.cpp \
    src/item.cpp \
    src/itemrenderer.cpp

HEADERS += \
    src/cube.h \
    src/item.h \
    src/itemrenderer.h

OTHER_FILES += \
    rpm/Calibration.changes.in \
    rpm/Calibration.spec \
    rpm/Calibration.yaml \
    Calibration.desktop \
    src/main.qml \
    src/DistortionGrid.qml

RESOURCES += \
    main.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android




