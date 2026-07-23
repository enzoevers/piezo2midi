# Environment setup

## Formatter

Download `clang-format` as part of `LLVM`: https://releases.llvm.org/download.html. While installing LLVM make sure to check the box `Add LLVM to the system PATH for ...`. Make sure to install at least version `16` to have `InsertNewlineAtEOF` available.

> Note that you may have to close and re-open viusal studio to get the updated PATH.

The [`.clang-format`](.clang-format) file in this repo is generate with:

```sh
$ clang-format -style=llvm -dump-config > .clang-format
```

## Git hooks

Run:

```cmd
cd <this repo>
git config --local core.hooksPath ./GitHooks
```

## Format locally

To format the code locally, run:

```sh
$ cd <this repo>/Code
$ ./format.sh --in-place
```

## Build and run unit tests

```sh
$ cd <root of this repo>

$ cmake -S ./Code -B ./build -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
$ cmake --build ./build --config Debug
$ ctest --test-dir ./build --output-on-failure
```

## AVR toolchain

### Build in Docker

```sh
$ docker-compose -f Docker/DockerCompose.yaml run --rm avr-cross-compiler-ubuntu-2604
```

### Build locally

Getting all the required tools to build and flash the code:

```sh
$ cd <root of this repo>

$ python ./Scripts/DownloadAvrToolchain.py
$ python ./Scripts/DownloadAvrdude.py # For uploading the firmware
```

Then update your PATH

For bash/shell:

```sh
$ eval "$(python ./Scripts/UpdatePathWithAvrToolchain.py --print-command --shell bash)"
```

For powershell:

```ps1
> Invoke-Expression "$(python ./Scripts/UpdatePathWithAvrToolchain.py --print-command --shell powershell)"
```

For command prompt:

```cmd
> for /f "delims=" %i in ('python .\Scripts\UpdatePathWithAvrToolchain.py --print-command --shell cmd') do %i
```

Building the code (for the arduino uno in this example):

Bash:

```sh
# Still being root of this repo

$ currentPath=$(pwd)
$ cmake -G "Unix Makefiles" -S ./Code -B ./build \
    -DCMAKE_TOOLCHAIN_FILE="$currentPath/Code/CMake/AvrToolchain.cmake" \
    -DAVR_MCU=atmega328p \
    -DF_CPU=16000000L \
    -DAVR_SYSROOT=$currentPath/avr-toolchain/avr8-gnu-toolchain/avr/ \
    -DCMAKE_BUILD_TYPE=Debug
$ cmake --build ./build --config Debug -- -j
```

Powershell:

```ps1
# Still being root of this repo

> $currentPath = (Get-Location).Path
> cmake -G "Unix Makefiles" -S ./Code -B ./build `
    -DCMAKE_TOOLCHAIN_FILE="$currentPath/Code/CMake/AvrToolchain.cmake" `
    -DAVR_MCU=atmega328p `
    -DF_CPU=16000000L `
    -DAVR_SYSROOT=$currentPath/avr-toolchain/avr8-gnu-toolchain/avr/ `
    -DCMAKE_BUILD_TYPE=Debug
> cmake --build ./build --config Debug -- -j
```

To check the compiled assembly with the source code interleaved run:

```sh
$ avr-objdump -d -S ./build/TestingApp/TestingApp.elf
```

Upload the code (assuming the arduino is on COM3):

Bash:

```sh
# Still being root of this repo

$ ./avrdude/avrdude -c arduino -P COM3 \
    -b 115200 -p atmega328p \
    -D -U flash:w:./build/TestingApp/TestingApp.elf:e
```

Powershell:

```ps1
# Still being root of this repo

> ./avrdude/avrdude -c arduino -P COM3 `
    -b 115200 -p atmega328p `
    -D -U flash:w:./build/TestingApp/TestingApp.elf:e
```

Now you can use, for example, [PuTTY](https://putty.org/index.html) to interact with the device over serial.

## Resource usage report

Use `Scripts/analyze_resource_usage.py` to generate a JSON report with:

- Flash and RAM usage of the final application
- Flash and RAM usage per library (or per module when no static libraries are present)
- Biggest parts per library/module
- Optional symbol-size breakdowns for deeper analysis

If you use a Python virtual environment, activate it first (from repo root). Note that in different platforms the paths inside the virtual env might be different.

```sh
$ python -m venv .venv
$ source ./.venv/bin/activate
$ pip install -r ./Scripts/requirements.txt
```

Generate the report (from repo root):

```sh
$ python ./Scripts/analyze_resource_usage.py \
    --build-dir ./build \
    --elf ./build/InstrumentApp/InstrumentApp.elf \
    --output ./build/resource_usage_report.json
```

Include symbol sizes in the report:

```sh
$ python ./Scripts/analyze_resource_usage.py \
    --build-dir ./build \
    --elf ./build/InstrumentApp/InstrumentApp.elf \
    --output ./build/resource_usage_report.json \
    --include-symbol-sizes
```

Useful options:

- `--elf <path>`: analyze a specific ELF/executable
- `--top <N>`: number of biggest parts listed per library/module
- `--include-symbol-sizes`: add symbol-size analysis
- `--symbol-tool <path-or-name>`: override symbol tool (`nm`) executable
- `--symbols-top <N>`: number of biggest symbols kept per artifact/module

# Virtual destructor issue

AVR has an issue not not having libstdc++ linked in, so we can't have a virtual destructor.
