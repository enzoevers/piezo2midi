file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    *.h
    *.cpp)

add_executable(TestCommunication ${SOURCES})

target_link_libraries(TestCommunication
    PRIVATE
        Communication
        Communication_Mocks
        DigitalIO_Mocks
        GTest::gtest_main)

gtest_discover_tests(TestCommunication)