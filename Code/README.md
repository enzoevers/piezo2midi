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

```ps1
> .\Scripts\DownloadAndInstallMsys.ps1 # For make.exe
> .\Scripts\DownloadAvrToolchain.ps1 # for avr-gcc and avr-g++
> .\Scripts\UpdatePathWithAvrToolchain.ps1
```

```ps1
> cd <root of this repo>

> Set-ExecutionPolicy Unrestricted -Scope CurrentUse

> cmake -S ./Code -B ./build -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
> cmake --build ./build --config Debug
> ctest --test-dir ./build --output-on-failure
```

## AVR toolchain

Getting all the required tools to build and flash the code:

```ps1
> cd <root of this repo>

> Set-ExecutionPolicy Unrestricted -Scope CurrentUse

> .\Scripts\DownloadAvrToolchain.ps1
> .\Scripts\UpdatePathWithAvrToolchain.ps1
> .\Scripts\DownloadAvrdude.ps1
```

Building the code (for the arduino uno in this example):

```ps1
# Still being root of this repo

> $currentPath = (Get-Location).Path
> cmake -G "Unix Makefiles" -S ./Code -B ./build `
    -DCMAKE_TOOLCHAIN_FILE="$currentPath/Code/CMake/AvrToolchain.cmake" `
    -DAVR_MCU=atmega328p `
    -DF_CPU=16000000L `
    -DAVR_SYSROOT=$currentPath/avr8-gnu-toolchain/avr/ `
    -DCMAKE_BUILD_TYPE=Debug
> cmake --build ./build --config Debug -- -j
```

To check the compiled assembly with the source code interleaved run:

```ps1
> avr-objdump -d -S .\build\TestingApp\TestingApp.elf
```

Upload the code (assuming the arduino is on COM3):

```ps1
# Still being root of this repo

> avrdude/avrdude.exe -c arduino -P COM3 `
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

If you use a Python virtual environment, activate it first (from repo root):

```ps1
> python -m venv .venv
> .\.venv\Scripts\Activate.ps1
> pip install -r .\Scripts\requirements.txt
```

Generate the report (from repo root):

```ps1
> .\.venv\Scripts\python.exe .\Scripts\analyze_resource_usage.py `
    --build-dir .\build `
    --output .\build\resource_usage_report.json
```

Include symbol sizes in the report:

```ps1
> .\.venv\Scripts\python.exe .\Scripts\analyze_resource_usage.py `
    --build-dir .\build `
    --output .\build\resource_usage_report.json `
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
