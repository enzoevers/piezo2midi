file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(DigitalIO_Mocks INTERFACE)

target_include_directories(DigitalIO_Mocks INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:sdk/include>
)

target_sources(DigitalIO_Mocks INTERFACE
    ${SOURCES}
)

target_link_libraries(DigitalIO_Mocks INTERFACE
    DigitalIO
    GTest::gmock
)
