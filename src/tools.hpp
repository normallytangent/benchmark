/************************
 * Datatypes sizes test *
*************************/
void print_datatypes(){
  std::cout << "Printing datatype info...\nSizes of int: " << sizeof(int)
            << ", uint: " << sizeof(unsigned int) 
            << ", long: " << sizeof(long) 
            << ", size_t: " << sizeof(std::size_t) 
            << ", ssize_t: " << sizeof(ssize_t) << "\n";

  std::cout << "Numeric limits\n";
  std::cout  << "int : " << std::numeric_limits<int>::max() << "\n";
  std::cout  << "uint: " << std::numeric_limits<unsigned int>::max() << "\n";
  std::cout  << "long:  " << std::numeric_limits<long>::max() << "\n";
  std::cout  << "size: " << std::numeric_limits<std::size_t>::max() << "\n";
  std::cout  << "ssiz:  " << std::numeric_limits<ssize_t>::max() << "\n";
}

uint64_t get_time_now(){
  uint64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          std::chrono::high_resolution_clock::now()
                          .time_since_epoch()).count();
  return ns;
}