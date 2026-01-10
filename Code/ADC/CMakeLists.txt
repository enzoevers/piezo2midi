file(GLOB_RECURSE PUBLIC_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/public/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/public/*.cpp)

file(GLOB_RECURSE PRIVATE_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/private/*.cpp
)

add_library(ADC STATIC)

target_include_directories(ADC PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<INSTALL_INTERFACE:sdk/include>
)

target_sources(ADC
    PUBLIC
        ${PUBLIC_SOURCES}
    PRIVATE
        ${PRIVATE_SOURCES}
)

target_link_libraries(ADC
    PRIVATE
        Communication)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()
