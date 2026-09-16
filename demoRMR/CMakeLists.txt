add_executable(demoRMR
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    robot.cpp
    robot.h
)

target_include_directories(demoRMR PRIVATE
    "${PROJECT_SOURCE_DIR}/librobot"
    "${PROJECT_SOURCE_DIR}/amcl/include"
)

target_link_libraries(demoRMR PRIVATE
    demoRMR_build_options
    librobot
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)

if(NOT DISABLE_AMCL)
    target_link_libraries(demoRMR PRIVATE amcl)
endif()

if(NOT DISABLE_OPENCV)
    target_link_libraries(demoRMR PRIVATE demoRMR_opencv)
endif()

if(NOT DISABLE_JOYSTICK)
    target_link_libraries(demoRMR PRIVATE QJoysticks)
endif()

if(WIN32)
    target_link_libraries(demoRMR PRIVATE ws2_32 winmm)

    if(NOT DISABLE_AMCL)
        add_custom_command(TARGET demoRMR POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "$<TARGET_FILE:amcl>"
                "$<TARGET_FILE_DIR:demoRMR>"
            VERBATIM
        )
    endif()

    if(TARGET QJoysticks_SDL2)
        add_custom_command(TARGET demoRMR POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "$<TARGET_FILE:QJoysticks_SDL2>"
                "$<TARGET_FILE_DIR:demoRMR>"
            VERBATIM
        )
    endif()
endif()

set_target_properties(demoRMR PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

install(TARGETS demoRMR RUNTIME DESTINATION bin)
