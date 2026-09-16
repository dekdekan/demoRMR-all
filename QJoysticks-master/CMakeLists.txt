add_library(QJoysticks STATIC
    src/QJoysticks.cpp
    src/QJoysticks.h
    src/QJoysticks/Android_Joystick.cpp
    src/QJoysticks/Android_Joystick.h
    src/QJoysticks/JoysticksCommon.h
    src/QJoysticks/SDL_Joysticks.cpp
    src/QJoysticks/SDL_Joysticks.h
    src/QJoysticks/VirtualJoystick.cpp
    src/QJoysticks/VirtualJoystick.h
    etc/resources/qjoysticks-res.qrc
)

target_include_directories(QJoysticks
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL/include"
)
target_link_libraries(QJoysticks
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
)
target_compile_definitions(QJoysticks PRIVATE SDL_SUPPORTED)

if(WIN32)
    target_compile_definitions(QJoysticks PRIVATE SDL_WIN SDL_MAIN_HANDLED)

    if(MSVC)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(sdl_dir "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL/bin/windows/msvc/x64")
        else()
            set(sdl_dir "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL/bin/windows/msvc/x86")
        endif()

        add_library(QJoysticks_SDL2 SHARED IMPORTED GLOBAL)
        set_target_properties(QJoysticks_SDL2 PROPERTIES
            IMPORTED_IMPLIB "${sdl_dir}/SDL2.lib"
            IMPORTED_LOCATION "${sdl_dir}/SDL2.dll"
        )
        target_link_libraries(QJoysticks PRIVATE QJoysticks_SDL2)
    else()
        find_package(SDL2 REQUIRED)
        target_link_libraries(QJoysticks PRIVATE SDL2::SDL2)
    endif()
elseif(APPLE)
    find_package(SDL2 REQUIRED)
    target_link_libraries(QJoysticks PRIVATE SDL2::SDL2)
else()
    find_package(SDL2 REQUIRED)
    target_link_libraries(QJoysticks PRIVATE SDL2::SDL2)
endif()
