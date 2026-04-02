# Software architecture

## Folder structure and CMake

The code is split into multiple libraries. Each library may contain the following folders:

- MyLib
  - mocks/MyLib/mocks
  - private
  - public/MyLib
  - test

Directly in the following folders there will be a `CMakeLists.txt` file:

- MyLib
- MyLib/mocks
- MyLib/test

### MyLib/CMakeLists.txt

```cmake
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

add_library(MyLib STATIC)

target_include_directories(MyLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<INSTALL_INTERFACE:sdk/include>
)

target_sources(MyLib
    PUBLIC
        ${PUBLIC_SOURCES}
    PRIVATE
        ${PRIVATE_SOURCES}
)

target_link_libraries(MyLib
    PUBLIC
        <libs>
)

if(BUILD_TESTS)
    add_subdirectory(mocks)
    add_subdirectory(test)
endif()
```

### MyLib/mocks/CMakeLists.txt

```cmake
file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(MyLib_Mocks INTERFACE)

target_include_directories(MyLib_Mocks INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:sdk/include>
)

target_sources(MyLib_Mocks INTERFACE
    ${SOURCES}
)

target_link_libraries(MyLib_Mocks INTERFACE
    MyLib
    GTest::gmock
)
```

### MyLib/test/CMakeLists.txt

```cmake
file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    *.h
    *.cpp)

add_executable(TestMyLib ${SOURCES})

target_link_libraries(TestMyLib
    PRIVATE
        MyLib
        MyLib_Mocks
        GTest::gtest_main)

gtest_discover_tests(TestMyLib)
```

## Abstracting register interaction

The `IOProxy` template abstracts register access by parameterizing over register and define enums, as well as value types. For each hardware module, specialized interfaces are created by instantiating `IOProxy` with enums that represent the specific registers and defines relevant to that module (e.g., `UART_AVR_RegisterEnum`, `UART_AVR_DefineEnum`).

This approach allows the interface to be strongly typed and tailored to the hardware's register map, making code more robust and self-documenting. For example, `IIOProxy_UART_AVR` inherits from `IOProxy<UART_AVR_RegisterEnum, uint8_t, UART_AVR_DefineEnum, uint8_t>`, ensuring only valid register and define names can be used.

In production, concrete implementations of these interfaces perform actual hardware register reads and writes. In tests, mock classes (e.g., `Mock_IOProxy_UART_AVR`) implement the same interface, allowing tests to verify register interactions using frameworks like Google Mock. This enables unit testing of hardware logic by simulating register behavior and validating that the correct register operations are performed, without requiring real hardware.
