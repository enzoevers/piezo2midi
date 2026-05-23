file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    *.h
    *.cpp)

add_executable(TestInterrupt ${SOURCES})

target_link_libraries(TestInterrupt
    PRIVATE
        Interrupt
        Interrupt_Mocks
        GTest::gtest_main)

gtest_discover_tests(TestInterrupt)