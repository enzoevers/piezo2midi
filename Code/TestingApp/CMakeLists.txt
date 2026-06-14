file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    *.h
    *.cpp)

add_executable(TestingApp ${SOURCES})

set_target_properties(TestingApp PROPERTIES SUFFIX ".elf")

target_include_directories(TestingApp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:sdk/include>
)

target_link_libraries(TestingApp
    PRIVATE 
        DigitalIO
        Communication
        ADC
        Timing
        Interrupt
)