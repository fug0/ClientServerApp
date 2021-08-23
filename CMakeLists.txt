# CMakeList.txt : CMake project for MServer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("ClientServerApp")

add_subdirectory (Client)
add_subdirectory (Multithreaded_TCP_Server)