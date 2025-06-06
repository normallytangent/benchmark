/****************************
 * Benchmarking AdaptiveCpp *
*****************************/

template <typename T>
std::pair<double, double> bench_acpp_min_element(int N, int NTIMES) {

  int counter = 0;  
  auto gen = [N, &counter](int &i){
    i = (counter > 10 ?  counter : N-counter);
    counter++;
    };

  T* data = new T[N];
  std::for_each(data, data+N, gen);

  auto pol = std::execution::par_unseq;
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

  auto duration = std::min_element(time_vector.begin(), time_vector.end());
  double GBytes = (double (N) * sizeof(int)) * 1.0e-9;
  double bandwidth = ( GBytes ) / *duration; // Note the changed bw since we measure
                                             // the minimum duration instead of avg.

  std::cout << "# AdaptiveCpp: std::min_element";
  std::cout << "\n# Verification: " << *res << ", at:  " << std::distance(data, res);
  delete[] data;
  return std::make_pair(GBytes, bandwidth);
}