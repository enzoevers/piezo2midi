file(GLOB_RECURSE PUBLIC_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/public/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/public/*.cpp)

file(GLOB_RECURSE PRIVATE_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/private/*.cpp
)

if(BUILD_TESTS)
    list(FILTER PRIVATE_SOURCES EXCLUDE REGEX "${CMAKE_CURRENT_SOURCE_DIR}/private/hardware/.*")
endif()

add_library(Timing STATIC)

target_include_directories(Timing PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<INSTALL_INTERFACE:sdk/include>
)

target_sources(Timing
    PUBLIC
        ${PUBLIC_SOURCES}
    PRIVATE
        ${PRIVATE_SOURCES}
)

target_link_libraries(Timing
    PUBLIC
        IOProxy
        Interrupt
)

if(BUILD_TESTS)
    add_subdirectory(mocks)
    add_subdirectory(test)
endif()
