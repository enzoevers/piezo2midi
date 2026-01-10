file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    *.h
    *.cpp)

add_executable(TestingApp ${SOURCES})

target_include_directories(TestingApp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:sdk/include>
)

target_link_libraries(TestingApp
    PRIVATE 
        DigitalIO
        Communication
)