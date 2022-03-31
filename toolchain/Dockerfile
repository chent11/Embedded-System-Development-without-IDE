FROM debian:bullseye as build
LABEL maintainer="Chen Tao t.clydechen@gmail.com"

ENV CT_VERSION crosstool-ng-1.25.0-rc1
ENV CCACHE_VERSION v4.6

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
    gcc \
    git \
    help2man \
    libncurses-dev \
    libtool-bin \
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

# Build ccache
WORKDIR /ccache
RUN git checkout ${CCACHE_VERSION}
RUN mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release -DHIREDIS_FROM_INTERNET=ON -DZSTD_FROM_INTERNET=ON .. && make -j`nproc` && cp ccache /toolchain/bin/

# Build crosstool-ng
WORKDIR /crosstool-ng
RUN git checkout ${CT_VERSION}
RUN ./bootstrap && ./configure --enable-local && make -j`nproc`
# Build GCC
COPY cortex-m4-eabi-hf-nano_nelib.config /crosstool-ng/.config
RUN ./ct-ng build

FROM debian:bullseye-slim as main

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get install -y \
    cmake \
    dumb-init \
    libncursesw5-dev \
    make \
    ninja-build
RUN apt-get clean autoclean && apt-get autoremove -y && rm -rf /var/lib/{apt,dpkg,cache,log}/

COPY --from=build /toolchain /toolchain

# ENV PREFIX arm-none-eabi-
ENV PREFIX_PATH /toolchain/arm-none-eabi
ENV PATH "/toolchain/bin:/toolchain/arm-none-eabi/bin:$PATH"
ENTRYPOINT [ "dumb-init", "--" ]