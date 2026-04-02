# https://www.kuon.ch/post/2018-07-11-avr-cmake/
# CPU, you can find the list here: https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html
option(AVR_MCU "Target AVR microcontroller" atmega328p)
option(F_CPU "Target CPU frequency" 16000000L)
option(AVR_SYSROOT "AVR toolchain sysroot" "")

set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_SYSROOT ${AVR_SYSROOT})

set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_ASM_COMPILER avr-gcc)

message(STATUS "CMAKE_MAKE_PROGRAM = ${CMAKE_MAKE_PROGRAM}")

add_definitions(
    -DF_CPU=${F_CPU}
)

add_link_options(-mmcu=${AVR_MCU})
add_compile_options(-mmcu=${AVR_MCU})
set(CMAKE_EXE_LINKER_FLAGS -mmcu=${AVR_MCU})

# https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#cross-compiling-for-linux
# set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)