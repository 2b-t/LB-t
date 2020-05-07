# LB-t - A parallel lattice-Boltzmann CFD solver

*Author: Tobit Flatscher (December 2019 - May 2020)

## Overview
This is a parallel lattice-Boltzmann computational fluid dynamics (CFD) solver framework based on C++17 and OpenMP. The file includes a Makefile for ICC and GCC.
**Work in progress...**

## How to
Copy this folder or directly **clone this repository** by typing
```
$ git clone https://github.com/2b-t/LB-t.git
```
**Compile** the file with Make by typing 
```
$ make run
```
in your Linux shell or open the `LB-t.cbp` file in [Code::Blocks](http://www.codeblocks.org/). In the latter case make sure that directories `backup/`, `output/bin/` and `output/vtk/` exist. In the case of the Makefile they are created automatically.
For visualisation there are two options available: Either you can output `.vtk`-files and display them in [Paraview](https://www.paraview.org/) or export the results as `.bin` and use Matlab or Octave with the [simple visualisation file I have written](https://github.com/2b-t/CFD-visualisation.git).

## Optimisations
- Linear memory layout with vectorisation-friendly lattice numbering scheme
- Indexing with A-A pattern for reduced memory bandwith and better parallel scalability
- Three dimensional loop blocking for improved cache-reuse and better parallel scalability
- 64-byte cache-line alignment of all relevant arrays for vectorisation
- `AVX2` and `AVX512` manual intrinsics collision kernels
- Frequent use of `const` and `constexpr`, static variables, templates and macros/pre-processor directives for compile time optimisations
- Indexing functions as inline functions for reduced overhead
- Loop unrolling with pre-processor directives
- Parallelisation on multiple threads with OpenMP

## Current features
- D3Q19 and D3Q27 lattices
- BGK and TRT collision operators
- Halfway bounce-back boundaries for solid walls
- Periodic boundary conditions (if nothing else specified)
- Export plug-ins to `.vtk` (slow) and `.bin` (fast)

## Planned features
- Latt's regularised pressure and velocity boundaries
- Guo's interpolation pressure and velocity boundaries
- BGK with Smagorinsky turbulence model for turbulent flows
- Interpolated bounce-back for higher accuracy
