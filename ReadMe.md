# LB-t - A parallel 3D lattice-Boltzmann CFD solver

*Author: [Tobit Flatscher](https://github.com/2b-t) (December 2019 - October 2020)

[![Turbulent gaseous flow in porous media](/doc/PorousMedia_Re3750Sc1.jpeg)](https://www.youtube.com/watch?v=7SR4vhMnWZc "Turbulent gaseous flow in porous media")

## Overview
This is a **object-oriented parallel lattice-Boltzmann computational fluid dynamics (CFD) solver framework for arbitrary lattices (for any number of speed and spatial dimensions) based on C++17 and [OpenMP](https://www.openmp.org/)**. The files includes a Makefile for the Intel Compiler ICC as well as the Gnu Compiler GCC.

## Installation

### Linux (recommended)
Copy this folder or directly **clone this repository** by typing
```
$ git clone https://github.com/2b-t/LB-t.git
```
**Compile** the file with Make by typing 
```
$ make run
```
in your Linux shell or open the `LB-t.cbp` file in [Code::Blocks](http://www.codeblocks.org/). In case you have issues make sure Make is installed `sudo apt-get install build-essential`and your GCC version is greater than 8. You can install a newer GCC by typing `sudo apt install gcc-10`. 

If you want to change the compiler to the Intel compiler, first make sure the Intel compiler collection is installed on your system by opening a console and typing
```
$ icc -v
```
If it is not available on your system [you might be able to install it free of charge if you are a registered student](https://software.intel.com/en-us/compilers) (or buy an overpriced license). If it is available on your system you might change the corresponding lines of the Make-file `Makefile`
```
1  # Makefile
2  # Tobit Flatscher - github.com/2b-t (2020)
3
4  # Compiler settings (alternatively: export COMPILER=)
5  COMPILER = ICC
6
```
If you can't/don't want to afford their overpriced licenses and you are not a registered student, you can leave it on `GCC` and use the Gnu Compiler Collection only (or set-up a profile for another compiler in the lines below). From my experience the performance benefit of using ICC with this code is only marginal anyways.

### Windows
Under Windows you have to make sure an appropriate 64-bit compiler that supports C++17 is installed. Here some options
-[TDM-GCC](https://jmeubank.github.io/tdm-gcc/) inside [Code::Blocks](http://www.codeblocks.org/): Be sure that the version of TDM-GCC is 8 or higher and when selecting components don't forget to select `gcc/openmp`. Then continue to [setup TDM-GCC](http://forums.codeblocks.org/index.php?topic=21570.0) and [OpenMP](https://stackoverflow.com/a/58546530/9938686) inside Code::Blocks. Download this repository and open the Code::Blocks file. Be sure to change the selected compiler in `Project build options` to your newly configured 64-bit compiler and choose the policy `Append target options to project options`.
-[MSys2](https://www.msys2.org/) and Makefile: Download and install MSys2, open it and [download OpenMp by typing `pacman -S mingw-w64-x86_64-openmp`](https://packages.msys2.org/package/mingw-w64-x86_64-openmp?repo=mingw64) and [Make by typing `pacman -S mingw-w64-x86_64-make`](https://packages.msys2.org/package/mingw-w64-x86_64-make). Finally download the repository, browse it inside the MSys2 shell by typing `$ cd 'C:\<directory>'` and type `$ make run`.

For visualisation there are two options available: Either you can output `.vtk`-files and display them in [Paraview](https://www.paraview.org/) or export the results as `.bin` and use Matlab or Octave with the [simple visualisation file I have written](https://github.com/2b-t/CFD-visualisation.git).
Make sure that the latter plug-in is copied to the `output/` folder and the files are exported as `*.bin`.

## Implemented optimisations
-[Linear memory layout](https://www.springer.com/gp/book/9783319446479) with propietary vectorisation-friendly lattice numbering scheme
-Grid merging for storing several distinct populations next to each other for e.g. coupled turbulent scalar transport
-One-grid algorithm with [A-A pattern](https://www.doi.org/10.1109/ICPP.2009.38) for reduced memory bandwith and better parallel scalability
-Fused collision and streaming step and reversely applied boundary conditions for reduced memory bandwith and better parallel scalability
-Three dimensional [loop blocking](https://www.doi.org/10.1142/S0129626403001501) for improved cache-reuse and better parallel scalability
-64-byte cache-line alignment of all relevant arrays for vectorisation
-Manual `AVX2` and `AVX512` [intrinsics](https://www.apress.com/gp/book/9781484200643) collision kernels
-Frequent use of `const`, `constexpr`/`consteval` and `static` variables, `templates` as well as macros/pre-processor directives for compile time optimisations
-[Curiously Recurring Template Pattern (CRTP)](https://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c/) for compile-time static polymorphism
-Indexing functions as `inline` functions for reduced overhead
-Loop unrolling with compiler directives
-Parallelisation on multiple threads with [OpenMP](https://www.openmp.org/)

## Current features
-[D2Q9, D3Q19 and D3Q27 lattices](https://www.doi.org/10.1209/0295-5075/17/6/001)
-[BGK](https://www.doi.org/10.1103/PhysRev.94.511) and [TRT collision operators](http://global-sci.org/intro/article_detail/cicp/7862.html)
-[BGK with Smagorinsky turbulence model](https://arxiv.org/abs/comp-gas/9401004) for turbulent flows
-[Halfway bounce-back](https://www.doi.org/10.1007/BF02181482) boundaries for solid walls
-[Guo's interpolation](https://www.doi.org/910.1088/1009-1963/11/4/310) pressure and velocity boundaries
-Periodic boundary conditions (if nothing else specified)
-Export plug-ins to `.vtk` (slow) and `.bin` (fast)
-Designed with Resource acquisition is initialization (RAII) programming ideom
-Beginner-friendly documentation with [Doxygen](http://www.doxygen.nl/)

## Planned features
-Initialisation from a given velocity field including interpolation to finer grid
-Initialisation with non-equilibrium distribution estimate
-Output converter `.bin` <-> `.vtk`
-nVidia CUDA C++ implementation
-[Latt's regularised](https://www.doi.org/10.1103/PhysRevE.77.056703) pressure and velocity boundaries
-Entropic single relaxation time collision operator
-Cumulant collision operator
-[Interpolated bounce-back](https://www.doi.org/10.1063/1.1399290) for higher accuracy
-Additional advection-diffusion equation and corresponding boundary conditions
-Turbulent advection-diffusion
-Documentation and theory guide
-Extension to compressible multi-speed lattices

## History
I started with this project after programming a C-style framework (D2Q9 and D3Q19) with many more features for the [Institute of Thermal Engineering (IWT) - Graz University of Technology (Austria)](https://www.tugraz.at/en/institutes/iwt/home/) in terms of my Master's thesis and project work (2018-2019). While it had excellent performance (around 17 Mlups double precision BGK on a single core of a Intel i9-7920X, Intel Turbo Boost enabled) and scalability (around 145 Mlups on all twelve cores, Intel Turbo Boost enabled, with Turbo Boost disabled the parallel scalability is over 90%), I had hardcoded everything (e.g. manual loop unrolling) making the code very unflexible and difficult to maintain.
This code is a **work in progress** with the goal to achieve similar or better performance with modern C++ features making it significantly more generic. It is a personal project of mine with the main interest of pushing the performance of such methods while keeping the code generic. Thus, I have *not thoroughly tested* and verified the implementation. The main goal of this project is to aid people starting with their own LBM code by giving them idea how one could implement certain features in a generic way with compile time features without any overhead.
Currently the regular non-vectorised collision operators are around 15% slower than the C-style framework without the manual intrinsics collision operators and around 10% faster with them.

## Resources
For people interested in applying LBM to their problems there exist significantly more sophisticated and flexible implementations such as [OpenLB](https://www.openlb.net/) by Karlsruhe Institute of Technology (Germany) and [Palabos](https://palabos.unige.ch/) maintained by the University of Geneva (Switzerland), but at a cost of performance.
From a performance point of view the [highly optimised benchmark kernels by Erlangen Regional Computing Center RRZE (Germany)](https://github.com/RRZE-HPC/lbm-benchmark-kernels) are very interesting. For even more performance consider having a look at optimised implementations on architectures with greater bandwidth such as [Xeon Phi co-processors](https://doi.org/10.1002/cpe.5072) and [nVidia graphic accelerators](https://doi.org/10.1155/2017/1205892) with [CUDA](https://www.packtpub.com/application-development/cuda-cookbook).
If you want to know more about the theory behind the method I can recommend ["The Lattice Boltzmann Method - Principles and Practice" by T. Krüger et al.](https://www.springer.com/gp/book/9783319446479) that comes with its own [basic 2D code examples](https://github.com/lbm-principles-practice).
