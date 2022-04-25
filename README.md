# Embedded System Development with Docker

We are losing control of our code on embedded system development with some modern fancy IDEs like KEIL, Eclipse, IAR...

It's may not a big deal if you are only focusing on the development itself, and it actually would increase developing efficiency, especially for beginners. But over-reliance on IDE would prevent you from learning deeper coding knowledge, so if you are an enthusiastic learner like me, you may like this project.

I plan to write an RTOS-based project, but I haven't decided on the main function of this project yet. It's just a led-blinker for now.

## How to build?

1. Clone and update all submodules.
    ```
    git clone --recursive https://github.com/chent11/Embedded-System-Development-without-IDE.git
    cd Embedded-System-Development-without-IDE/toolchain
    ```
2. Build the docker-cross-compiler environment or pull my built image.
    ```
    # build from source (would take 20-90 minutes to build whole GCC from source)
    cd toolchain
    docker build -t cross-compiler .
    # pull the built image (recommended)
    docker pull chent11/cross-compiler:cortex-m4-hf
    ```
3. Use the docker-cmd script to build cmsis-dsp library.
    ```
    cd ../modules
    ../docker-cmd ./build_cmsis_dsp_lib.sh
    ```
4. Change the directory to the root of this repo, and use the docker-cmd script to build.
    ```
    cd ..
    ./docker-cmd make V=2
    ```
5. Flash (TBD)

## How to debug? (TBD)