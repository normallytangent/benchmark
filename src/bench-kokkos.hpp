/***********************
 * Benchmarking Kokkos *
************************/

#include <Kokkos_Core.hpp>
#include <Kokkos_StdAlgorithms.hpp>

template<class ExecSpace, typename T>
std::vector<double> bench_kokkos(int N, int NTIMES, std::ofstream& file) {

  namespace KE = Kokkos::Experimental;

  using MemSpace = typename ExecSpace::memory_space;
  using ViewType = Kokkos::View<T*, MemSpace>;

  ViewType data( "data", N );
  ViewType data2( "data2", N );
  auto gen = [N](T i){return i;}; //(i > (int)N/2 ? i : N-i);}; //CHANGEME
  Kokkos::parallel_for("fill", N, KOKKOS_LAMBDA(const int & i) {
    data(i) = gen(i);
  });

  file << "\n# Kokkos: KE::reverse_copy"; //CHANGEME

  auto myLambda = [=]() {
    KE::reverse_copy(ExecSpace(), KE::begin(data), KE::end(data), KE::begin(data2)); //CHANGEME
  };

  // cache warm-up
  myLambda();

  std::vector<double> time_vector;
  for (std::size_t k = 0; k < NTIMES; ++k) {
    auto start = get_time_now();
    myLambda();
    auto end = get_time_now();

    time_vector.push_back((end - start) * 1e-9);
  }

#ifdef VERIFY
  auto host_data = Kokkos::create_mirror_view_and_copy(Kokkos::HostSpace(), data);
  auto dist_to_res = KE::distance(KE::begin(data), res);
  std::cout << "\n# Verification: " << *(KE::begin(host_data) + dist_to_res)
            << ", at: " << dist_to_res;
#endif

  return time_vector;
}
