file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    *.h
    *.cpp)

add_executable(TestDigitalIO ${SOURCES})

target_link_libraries(TestDigitalIO
    PRIVATE
        DigitalIO
        DigitalIO_Mocks
        GTest::gtest_main)

gtest_discover_tests(TestDigitalIO)