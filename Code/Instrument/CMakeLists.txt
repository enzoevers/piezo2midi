file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    *.h
    *.cpp)

add_executable(Piezo2MIDI ${SOURCES})

target_include_directories(Piezo2MIDI PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<INSTALL_INTERFACE:sdk/include>
)

target_link_libraries(Piezo2MIDI
    PRIVATE
)