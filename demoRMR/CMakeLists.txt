set(SOURCES
    main.cpp
    mainwindow.cpp
    robot.cpp
)

set(HEADERS
    mainwindow.h
    robot.h
)

if(NOT DISABLE_JOYSTICK)
    add_compile_definitions(USING_QJOYSTICKS)
endif()

qt_add_executable(${PROJECT_NAME} WIN32
    ${SOURCES}
    ${HEADERS}
    mainwindow.ui
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ROBOT_LIBRARY)
if(DISABLE_OPENCV)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DISABLE_OPENCV)
endif()
if(DISABLE_SKELETON)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DISABLE_SKELETON)
endif()
if(DISABLE_JOYSTICK)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DISABLE_JOYSTICK)
endif()
if(DISABLE_AMCL)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DISABLE_AMCL)
else()
    configure_file(
        mapa.txt
        "${CMAKE_CURRENT_BINARY_DIR}/mapa.txt"
        COPYONLY
    )
endif()

target_link_libraries(${PROJECT_NAME} 
    PRIVATE librobot::librobot
    PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Qt6::Network
)

if(NOT DISABLE_JOYSTICK)
    target_link_libraries(${PROJECT_NAME} PRIVATE QJoysticks::QJoysticksLib)
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 Winmm)
endif()
