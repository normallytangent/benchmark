/***********************
 * Benchmarking Kokkos *
************************/
#include <Kokkos_Core.hpp>
#include <Kokkos_StdAlgorithms.hpp>

template<class ExecSpace, typename T>
std::pair<double, double> bench_kokkos_min_element(int N, int NTIMES) {

  namespace KE = Kokkos::Experimental;

  using MemSpace = typename ExecSpace::memory_space;
  using ViewType = Kokkos::View<T*, MemSpace>;

  ViewType data( "data", N );
  auto gen = [N](int i){return (i > 10 ? i : N-i);};
  Kokkos::parallel_for("fill", N, KOKKOS_LAMBDA(const int & i) {
    data(i) = gen(i);
  });

  auto host_data = Kokkos::create_mirror_view_and_copy(Kokkos::HostSpace(), data);

  // cache warm-up
  auto res = KE::min_element(ExecSpace(), KE::begin(data), KE::end(data));

  std::vector<double> time_vector;
  for (std::size_t k = 0; k < NTIMES; ++k) {
    auto start = get_time_now();
    res = KE::min_element(ExecSpace(), KE::begin(data), KE::end(data));
    auto end = get_time_now();

    time_vector.push_back((end - start) * 1e-9);
  }

  double duration = * std::min_element(time_vector.begin(), time_vector.end());
  double GBytes = (double (N) * sizeof(int)) * 1.0e-9;
  double bandwidth = ( GBytes ) / duration;

  auto dist_to_res = KE::distance(KE::begin(data), res);
  std::cout << "# Kokkos: std::min_element";
  std::cout << "\n# Verification: " << *(KE::begin(host_data) + dist_to_res)
            << ", at: " << dist_to_res;

  return std::make_pair(GBytes, bandwidth);
}