# cmake -DCMAKE_CXX_COMPILER=clang++-18 \
#       -DKokkos_ROOT=/root/kokkos-4.6.01/builddir/install \
#       -DKokkos_DIR=/root/kokkos-4.6.01/builddir/install/lib/cmake/Kokkos \
#       ..
cmake_minimum_required(VERSION 3.10)
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "clang++-18")

project("bench.kokkos")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp -DUSEKOKKOS")

find_package(Kokkos 4.6 REQUIRED CONFIG)
add_executable(bench.kokkos "src/benchmark.cpp")
target_link_libraries(bench.kokkos PRIVATE Kokkos::kokkos)