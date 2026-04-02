file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(Communication_Mocks INTERFACE)

target_include_directories(Communication_Mocks INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:sdk/include>
)

target_sources(Communication_Mocks INTERFACE
    ${SOURCES}
)

target_link_libraries(Communication_Mocks INTERFACE
    Communication
    GTest::gmock
)
