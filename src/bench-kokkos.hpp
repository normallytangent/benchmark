/***********************
 * Benchmarking Kokkos *
************************/
#include <Kokkos_Core.hpp>
#include <Kokkos_StdAlgorithms.hpp>

template<typename T>
std::pair<double, double> bench_kokkos_min_element(int N, int NTIMES) {
  Kokkos::initialize();
  {

    namespace KE = Kokkos::Experimental;
    auto exespace = Kokkos::DefaultExecutionSpace();
    Kokkos::View<T*, Kokkos::HostSpace> data("data", N);

    auto gen = [N](int i){return (i > 10 ? i : N-i);};
    Kokkos::parallel_for(N, KOKKOS_LAMBDA(int i) {
      data(i) = gen(i);
    });

    // cache warm-up
    auto result = KE::min_element(Kokkos::OpenMP(), KE::cbegin(data), KE::cend(data));
    auto start = get_time_now();
    for (std::size_t k = 0; k < NTIMES; ++k) {
        result = KE::min_element(Kokkos::OpenMP(), KE::cbegin(data), KE::cend(data));
    }
    auto end = get_time_now();

    double duration = (end - start)* 1e-9;
    double GBytes = (double (N) * sizeof(int)) * 1.0e-9;
    double bandwidth = ( GBytes * NTIMES ) / duration;

    std::cout << "# Kokkos: ";
    return std::make_pair(GBytes, bandwidth);
  }
  Kokkos::finalize();
}