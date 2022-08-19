# Gee

Gee is a framework for embedded system development in C++.

## Build

1. Clone and update all submodules.
    ```
    git clone --recursive https://github.com/chent11/Gee.git
    ```
2. Build the docker-cross-compiler environment or pull my built image.
    **Pulling the built image (recommended)**
    ```
    docker pull chent11/cross-compiler:cortex_m4-nano_newlib-hf-with-python
    ```
    **Building from source**
    This would take 20-90 minutes to build whole GCC from source, and you need change the image name in toolchain/docker_image to built image's name.
    ```
    cd toolchain
    docker build -t cross-compiler .
    ```
3. Change the directory to the root of this repo, and use the docker-cmd script to build.
    ```
    ./docker-cmd make V=2
    ```
4. Use the docker-cmd script to build cmsis-dsp library. (**Optional**)
    ```
    cd source/modules
    ../docker-cmd ./build_cmsis_dsp_lib.sh
    ```
5. Flash. You can modify the programmer in `Makefile`. Supported programmer are **jlink**, **openocd-jlink**, **openocd-stlink**.
    ```
    make upload
    ```

## Code Check

1. **Cpp Check**. This will check misra rules and some cppcheck default rules. You can modify these rules in `mk/rules.mk`
    ```
    make check-cppcheck
    ```
2. **Clang-tidy**. Checking rules in `.clang-tidy`.
    ```
    make check-clang-tidy
    ```
3. **Clang-format**. Checking rules in `clang-format`.
    ```
    make check-format
    ```
4. Checking all rules.
    ```
    make check-all
    ```

## Generate Compilation Database
Because some compilation database generating tools have some bugs on MacOS, I wrote a simple script(`toolchain/generate_compilation_database.py`) to do so for me. You can use it like this:
```
make generate-complication-database
```

## Debug
Open this repo with vscode and install extension [cortex-debug](https://github.com/Marus/cortex-debug), then switch to built-in debug side bar and choose a correct programmer type, click run button.
