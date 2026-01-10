file(GLOB_RECURSE PUBLIC_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/public/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/public/*.cpp)

file(GLOB_RECURSE PRIVATE_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/private/*.cpp
)

# Temporarily exclude SPI sources until they are implemented
list(FILTER PRIVATE_SOURCES EXCLUDE REGEX "${CMAKE_CURRENT_SOURCE_DIR}/private/SPI/.*")
list(FILTER PRIVATE_SOURCES EXCLUDE REGEX "${CMAKE_CURRENT_SOURCE_DIR}/public/SPI/.*")

if(BUILD_TESTS)
    list(FILTER PRIVATE_SOURCES EXCLUDE REGEX "${CMAKE_CURRENT_SOURCE_DIR}/private/.*/hardware/.*")
endif()

add_library(Communication STATIC)

target_include_directories(Communication PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<INSTALL_INTERFACE:sdk/include>
)

target_link_libraries(Communication
    PUBLIC
        DigitalIO
)

target_sources(Communication
    PUBLIC
        ${PUBLIC_SOURCES}
    PRIVATE
        ${PRIVATE_SOURCES}
)

if (BUILD_TESTS)
    add_subdirectory(mocks)
    add_subdirectory(test)
endif()
