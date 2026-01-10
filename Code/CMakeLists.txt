cmake_minimum_required(VERSION 3.28)
project(PIEZO2MIDI VERSION 0.0.1 LANGUAGES CXX)

option(BUILD_TESTS "Build the unit tests" OFF)

if(BUILD_TESTS)
    add_compile_definitions(BUILD_TESTS)

    include(FetchContent)
    FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    include(GoogleTest)
endif()

add_compile_definitions("$<$<NOT:$<CONFIG:Debug>>:NDEBUG>")

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if(BUILD_TESTS)
  add_definitions(
      -DF_CPU=16000000L
  )
endif()

add_subdirectory(IOProxy)
add_subdirectory(DigitalIO)
add_subdirectory(Communication)
#add_subdirectory(ADC)
#add_subdirectory(Instrument)


if(NOT BUILD_TESTS)
    add_subdirectory(TestingApp)
endif()
