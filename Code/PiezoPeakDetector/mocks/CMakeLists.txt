file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(PiezoPeakDetector_Mocks INTERFACE)

target_include_directories(PiezoPeakDetector_Mocks INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:sdk/include>
)

target_sources(PiezoPeakDetector_Mocks INTERFACE
    ${SOURCES}
)

target_link_libraries(PiezoPeakDetector_Mocks INTERFACE
    PiezoPeakDetector
    GTest::gmock
)
