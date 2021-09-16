# syntax=docker/dockerfile:1
FROM ubuntu:20.04

WORKDIR /code

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update

# Install general tools: Git, Cmake, GCC, OpenMP
RUN apt-get -y install git-all \
    && apt-get -y install cmake \
    && apt-get -y install build-essential \
    && apt-get -y install libomp-dev

# Install VTK dependencies: libglvnd
RUN apt-get -y install libxext-dev libx11-dev x11proto-gl-dev \
    && git clone https://github.com/NVIDIA/libglvnd \
    && cd libglvnd \
    && apt-get -y install autoconf autogen \
    && apt-get -y install pkg-config \
    && apt-get -y install libtool \
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
RUN apt-get -y install nlohmann-json3-dev

# Install googletest
RUN apt-get -y install libgtest-dev
