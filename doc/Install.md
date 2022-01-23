# LB-t - A parallel 3D lattice-Boltzmann CFD solver

Author: [Tobit Flatscher](https://github.com/2b-t)

## 1. Installation

### 1.1 Install from binary

Stable releases for Windows can be downloaded as **executables from the [Releases section](https://github.com/2b-t/LB-t/releases/)**.

### 1.2 Build from source

Alternatively one can also build the code from the source with [CMake](https://cmake.org/). The following sections will walk you through the compilation on Windows and Linux. The only dependency is the VTK toolbox.

#### 1.2.1 Windows

The recommended build toolchain for Windows is based on Visual Studio in combination with CMake and the VTK library.

- As a first step make sure you have [**Visual Studio** 2019](https://visualstudio.microsoft.com/downloads/) or later installed. If this is not the case proceed to upgrade your version or install the [free community edition](https://visualstudio.microsoft.com/vs/community/).
- Grab yourself a copy of [**CMake**](https://cmake.org/download/) and install it on your system. For convenience there is an `.msi` installer available. 
- Additionally we will need **VTK**. Sadly there are no binary installers available and you will have to build VTK from source. This paragraph will briefly walk you through it but in case you have any issues you can have a look at the more detailed installation guide [here](https://vtk.org/Wiki/VTK/Building/Windows) or consult Youtube tutorials on it. On one particular system I had issues with the latest version VTK-9.03.0 so in case this happens to you just go with the older VTK-8.20.0 version.
  - You can download them from the [official webpage](https://vtk.org/download/).
  - Unzip the files in a folder of your choice. Be sure to not use the internal unzip functionality of Windows but instead use dedicated software like [7Zip](https://www.7-zip.org/download.html) as former can be very slow.
  - Start CMake and select the previously unzipped folder as source folder and a folder of your choice such as `VTK-build` as destination. Select the correct build toolchain (e.g. Visual Studio 2019) and select `Configure`, `Generate` and finally `Open Project`. This should open Visual Studio. Compile the solution (which might take a while). In case you get an error regarding the `pthread` library be sure to add `pthread` nudget to your project as described in this [StackOverflow post](https://stackoverflow.com/a/53011807).
- For parsing the settings file you will need the [nlohmann json parser](https://github.com/nlohmann/json). It is advised to use a [package manager](https://json.nlohmann.me/integration/package_managers/) such as vcpkg to install it. This can be done with `$ ./vcpkg install nlohmann-json`.
- Now that all dependencies are installed we can proceed to **compile this source code**.
  - Start of by downloading this code and unzip it to an arbitrary folder.
  - Then open a new **CMake GUI** and select the `src` folder of this repository as source and an arbitrary folder as destination. After clicking `Configure` you will likely get some error messages. Add the `VTK-build` folder to the corresponding CMake variable, clock `Configure` again, then `Generate` and finally `Open Project` and compile the executable. Be aware that if you are not linking statically you will have to add the VTK `DLL`s to the directory where your executable is located.

For the visualisation of the results I can recommend [Paraview](https://www.paraview.org/) which can be installed from [here](https://www.paraview.org/download/). An extensive installation guide for other Linux distributions can be found [here](https://www.paraview.org/Wiki/ParaView:Build_And_Install).

#### 1.2.2 Linux

- Install [Git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git) (e.g. for Debian-based distributions such as Ubuntu with `$ sudo apt install git-all`) or download the corresponding repositories manually instead of using the Git commands.

- Install CMake with the Snap package manager `$ sudo snap install cmake --classic`

- VTK will require [**GLVND**](https://github.com/NVIDIA/libglvnd). Proceed to install it as well:
  - First install the following dependencies: `$ sudo apt-get install libxext-dev libx11-dev x11proto-gl-dev`
  - Then clone the GLVND repository with `$ git clone https://github.com/NVIDIA/libglvnd`.
  - Open a new console inside the newly created folder `libglvnd`, and launch `$ ./autogen.sh`, followed by `$ ./configure` and `$ sudo make install`
  
- Now you should be ready to build **VTK** as described [here](https://vtk.org/Wiki/VTK/Building/Linux):
  - Clone the repository to a folder of your choice with `$ git clone https://gitlab.kitware.com/vtk/vtk.git VTK`
  - Create a folder where the build directory should be located with `$ mkdir VTK-build`, browse it with `$ cd VTK-build` and build it with `$ cmake -DCMAKE_BUILD_TYPE:STRING=Release ../VTK` where the last argument corresponds to the path of VTK (in this case a folder `VTK` relative to `VTK-build`)
  - Compile the library with `$ make -j8` where the number (in this case `8`) corresponds to the number of cores that should be used
  
- For parsing the settings file you will need the [**nlohmann json** parser](https://github.com/nlohmann/json). One could embed the package, reference it externally or embed it as [described here](https://json.nlohmann.me/integration/cmake/). I have chosen to use nlohmann as an external reference which means you will have to install it as a package, e.g. on Debian with `$ sudo apt-get install nlohmann-json3-dev`
  Alternatively you an clone the repository with `$ git clone https://github.com/nlohmann/json`, then browse the cloned folder `$ cd json` , use make and cmake to build and install it with `$ cmake .`, `$ make` and finally `$ sudo make install`.

- Finally you should be able to build this code
  - Clone this repository to a folder of your choice
  - Browse the main folder and add a new folder`build` right next to `src` and `test`.
  - Open a new console inside the `build` folder and build the code by typing `$ cmake -DVTK_DIR="../../VTK-build" ../src`, where the string following `VTK_DIR` should correspond to the location where the built VTK files are located, 
  - Finally compile the files by typing `$ make`, optionally with the flag `-j$(nproc)` to use the maximum number of cores available
  
  For the compilation you will need a C++17-compliant compiler. In case you are running an older Linux version such as Ubuntu 18.04 you might have to install a newer version of GCC as described [here](https://askubuntu.com/a/1149383) and set it as default as described [here](https://askubuntu.com/a/265180):  `$ sudo apt install g++-9` and `$ sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 9`.

For the visualisation of the results I can recommend [Paraview](https://www.paraview.org/) which can be installed on Ubuntu machines with `$ sudo apt-get install -y paraview`. An extensive installation guide for other Linux distributions can be found [here](https://www.paraview.org/Wiki/ParaView:Build_And_Install).
