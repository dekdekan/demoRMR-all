cmake_minimum_required(VERSION 3.16)
project(demoRMR VERSION 1.0 LANGUAGES CXX)

option(DISABLE_OPENCV "Disable OpenCV support" OFF)
option(DISABLE_SKELETON "Disable skeleton module" OFF)
option(DISABLE_JOYSTICK "Disable joystick support" ON)
option(DISABLE_AMCL "Disable AMCL localization" OFF)

# Use the locally installed OpenCV by default on Windows. An explicit
# -DOpenCV_DIR=... value still takes precedence.
if(WIN32 AND NOT DEFINED OpenCV_DIR)
    set(_default_opencv_dir "C:/OpenCV/opencv-4.14.0/opencv/build")
    if(EXISTS "${_default_opencv_dir}/OpenCVConfig.cmake")
        set(OpenCV_DIR "${_default_opencv_dir}" CACHE PATH
            "Directory containing OpenCVConfig.cmake")
    endif()
    unset(_default_opencv_dir)
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Network)

if(NOT DISABLE_OPENCV)
    find_package(OpenCV 4 REQUIRED COMPONENTS
        core
        highgui
        imgcodecs
        imgproc
        videoio
    )
endif()

find_package(librobot REQUIRED)

if(NOT DISABLE_JOYSTICK)
    add_subdirectory(QJoysticks-master QJoysticks_build)
endif()

add_subdirectory(demoRMR)
