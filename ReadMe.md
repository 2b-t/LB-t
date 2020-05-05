# LB-t - A parallel lattice-Boltzmann CFD solver

*Author: Tobit Flatscher (December 2019 - May 2020)

## Overview
This is a parallel lattice-Boltzmann computational fluid dynamics (CFD) solver framework based on C++17 and OpenMP.
**Work in progress...**

## Optimisations
- Linear memory layout with vectorisation-friendly numbering scheme
- Indexing with A-A pattern for reduced memory bandwith
- Three dimensional loop blocking for improved cache-reuse
- 64-byte cache-line alignment of all relevant arrays
- `AVX2` and `AVX512` manual intrinsics collision kernels
- Frequent use of `const` and `constexpr`, static variables, templates and macros/pre-processor directives for compile time optimisations
- Indexing functions as inline functions
- Loop unrolling with pre-processor directives
- Parallelisation on multiple threads with OpenMP

## Current features
- D3Q19 and D3Q27 lattices
- BGK and TRT collision operators
- Halfway bounce-back boundaries for solid walls
- Latt's regularised pressure and velocity boundaries
- Guo's interpolation pressure and velocity boundaries
- Periodic boundary conditions if nothing else specified
- Export plug-ins to VTK (slow) and BIN (fast)

## Planned features
- BGK with Smagorinsky turbulence model for turbulent flows
- Interpolated bounce-back for higher accuracy
