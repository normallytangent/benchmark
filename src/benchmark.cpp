// acpp-23.10.0/move-build/install/bin/acpp --acpp-targets="generic" -ltbb -O3
// -march=native --acpp-stdpar benchmark.cpp -o bench.acpp
// ACPP_VISIBILITY_MASK=omp ACPP_ADAPTIVITY_LEVEL=2 ./bench.acpp

// cmake -DCMAKE_CXX_COMPILER=clang++-18
//       -DKokkos_ROOT=<path-to-kokkos>/builddir/install
//       -DKokkos_DIR=<path-to-kokkos>/builddir/install/lib/cmake/Kokkos
//       ..

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
  int N = 10e8; // 10e6 elements to fill one node.
  int NTIMES = 100; // TODO

  #if defined (USEACPP)
  auto pmin = bench_acpp_min_element<int>(N, NTIMES);
  #endif

  #if defined (USEKOKKOS)
  using ExecSpace = Kokkos::DefaultExecutionSpace;
  Kokkos::initialize();
    auto pmin = bench_kokkos_min_element<ExecSpace, int>(N, NTIMES);
  Kokkos::finalize();
  #endif

  std::cout << "\n# memory (MB)" << " bandwidth (GB/s)\n  "
                << pmin.first * 1000 << "      " << pmin.second << std::endl;

  return 0;
}