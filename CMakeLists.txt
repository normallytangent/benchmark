cmake_minimum_required(VERSION 3.10)
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

set(CMAKE_CXX_STANDARD 20)

project("bench.kokkos")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp -DUSEKOKKOS")

find_package(Kokkos 4.6 REQUIRED CONFIG)
add_executable(bench.kokkos "src/benchmark.cpp")
target_link_libraries(bench.kokkos PRIVATE Kokkos::kokkos)