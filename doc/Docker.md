# LB-t - A parallel 3D lattice-Boltzmann CFD solver

Author: [Tobit Flatscher](https://github.com/2b-t)



## 2. Run from Docker

This code is shipped with a **[Docker](https://www.docker.com/) that allows the software to be run without having to install all the dependencies**. For this one has to [set-up Docker](https://docs.docker.com/get-docker/) (select your operating system and follow the steps) as well as [Docker Compose](https://docs.docker.com/compose/install/) ideally with `$ sudo pip3 install docker-compose`.

Then browse the `docker` folder containing all the different Docker files, open a console and start the docker with

```bash
$ sudo docker-compose up
```

and then - after the container had been compiled - open another terminal and connect to the Docker

```bash
$ sudo docker-compose exec lbt_docker sh
```

Now you can work inside the Docker as if it was your own machine. In the following paragraphs it is discussed how one can use [Visual Studio Code](https://code.visualstudio.com/) as an IDE in order to have a more convenient access. Advantages of Docker compared to an installation on the host system are discussed in more detail [here](https://hentsu.com/docker-containers-top-7-benefits/).

The current Dockerfile installs all dependencies from Debian packages. In case you would like to install them from source instead (e.g. in order to have the latest bug fixes) use the following Dockerfile:

```dockerfile
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
```

### 2.1 Graphic user interfaces inside the Docker

Docker was actually not designed to be used with a graphic user interface. There are several workarounds for this on Linux operating systems, most of them mount relevant X11 folders from the host system into the Docker. In our case this is achieved by a corresponding Docker Compose file `docker-compose-gui.yml` that [extends](https://docs.docker.com/compose/extends/) the basic `docker-compose.yml` file.

Before launching it one has to allow the user to access the X server from within the Docker with

```bash
$ xhost +local:root
```

Then one can open the Docker by additionally supplying the command line argument `-f <filename>`

```bash
$ docker-compose -f docker-compose-gui.yml up
```

#### 2.1.1 Hardware accelerated OpenGL with `nvidia-container-runtime`

Another problem emerges when wanting to use hardware acceleration such as with OpenGL. In such a case one has to allow the Docker to access the host graphics card. This can be achieved with the [`nvidia-docker`](https://github.com/NVIDIA/nvidia-docker) or alternatively with the [`nvidia-container-runtime`](https://github.com/NVIDIA/nvidia-container-runtime).

Latter was chosen for this Docker: The configuration files `docker-compose-gui-nvidia.yml` and `docker-compose-nvidia.yml` inside the `docker` folder contain Docker Compose configurations for accessing the hardware accelerators inside the Docker. Former is useful when running hardware-accelerated graphic user interfaces while the latter can be used to run CUDA inside the Docker.

For this start by launching `docker info` and check if the field `runtime` additionally to the default `runc` also holds an `nvidia` runtime. If not please follow the [installation guide](https://github.com/NVIDIA/nvidia-container-runtime#installation) as well as the [engine setup](https://github.com/NVIDIA/nvidia-container-runtime#docker-engine-setup) (and then restart your computer).

Then you should be able to run the Docker Compose image with

```bash
$ docker-compose -f docker-compose-gui-nvidia.yml up
```

To verify that the hardware acceleration is actually working you can check the output of `nvidia-smi`. If working correctly it should output you the available hardware accelerators on your system.

```bash
$ nvidia-smi
```

### 2.2 Docker inside Visual Studio Code

Additionally this repository comes with a [Visual Studio Code](https://code.visualstudio.com/) project. The following sections will walk you through how this can be set-up.

#### 2.2.1 Set-up

If you do not have Visual Studio Code installed on your system then [install it](https://code.visualstudio.com/download). And finally follow the Docker post-installation steps given [here](https://docs.docker.com/engine/install/linux-postinstall/) so that you can run Docker without `sudo`. Finally install the [Docker](https://marketplace.visualstudio.com/items?itemName=ms-azuretools.vscode-docker) and [Remote - Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) plugins inside Visual Studio Code and you should be ready to go.

#### 2.2.2 Open the project

More information about Docker with Visual Studio Code can be found [here](https://code.visualstudio.com/docs/containers/overview).

#### 2.2.3 Change the Docker Compose file

The Docker-Compose file can be changed inside `.devcontainer/devcontainers.json`:

```json
{
  "name": "LB-t Docker Compose",
  "dockerComposeFile": [
    "../docker/docker-compose.yml" // Change Docker Compose file here
  ],
```

This can be used to switch to a Docker with graphic output and/or hardware acceleration.

### 2.3 Performance with Docker

As can be seen from this [IBM Research report](https://dominoweb.draco.res.ibm.com/reports/rc25482.pdf) the performance overhead of using containers should be negligible when compared to running it directly on the host system.
