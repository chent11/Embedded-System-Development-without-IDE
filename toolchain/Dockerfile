FROM debian:bullseye as build
LABEL maintainer="Chen Tao t.clydechen@gmail.com"

ENV CT_VERSION crosstool-ng-1.25.0-rc1
ENV CCACHE_VERSION v4.6
ENV CMAKE_VERSION 3.22.3

RUN mkdir -p /toolchain/bin
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get install -y \
    autoconf \
    bison \
    cmake \
    curl \
    file \
    flex \
    g++ \
    gawk \
    gpg \
    gcc \
    git \
    help2man \
    libncurses-dev \
    libtool-bin \
    libssl-dev \
    make \
    pkg-config \
    texinfo \
    unzip \
    wget \
    xz-utils

# git clone ccache
RUN git clone --depth 1 --branch ${CCACHE_VERSION} https://github.com/ccache/ccache.git
# git clone crosstool-ng
RUN git clone --depth 1 --branch ${CT_VERSION} https://github.com/crosstool-ng/crosstool-ng.git
# download cmake source
RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.sh
RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-SHA-256.txt
RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-SHA-256.txt.asc

# Build ccache
WORKDIR /ccache
RUN mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release -DHIREDIS_FROM_INTERNET=ON -DZSTD_FROM_INTERNET=ON .. && make -j`nproc` && cp ccache /toolchain/bin/

# Build cmake
WORKDIR /
RUN gpg --keyserver hkps://keyserver.ubuntu.com --recv-keys C6C265324BBEBDC350B513D02D2CEF1034921684
RUN gpg --verify cmake-${CMAKE_VERSION}-SHA-256.txt.asc cmake-${CMAKE_VERSION}-SHA-256.txt
RUN sha256sum -c --ignore-missing cmake-${CMAKE_VERSION}-SHA-256.txt
RUN mkdir /toolchain/cmake
RUN /bin/bash cmake-${CMAKE_VERSION}-linux-x86_64.sh --skip-license --exclude-subdir --prefix=/toolchain/cmake

# Build crosstool-ng
WORKDIR /crosstool-ng
RUN ./bootstrap && ./configure --enable-local && make -j`nproc`
# Build GCC
COPY crosstool-ng.config /crosstool-ng/.config
RUN ./ct-ng build

FROM debian:bullseye-slim as main

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get install -y \
    dumb-init \
    libncursesw5-dev \
    libssl-dev \
    make \
    ninja-build
RUN apt-get clean autoclean && apt-get autoremove -y && rm -rf /var/lib/{apt,dpkg,cache,log}/

COPY --from=build /toolchain /toolchain

# ENV PREFIX arm-none-eabi-
ENV PREFIX_PATH /toolchain/arm-none-eabi
ENV PATH "/toolchain/bin:/toolchain/arm-none-eabi/bin:/toolchain/cmake/bin:$PATH"
ENTRYPOINT [ "dumb-init", "--" ]