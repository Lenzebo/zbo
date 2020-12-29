# zbo

## CI Status
![C/C++ CI](https://github.com/Lenzebo/zbo/workflows/C/C++%20CI/badge.svg)

## Introduction
A collection of useful utilities that I use in different projects that are not domain specific.

## Structure
### cmake 
Common cmake scripts for clang-tidy, code coverage and conan dependency management

### docker
A dockerfile building a docker container for development and CI 
### zbo
C++ library containing: 
* `circular_range.h` Helper class to iterate over contiguous memory in a circular fashion
* `factory.h` A templated class to create a factory for a given interface with self-registering types
* `max_size_vector.h` A vector implementation compatible to stl algorithms that has a fixed compile-time maximum size
* `meta_enum.h` and `meta_enum_range.h` provide faciltities to create enum types that are printable, enumerable, etc... i.e. allow introspection on the enum type itself
* `named_type.h` provide a strong typedef facility to create type-safe interfaces
* `stop_watch.h` provide a class to measure time differences 
