cmake_minimum_required(VERSION 3.14)
project(lockfile C)

set(CMAKE_C_STANDARD 99)

add_executable(lockfile main.c lockfile.c lockfile.h)