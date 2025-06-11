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
#include <functional>
#include <fstream>

#include "tools.hpp"

#ifdef USEACPP
#include "bench-acpp.hpp"
#endif

#ifdef USEKOKKOS
#include "bench-kokkos.hpp"
#endif

#define VERIFY 1

int main() {
  // Attempt to fill up L3 cache. Xeon R Gold has L3 = 22MB => 44 MB for two node setup
  // Vector size should be 3.8 times the cache = 83,6MB and total memory req = ?
  int N = 20e8; // 10e6 elements to fill one node.
  int NTIMES = 100;

  #if defined (USEACPP)
  auto pol = std::execution::par_unseq;
  auto time_vector = bench_acpp<int>(pol, N, NTIMES);
  #endif

  #if defined (USEKOKKOS)
  using ExecSpace = Kokkos::DefaultExecutionSpace;
  Kokkos::initialize();
    auto time_vector = bench_kokkos<ExecSpace, int>(N, NTIMES);
  Kokkos::finalize();
  #endif

  double duration = * std::min_element(time_vector.begin(), time_vector.end());
  double GBytes = (double (N) * sizeof(int)) * 1.0e-9;
  double bandwidth = ( GBytes ) / duration;

  std::ofstream file;
  file.open("/root/bench/cuda.txt", std::ios::out | std::ios::app);
  file << "\n# memory (MB)" << " bandwidth (GB/s)\n  "
       << GBytes * 1000 << "      " << bandwidth << std::endl;
  file.close();

  return 0;
}
