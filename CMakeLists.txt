cmake_minimum_required(VERSION 3.21)

project(demoRMR LANGUAGES CXX)

# OpenCV 4.10 still uses CMake's legacy FindCUDA module when CUDA support is
# present in the installed package.
if(POLICY CMP0146)
    cmake_policy(SET CMP0146 OLD)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

option(DISABLE_OPENCV "Build without OpenCV support" OFF)
option(DISABLE_SKELETON "Build without skeleton support" OFF)
option(DISABLE_JOYSTICK "Build without joystick support" ON)
option(DISABLE_AMCL "Build without AMCL support" ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

add_library(demoRMR_build_options INTERFACE)
target_compile_features(demoRMR_build_options INTERFACE cxx_std_11)
target_compile_definitions(demoRMR_build_options INTERFACE
    $<$<BOOL:${DISABLE_OPENCV}>:DISABLE_OPENCV>
    $<$<BOOL:${DISABLE_SKELETON}>:DISABLE_SKELETON>
    $<$<BOOL:${DISABLE_JOYSTICK}>:DISABLE_JOYSTICK>
    $<$<BOOL:${DISABLE_AMCL}>:DISABLE_AMCL>
    $<$<EQUAL:${QT_VERSION_MAJOR},6>:USING_QT6>
    $<$<EQUAL:${QT_VERSION_MAJOR},5>:USING_QT5>
)

if(NOT DISABLE_OPENCV)
    find_package(OpenCV 4 REQUIRED COMPONENTS
        core
        highgui
        imgcodecs
        imgproc
        features2d
        calib3d
        videoio
        ml
        dnn
        flann
        objdetect
        photo
        video
    )

    add_library(demoRMR_opencv INTERFACE)
    target_include_directories(demoRMR_opencv INTERFACE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(demoRMR_opencv INTERFACE ${OpenCV_LIBS})
endif()

if(NOT DISABLE_AMCL)
    add_library(amcl SHARED IMPORTED GLOBAL)
    if(WIN32)
        set_target_properties(amcl PROPERTIES
            IMPORTED_CONFIGURATIONS "DEBUG;RELEASE;RELWITHDEBINFO;MINSIZEREL"
            IMPORTED_IMPLIB_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/amcl/lib/amcld.lib"
            IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/amcl/lib/amcld.dll"
            IMPORTED_IMPLIB_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/amcl/lib/amcl.lib"
            IMPORTED_LOCATION_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/amcl/lib/amcl.dll"
            IMPORTED_IMPLIB_RELWITHDEBINFO "${CMAKE_CURRENT_SOURCE_DIR}/amcl/lib/amcl.lib"
            IMPORTED_LOCATION_RELWITHDEBINFO "${CMAKE_CURRENT_SOURCE_DIR}/amcl/lib/amcl.dll"
            IMPORTED_IMPLIB_MINSIZEREL "${CMAKE_CURRENT_SOURCE_DIR}/amcl/lib/amcl.lib"
            IMPORTED_LOCATION_MINSIZEREL "${CMAKE_CURRENT_SOURCE_DIR}/amcl/lib/amcl.dll"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/amcl/include"
        )
    else()
        find_library(AMCL_LIBRARY NAMES amcl REQUIRED HINTS "${CMAKE_CURRENT_SOURCE_DIR}/amcl/lib")
        set_target_properties(amcl PROPERTIES
            IMPORTED_LOCATION "${AMCL_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/amcl/include"
        )
    endif()
endif()

if(NOT DISABLE_JOYSTICK)
    add_subdirectory(QJoysticks-master)
endif()

add_subdirectory(librobot)
add_subdirectory(demoRMR)
