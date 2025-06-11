/****************************
 * Benchmarking AdaptiveCpp *
*****************************/

template <typename T, class Policy>
std::vector<double> bench_acpp(Policy&& pol, int N, int NTIMES) {

  T* data = new T[N];
  auto gen = [data, N](T idx) {
    data[idx] = (idx > 10) ? idx : N - idx; //CHANGEME
  };

  std::vector<std::size_t> indices(N);
  std::iota(indices.begin(), indices.end(), 0);
  std::for_each(pol, indices.begin(), indices.end(), gen);

  std::ofstream file;
  file.open("/root/bench/cuda.txt", std::ios::out | std::ios::app);
  file << "\n# AdaptiveCpp: std::min_element"; //CHANGEME
  file.close();

  auto myLambda = [=]() {
    // auto comp = std::less<>{};
    return std::min_element(pol, data, data + N); //CHANGEME
  };

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

  // cache warm-up
  auto res = myLambda();

  std::vector<double> time_vector;
  for (std::size_t k = 0; k < NTIMES; ++k) {
    auto start = get_time_now();
    res = myLambda();
    auto end = get_time_now();
    time_vector.push_back((end - start) * 1e-9);
  }

#ifdef VERIFY
  std::cout << "\n# Verification: " << *res
            << ", at:  " << std::distance(data, res);
#endif

  delete[] data;

  return time_vector;
}
