/****************************
 * Benchmarking AdaptiveCpp *
*****************************/

template <typename T, class Policy>
std::vector<double> bench_acpp(Policy&& pol, int N, int NTIMES, std::ofstream& file) {

  T* data = new T[N];
  auto gen = [=](T idx) {
    data[idx] = idx; //(idx > (int)N/2) ? idx : N - idx; //CHANGEME
  };

  std::vector<std::size_t> indices(N);
  std::iota(indices.begin(), indices.end(), 0);
  std::for_each(pol, indices.begin(), indices.end(), gen);

  file << "\n# AdaptiveCpp: std::find"; //CHANGEME

  auto myLambda = [=]() {
    return std::find(pol, data, data + N, 4200000); //CHANGEME
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
            << ", at:  " << std::distance(data, res)
	    << std::endl;
#endif

  delete[] data;

  return time_vector;
}
