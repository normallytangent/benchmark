/****************************
 * Benchmarking AdaptiveCpp *
*****************************/
#include "CustomIterator.hpp"

template <typename T>
std::pair<double, double> bench_acpp_min_element(int N, int NTIMES) {
  auto pol = std::execution::par_unseq;

  T* data = new T[N];
  auto gen = [data, N](T idx) {
    data[idx] = (idx > 10) ? idx : N - idx;
  };

  std::vector<std::size_t> indices(N);
  std::iota(indices.begin(), indices.end(), 0);
  std::for_each(pol, indices.begin(), indices.end(), gen);

  // Aksel:
  // A better, but more complex solution is to use
  // std::ranges::iota_view, or a custom iterator class
  // that just stores the current index and implements
  // dereferencing by just returning the index.
  // Then you can just use CustomIterator<size_t>{0, N}.begin()
  // and CustomIterator<size_t>{0, N}.end() in your for_each.
  // The lambda will then get the index as argument.

  // std::for_each(pol, CustomIterator<T>{0, N}.begin(),
  //               CustomIterator<T>{0, N}.end(), gen);

  auto comp = std::less<>{};
  // cache warm-up
  auto res = std::min_element(pol, data, data + N);

  std::vector<double> time_vector;
  for (std::size_t k = 0; k < NTIMES; ++k) {
    auto start = get_time_now();
    res = std::min_element(pol, data, data + N);
    auto end = get_time_now();
    time_vector.push_back((end - start) * 1e-9);
  }

  double duration = * std::min_element(time_vector.begin(), time_vector.end());
  double GBytes = (double (N) * sizeof(int)) * 1.0e-9;
  double bandwidth = ( GBytes ) / duration; // Note the changed bw since we measure
                                            // the minimum duration instead of avg.

  std::cout << "# AdaptiveCpp: std::min_element";
  std::cout << "\n# Verification: " << *res << ", at:  " << std::distance(data, res);
  delete[] data;
  return std::make_pair(GBytes, bandwidth);
}