QT       += core gui network multimedia websockets svg concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    opencvmanager.cpp \
    ffmpegmanager.cpp \
    main.cpp \
    playwidget.cpp \
    widget.cpp

HEADERS += \
    opencvmanager.h \
    ffmpegmanager.h \
    playwidget.h \
    widget.h

FORMS += \
    widget.ui

include($$PWD/FindDiffer/FindDiffer.pri)
include($$PWD/BaseUI/BaseUI.pri)

macx:{
    # /opt/homebrew/Cellar/opencv
    # OPENCV_PATH = /usr/local/opencv
    OPENCV_PATH = $$PWD/lib/opencv
    INCLUDEPATH += $${OPENCV_PATH}/include
    INCLUDEPATH += $${OPENCV_PATH}/include/opencv4
    INCLUDEPATH += $${OPENCV_PATH}/include/opencv4/opencv2

    # FFMPEG_PATH = /opt/homebrew/Cellar/ffmpeg/7.1.1_2
    FFMPEG_PATH = $$PWD/lib/ffmpeg
    INCLUDEPATH += $${FFMPEG_PATH}/include

    # 添加常用OpenCV库
    LIBS += -L$${OPENCV_PATH}/lib
    LIBS += $${OPENCV_PATH}/lib/libopencv_core.dylib
    LIBS += $${OPENCV_PATH}/lib/libopencv_imgproc.dylib
    LIBS += $${OPENCV_PATH}/lib/libopencv_imgcodecs.dylib
    LIBS += $${OPENCV_PATH}/lib/libopencv_highgui.dylib
    LIBS += $${OPENCV_PATH}/lib/libopencv_videoio.dylib

    # 添加常用ffmpeg库
    LIBS += -L$${FFMPEG_PATH}/lib \
            -lavcodec \
            -lavformat \
            -lavutil \
            -lswresample \
            -lswscale
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CONFIG += debug_and_release
CONFIG(debug, debug|release) {
DESTDIR = $$PWD/./bin/Debug/
}else{
DESTDIR = $$PWD/./bin/Release/
}

RESOURCES += \
    player.qrc
