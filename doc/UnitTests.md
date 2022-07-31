# LB-t - A parallel 3D lattice-Boltzmann CFD solver

Author: [Tobit Flatscher](https://github.com/2b-t)

## 3. Unit testing

The code contains several unit tests and benchmarks to verify the correct functionality of the code. If you want to use the unittests please install [googletest](https://github.com/google/googletest) as described [here](https://github.com/google/googletest/blob/master/googletest/README.md). 

### 3.1 Linux

- Install googletest either by building it from source as described [here](https://github.com/google/googletest/blob/master/googletest/README.md) or install the corresponding Debian package with `$ sudo apt-get install libgtest-dev`.

- Proceed to build the test folder of this repository:
  - Creating a `build` folder next to the `src` and `test` folder
  - Opening a console inside the build folder and typing `$ cmake -DVTK_DIR="../../VTK-build" ../test`, then `$ make` and finally `$ ./run_lbt_tests`.

TO DO: Incorporate unit testing
