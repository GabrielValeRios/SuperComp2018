
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <string>

int main() {
    thrust::host_vector<double> host(1,0);
    std::string line;
    while (std::getline(std::cin, line))
    {
	host.push_back(std::stod(line));
    }
    std::chrono::high_resolution_clock::time_point processT1 = std::chrono::high_resolution_clock::now(); 

    thrust::device_vector<double> dev(host);

    std::chrono::high_resolution_clock::time_point processT2 = std::chrono::high_resolution_clock::now(); 
    double totalProcessDuration = std::chrono::duration_cast<std::chrono::milliseconds>( processT2 - processT1 ).count();
    std::cout << totalProcessDuration << std::endl;
    double result = thrust::reduce(dev.end()-11, dev.end(),0,thrust::plus<double>());
    result /= 10;
    std::cout << result << std::endl;
}
