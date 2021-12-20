FROM ubuntu:20.04

WORKDIR /code

ARG DEBIAN_FRONTEND=noninteractive

# Install general tools: Git, Cmake, GCC, OpenMP
RUN apt-get -y update && apt-get -y install \
    build-essential \
    git-all \
    cmake \
    libomp-dev

# Install VTK dependencies: libglvnd
RUN apt-get -y update \
    && apt-get -y install libxext-dev libx11-dev x11proto-gl-dev \
    && git clone https://github.com/NVIDIA/libglvnd \
    && cd libglvnd \
    && apt-get -y install autoconf autogen libtool pkg-config \
    && ./autogen.sh \
    && autoreconf --install \
    && ./configure \
    && make install \
    && cd ..

# Compile VTK library
RUN git clone https://gitlab.kitware.com/vtk/vtk.git VTK \
    && mkdir VTK-build \
    && cd VTK-build \
    && cmake -DCMAKE_BUILD_TYPE:STRING=Release ../VTK \
    && make -j$(nproc) \
    && cd ..

# Install nlohmann-json
RUN apt-get -y update && apt-get -y install nlohmann-json3-dev

# Install googletest
RUN apt-get -y update && apt-get -y install libgtest-dev
