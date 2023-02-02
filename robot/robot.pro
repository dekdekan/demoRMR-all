CONFIG -= qt

TEMPLATE = lib
DEFINES += ROBOT_LIBRARY
win32 {
LIBS += -lws2_32
LIBS += -lWinmm
}
CONFIG += c++11
DESTDIR = ../bin


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CKobuki.cpp \
    robot.cpp \
    rplidar.cpp

HEADERS += \
    CKobuki.h \
    robot_global.h \
    robot.h \
    rplidar.h \
    szevent.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
