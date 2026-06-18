#!/bin/bash

cmake -G "Unix Makefiles" -S /app/Code -B /app/build \
    -DCMAKE_TOOLCHAIN_FILE="/app/Code/CMake/AvrToolchain.cmake" \
    -DAVR_MCU=atmega328p \
    -DF_CPU=16000000L \
    -DAVR_SYSROOT=/app/avr-toolchain/avr8-gnu-toolchain/avr/ \
    -DCMAKE_BUILD_TYPE=Debug

cmake --build /app/build --config Debug -- -j

python3.14 /app/Scripts/analyze_resource_usage.py \
            --build-dir /app/build \
            --output /app/build/resource_usage_report.json \
            --include-symbol-sizes
