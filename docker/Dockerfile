FROM ubuntu:22.04

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

# Install VTK
RUN apt-get update \
 && apt-get install -y \
    libvtk9-qt-dev \
 && rm -rf /var/lib/apt/lists/*

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

ENV DEBIAN_FRONTEND=dialog

