file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    *.h
    *.cpp)

add_executable(TestMIDI ${SOURCES})

target_link_libraries(TestMIDI
    PRIVATE
        MIDI
        Communication_Mocks
        GTest::gtest_main)

gtest_discover_tests(TestMIDI)