# LB-t - A parallel lattice-Boltzmann CFD solver

*Author: Tobit Flatscher (December 2019 - May 2020)

[![Turbulent gaseous flow in porous media](/doc/PorousMedia_Re3750Sc1.jpeg)](https://www.youtube.com/watch?v=7SR4vhMnWZc "Turbulent gaseous flow in porous media")

## Overview
This is a **three-dimensional parallel lattice-Boltzmann computational fluid dynamics (CFD) solver framework based on C++17 and [OpenMP](https://www.openmp.org/)**. The files includes a Makefile for the Intel Compiler ICC as well as the Gnu Compiler GCC.
It is a personal project of mine with the main interest of pushing the performance of such methods while keeping the code generic. Thus, I have *not thoroughly tested* and verified the implementation. The main goal of this project is to aid people starting with their own LBM code by giving them idea how one could implement certain features in a generic way with compile time features without losing a lot of performance.
I started with this project after programming a C-style framework (D2Q9 and D3Q19) with many more features for the [Institute of Thermal Engineering (IWT) - Graz University of Technology (Austria)](https://www.tugraz.at/en/institutes/iwt/home/) in terms of my Master's thesis and project work (2018-2019). While it had excellent performance (around 17 Mlups double precision BGK on a single core of a Intel i9-7920X, Intel Turbo Boost enabled) and scalability (around 145 Mlups on all twelve cores, Intel Turbo Boost enabled, with Turbo Boost disabled the parallel scalability is over 90%), I had hardcoded everything (e.g. manual loop unrolling) making the code very unflexible and difficult to maintain.
This code is a **work in progress** with the goal to achieve similar or better performance with modern C++ features making it significantly more generic.
While this code has a good performance it is though very limited, in particular it is limited to regular grids and in its current state can't make use of grid refinement, and I currently do not intend to implement much more functionality.
For people interested in applying LBM to their problems there exist significantly more sophisticated and flexible implementations such as [OpenLB](https://www.openlb.net/) by Karlsruhe Institute of Technology (Germany) and [Palabos](https://palabos.unige.ch/) maintained by the University of Geneva (Switzerland), but at a cost of performance.
From a performance point of view the [highly optimised benchmark kernels by Erlangen Regional Computing Center RRZE (Germany)](https://github.com/RRZE-HPC/lbm-benchmark-kernels) are very interesting. For even more performance consider having a look at optimised implementations on architectures with greater bandwidth such as [Xeon Phi co-processors](https://doi.org/10.1002/cpe.5072) and [nVidia graphic accelerators](https://doi.org/10.1155/2017/1205892) with [CUDA](https://www.packtpub.com/application-development/cuda-cookbook).
If you want to know more about the theory behind the method I can recommend ["The Lattice Boltzmann Method - Principles and Practice" by T. Krüger et al.](https://www.springer.com/gp/book/9783319446479) that comes with their own [basic 2D code examples](https://github.com/lbm-principles-practice).

## Installation
Copy this folder or directly **clone this repository** by typing
```
$ git clone https://github.com/2b-t/LB-t.git
```
**Compile** the file with Make by typing 
```
$ make run
```
in your Linux shell or open the `LB-t.cbp` file in [Code::Blocks](http://www.codeblocks.org/). In the latter case use the Release and not the Debug configuration and make sure that directories `backup/`, `output/bin/` and `output/vtk/` exist. In the case of the Makefile they are created automatically.
For visualisation there are two options available: Either you can output `.vtk`-files and display them in [Paraview](https://www.paraview.org/) or export the results as `.bin` and use Matlab or Octave with the [simple visualisation file I have written](https://github.com/2b-t/CFD-visualisation.git).
Make sure that the latter plug-in is copied to the `output/` folder and the files are exported as `*.bin`.

## Implemented optimisations
- [Linear memory layout](https://www.springer.com/gp/book/9783319446479) with propietary vectorisation-friendly lattice numbering scheme
- Indexing with [A-A pattern](10.1109/ICPP.2009.38) for reduced memory bandwith and better parallel scalability
- Three dimensional [loop blocking](10.1142/S0129626403001501) for improved cache-reuse and better parallel scalability
- 64-byte cache-line alignment of all relevant arrays for vectorisation
- `AVX2` and `AVX512` manual [intrinsics](https://www.apress.com/gp/book/9781484200643) collision kernels
- Frequent use of `const` and `constexpr`, `static` variables, `templates` and macros/pre-processor directives for compile time optimisations
- [Curiously Recurring Template Pattern (CRTP)](https://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c/) for compile-time static polymorphism
- Indexing functions as `inline` functions for reduced overhead
- Loop unrolling with pre-processor directives
- Parallelisation on multiple threads with [OpenMP](https://www.openmp.org/)

## Current features
- [D3Q19 and D3Q27 lattices](10.1209/0295-5075/17/6/001)
- [BGK](10.1103/PhysRev.94.511) and [TRT collision operators](http://global-sci.org/intro/article_detail/cicp/7862.html)
- [BGK with Smagorinsky turbulence model](https://arxiv.org/abs/comp-gas/9401004) for turbulent flows
- [Halfway bounce-back](10.1007/BF02181482) boundaries for solid walls
- [Guo's interpolation](910.1088/1009-1963/11/4/310) pressure and velocity boundaries
- Periodic boundary conditions (if nothing else specified)
- Export plug-ins to `.vtk` (slow) and `.bin` (fast)
- Beginner-friendly documentation with [Doxygen](http://www.doxygen.nl/)

## Planned features
- Reducing code redundancy by introducing functions for e.g. the calculating the equilibrium distribution
- Encapsulating collision operators into an appropriate class instead of using functions
- [Latt's regularised](10.1103/PhysRevE.77.056703) pressure and velocity boundaries
- [Interpolated bounce-back](10.1063/1.1399290) for higher accuracy
