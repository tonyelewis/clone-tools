# clone-tools [![Build Status](https://travis-ci.org/tonyelewis/clone-tools.svg?branch=master)](https://travis-ci.org/tonyelewis/clone-tools) [![Documentation Status](https://readthedocs.org/projects/clone-tools/badge/?version=latest)](http://clone-tools.readthedocs.io/en/latest/?badge=latest)

**TODOCUMENT** :

 * document all headers
 * all gubbins
 * examples on all pages
 * motivation and simple usage
 * comparisons with other clone_ptrs

A small, header-only Modern C++ library to make cloning (ie copying polymorphic types) easy

**User documentation**: at [Read the Docs](http://clone-tools.readthedocs.io/en/latest/ "The CATH Tools user documentation at Read the Docs")

**Code**: at [GitHub](https://github.com/tonyelewis/clone-tools "The clone-tools GitHub respository")

**Build and Execution of Tests**: at [Travis-CI](https://travis-ci.org/tonyelewis/clone-tools "The clone-tools Travis-CI builds")

Features
--------

 * Overhead free (ie modern compilers should compile `clone_ptr` to the same as `unique_ptr` for equivalent operations)

How to use
----------

Supported compilers
-------------------

The library should work on any conformant C++11 / C++14 compiler. At present, the tests are built and run on [Travis-CI](https://travis-ci.org/tonyelewis/clone-tools "The clone-tools Travis-CI builds") under:

 * Linux, Clang 3.5.0 (with libc++)
 * Linux, GCC 4.8.4 (with libstdc++)
 * Mac, Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
