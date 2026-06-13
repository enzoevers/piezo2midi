file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    *.h
    *.cpp)

add_executable(TestTiming ${SOURCES})

target_link_libraries(TestTiming
    PRIVATE
        Timing
        Timing_Mocks
        Interrupt_Mocks
        GTest::gtest_main)

gtest_discover_tests(TestTiming)