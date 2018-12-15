#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <iostream>
#include <chrono>

typedef std::chrono::high_resolution_clock Time;
using namespace std::chrono;

int main() {

    // Read vector from file
    thrust::host_vector<double> host;

    double n;
    while(std::cin >> n) {
        host.push_back(n);
    }

    // Transfer to GPU
    /** TIME MEASURE **/
    Time::time_point t1 = Time::now();

    thrust::device_vector<double> dev(host);

    Time::time_point t2 = Time::now();
    duration<double> elapsed = duration_cast<milliseconds>(t2 - t1);
    /** END TIME MEASURE **/

    std::cout << elapsed.count() << " s" << std::endl;

    // Calc mean 
    double mean_value = thrust::reduce(dev.begin(), dev.end(), 0.0, thrust::plus<double>()) / dev.size();
    double mean_value_last_year = thrust::reduce(dev.end() - 259, dev.end(), 0.0, thrust::plus<double>()) / 259;
    double max_value = thrust::max_element(dev.begin(), dev.end());
    double min_value = thrust::min_element(dev.begin(), dev.end());
    std::cout << mean_value << std::endl;
    std::cout << mean_value_last_year << std::endl;
    std::cout << *max_value << std::endl;
    std::cout << *min_value << std::endl;

    return 0;

}
