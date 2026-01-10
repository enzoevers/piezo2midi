file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    *.h
    *.cpp)

add_executable(TestADC ${SOURCES})

target_link_libraries(TestADC
    PRIVATE
        ADC
        GTest::gtest_main)

gtest_discover_tests(TestADC)