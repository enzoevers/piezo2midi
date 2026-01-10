file(GLOB_RECURSE PUBLIC_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/public/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/public/*.cpp)

add_library(IOProxy INTERFACE)

target_include_directories(IOProxy INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<INSTALL_INTERFACE:sdk/include>
)

target_sources(IOProxy
    INTERFACE
        ${PUBLIC_SOURCES}
)
