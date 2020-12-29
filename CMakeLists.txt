cmake_minimum_required(VERSION 3.16)
project(zbo)


set(CMAKE_CXX_STANDARD 20)

include(cmake/ClangTidy.cmake)
include(cmake/Conan.cmake)

conan_cmake_run(
        REQUIRES
        gtest/1.10.0
        OPTIONS
        BASIC_SETUP
        CMAKE_TARGETS
        BUILD
        missing)

enable_testing()
add_subdirectory(zbo)
