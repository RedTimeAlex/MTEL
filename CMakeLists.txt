#CMake min version
cmake_minimum_required(VERSION 3.10)

#projet propirties
project(mtel-project VERSION 0.0.1 LANGUAGES C)

#set output directs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(Source/MTEL)
add_subdirectory(Source/mtellib)
