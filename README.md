s21_containers Project
=======================

This project involves the implementation of a C++ library, `s21_containers`, that provides basic standard container classes: list, map, queue, set, stack, and vector. The library also includes bonus container classes: array and multiset.

The aim of this project is to develop a deep understanding of C++ templates, iterators, and the Standard Template Library (STL).

Project Requirements
---------------------

* The project must be developed in C++17 using the gcc compiler.
* The project code must be located in the `src` folder and follow the Google style guide.
* The library classes must be implemented within the `s21` namespace.
* The library must provide a full set of standard methods and attributes for element handling, container capacity checking, and iteration.
* The use of STL implementation copying is not allowed.
* The logic of the STL must be followed in terms of checks, memory handling, and behavior in abnormal situations.

Project Structure
------------------

* `s21_containers.h`: The main header file that includes the header files of the implemented container classes.
* `s21_<container>.h`: Header files for each implemented container class.
* `src/`: Folder containing the source code of the project.
* `tests/`: Folder containing the unit-tests for the implemented container classes.

Bonus Features
--------------

* Implementation of the `s21_containersplus.h` library, which provides bonus container classes: array and multiset.
* Implementation of the `insert_many` method for the list, vector, queue, stack, map, set, and multiset containers.
