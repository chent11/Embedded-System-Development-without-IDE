# Embedded System Development without IDE

We are losing control of our code on embedded system development with some modern fancy IDEs like KEIL, Eclipse, IAR...

It's may not a big deal if you are only focusing on the development itself, and it actually would increase developing efficiency, especially for beginners. But over-reliance on IDE would prevent you from learning deeper coding knowledge, so if you are an enthusiastic learner like me, you may like this project.

Indeed, I won't reinvent the wheel like some device drivers. I'll use the most popular and reliable library, CMSIS, for this project.

I plan to write an RTOS-based project, but I haven't decided on the main function of this project yet. It's just a led-blinker for now.

## How to build?

1. Clone and update all submodules.
    ```
    git clone --recursive https://github.com/chent11/Embedded-System-Development-without-IDE.git
    ```
2. Enter the repo's folder and build a cross-compiler environment from docker.
    ```
    docker build -t cross-compiler .
    ```
3. Use the docker-run script to build cmsis-dsp library.
    ```
    cd modules
    ../docker-run ./build_cmsis_dsp_lib.sh
    ```
4. Change the directory to the root of this repo, and use the docker-run script to build.
    ```
    ./docker-run make V=2
    ```
5. Flash (TBD)