# Gee

Gee is a framework for embedded system development in C++.

## How to build?

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
5. Flash (TBD)

## How to debug? (TBD)
