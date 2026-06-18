file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    *.h
    *.cpp)

add_executable(TestPiezoPeakDetector ${SOURCES})

target_link_libraries(TestPiezoPeakDetector
    PRIVATE
        PiezoPeakDetector
        PiezoPeakDetector_Mocks
        GTest::gtest_main)

gtest_discover_tests(TestPiezoPeakDetector)