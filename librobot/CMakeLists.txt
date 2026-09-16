set(librobot_sources
    CKobuki.cpp
    CKobuki.h
    librobot.cpp
    librobot.h
    robot_global.h
    rplidar.cpp
    rplidar.h
    szevent.h
    udp_communication.cpp
    udp_communication.h
)

if(NOT DISABLE_SKELETON)
    list(APPEND librobot_sources skeleton.cpp skeleton.h)
endif()

add_library(librobot SHARED ${librobot_sources})
target_compile_definitions(librobot PRIVATE ROBOT_LIBRARY)
target_include_directories(librobot
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${PROJECT_SOURCE_DIR}/amcl/include"
)
target_link_libraries(librobot PUBLIC demoRMR_build_options)

if(NOT DISABLE_AMCL)
    target_link_libraries(librobot PUBLIC amcl)
endif()

if(NOT DISABLE_OPENCV)
    target_link_libraries(librobot PUBLIC demoRMR_opencv)
endif()

if(WIN32)
    target_link_libraries(librobot PRIVATE ws2_32 winmm)
endif()

set_target_properties(librobot PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

install(TARGETS librobot
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

