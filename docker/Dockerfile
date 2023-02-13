FROM ubuntu:20.04

WORKDIR /code

ARG DEBIAN_FRONTEND=noninteractive

# Install general tools: Git, Cmake, GCC, OpenMP
RUN apt-get update \
 && apt-get install -y \
    build-essential \
    cmake \
    git-all \
    libomp-dev \
 && rm -rf /var/lib/apt/lists/*

# Install VTK dependencies: libglvnd
RUN apt-get update \
 && apt-get install -y \
    libxext-dev \
    libx11-dev \
    x11proto-gl-dev \
 && git clone https://github.com/NVIDIA/libglvnd \
 && cd libglvnd \
 && apt-get install -y \
    autoconf \
    autogen \
    libtool \
    pkg-config \
 && ./autogen.sh \
 && autoreconf --install \
 && ./configure \
 && make install \
 && cd .. \
 && rm -rf /var/lib/apt/lists/*

# Compile VTK library
RUN apt-get update \
 && git clone https://gitlab.kitware.com/vtk/vtk.git VTK \
 && mkdir VTK-build \
 && cd VTK-build \
 && cmake -DCMAKE_BUILD_TYPE:STRING=Release ../VTK \
 && make -j$(nproc) \
 && cd ..

# Install nlohmann-json
RUN apt-get -y update \
 && apt-get install -y \
    nlohmann-json3-dev \
 && rm -rf /var/lib/apt/lists/*

# Install GTest
RUN apt-get update \
 && apt-get install -y  \
    libgtest-dev \
 && rm -rf /var/lib/apt/lists/*

# Attention, this breaks gcovr!
# Update to GCC 11 for C++20 support
#RUN apt-get update \
# && apt-get install -y  \
#    software-properties-common \
# && add-apt-repository ppa:ubuntu-toolchain-r/test \
# && apt-get install -y \
#    gcc-11 \
#    g++-11 \
# && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 11 \
# && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 11 \
# && rm -rf /var/lib/apt/lists/*

ENV DEBIAN_FRONTEND=dialog

