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

add_library(DigitalIO STATIC)

target_include_directories(DigitalIO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<INSTALL_INTERFACE:sdk/include>
)

target_sources(DigitalIO
    PUBLIC
        ${PUBLIC_SOURCES}
    PRIVATE
        ${PRIVATE_SOURCES}
)

target_link_libraries(DigitalIO
    PUBLIC
        IOProxy
)

if(BUILD_TESTS)
    add_subdirectory(mocks)
    add_subdirectory(test)
endif()
