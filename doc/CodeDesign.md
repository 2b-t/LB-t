# LB-t - A parallel 3D lattice-Boltzmann CFD solver

Author: [Tobit Flatscher](https://github.com/2b-t)

## Code design

How designed: Abstraction of memory and access patterns, one and two populations, separate static and dynamic interface, allow code to be compiled without VTK but with same functionality

Program all boundary conditions only once for all orientations and lattices, program collision operators once for all lattices: Templated
