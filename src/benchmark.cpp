// acpp-23.10.0/move-build/install/bin/acpp --acpp-targets="generic" -ltbb -O3 -march=native --acpp-stdpar godbolt.cpp
// OMP_NUM_THREADS=32 ACPP_ADAPTIVITY_LEVEL=2 ./a.out -- ACPP_VISIBILITY_MASK=omp

#include <iostream>
#include <algorithm>
#include <execution>
#include <utility>
#include <chrono>

#include "tools.hpp"

#ifdef USEACPP
#include "bench-acpp.hpp"
#endif

#ifdef USEKOKKOS
#include "bench-kokkos.hpp"
#endif

int main() {
  // Attempt to fill up L3 cache. Xeon R Gold has L3 = 22MB => 44 MB for two node setup
  // Vector size should be 3.8 times the cache = 83,6MB and total memory req = ?
  int N = 20e7; // 10e6 elements to fill one node. 
  int NTIMES = 100; // TODO

  #if defined (USEACPP)
  auto pmin = bench_acpp_min_element<int>(N, NTIMES);

  std::cout << "\n# memory (MB)" << " bandwidth (GB/s)\n  "
                << pmin.first * 1000 << "      " << pmin.second << std::endl;
  #endif

  #if defined (USEKOKKOS)
  auto pmin = bench_kokkos_min_element<int>(N, NTIMES);

  std::cout << "std::min_element\n# memory (MB)" << " bandwidth (GB/s)\n  "
                << pmin.first * 1000 << "      " << pmin.second << std::endl;
  #endif

  return 0;
}