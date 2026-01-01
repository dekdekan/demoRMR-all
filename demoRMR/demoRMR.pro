#-------------------------------------------------
#
# Project created by QtCreator 2018-02-11T14:35:38
#
#-------------------------------------------------

QT += core gui widgets
CONFIG += c++14

include($$PWD/../compile_defines.pri)

!contains(DEFINES, DISABLE_JOYSTICK) {
    message("Including joystick support")
    include($$PWD/../QJoysticks-master/QJoysticks.pri)
}

TARGET = demoRMR
TEMPLATE = app

win32 {
    LIBS += -lws2_32 -lWinmm
}

INCLUDEPATH += ../librobot
LIBS += -L../bin -llibrobot

!contains(DEFINES, DISABLE_OPENCV) {
    win32 {
        greaterThan(QT_MAJOR_VERSION, 5) {
            message(LINKING OpenCV for Qt6)
            INCLUDEPATH += C:/opencv_vc17/include
            LIBS += -LC:/opencv_vc17/x64/vc17/lib

            CONFIG(debug, debug|release) {
                LIBS += -lopencv_core4100d \
                        -lopencv_highgui4100d \
                        -lopencv_imgcodecs4100d \
                        -lopencv_imgproc4100d \
                        -lopencv_features2d4100d \
                        -lopencv_calib3d4100d \
                        -lopencv_videoio4100d \
                        -lopencv_ml4100d \
                        -lopencv_dnn4100d \
                        -lopencv_flann4100d \
                        -lopencv_objdetect4100d \
                        -lopencv_photo4100d \
                        -lopencv_video4100d
            } else {
                LIBS += -lopencv_core4100 \
                        -lopencv_highgui4100 \
                        -lopencv_imgcodecs4100 \
                        -lopencv_imgproc4100 \
                        -lopencv_features2d4100 \
                        -lopencv_calib3d4100 \
                        -lopencv_videoio4100 \
                        -lopencv_ml4100 \
                        -lopencv_dnn4100 \
                        -lopencv_flann4100 \
                        -lopencv_objdetect4100 \
                        -lopencv_photo4100 \
                        -lopencv_video4100
            }
        } else {
            message(LINKING OpenCV for Qt5)
            INCLUDEPATH += C:/opencv_vc16/include
            LIBS += -LC:/opencv_vc16/lib

            CONFIG(debug, debug|release) {
                LIBS += -lopencv_core440d \
                        -lopencv_highgui440d \
                        -lopencv_imgcodecs440d \
                        -lopencv_imgproc440d \
                        -lopencv_features2d440d \
                        -lopencv_calib3d440d \
                        -lopencv_videoio440d \
                        -lopencv_ml440d \
                        -lopencv_dnn440d \
                        -lopencv_flann440d \
                        -lopencv_objdetect440d \
                        -lopencv_photo440d \
                        -lopencv_shape440d \
                        -lopencv_video440d
            } else {
                LIBS += -lopencv_core440 \
                        -lopencv_highgui440 \
                        -lopencv_imgcodecs440 \
                        -lopencv_imgproc440 \
                        -lopencv_features2d440 \
                        -lopencv_calib3d440 \
                        -lopencv_videoio440 \
                        -lopencv_ml440 \
                        -lopencv_dnn440 \
                        -lopencv_flann440 \
                        -lopencv_objdetect440 \
                        -lopencv_photo440 \
                        -lopencv_shape440 \
                        -lopencv_video440
            }
        }
    }
    unix {
        CONFIG += link_pkgconfig
        PKGCONFIG += opencv4
    }
}
SOURCES += main.cpp\
        mainwindow.cpp \
        robot.cpp

HEADERS  += mainwindow.h \
    robot.h

FORMS    += mainwindow.ui
message(DEFINES in demoRMR: $$DEFINES)  # Debugging message
