file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    *.h
    *.cpp)

add_executable(TestChannelMapping ${SOURCES})

target_link_libraries(TestChannelMapping
    PRIVATE
        ChannelMapping
        ChannelMapping_Mocks
        PiezoPeakDetector_Mocks
        GTest::gtest_main)

gtest_discover_tests(TestChannelMapping)