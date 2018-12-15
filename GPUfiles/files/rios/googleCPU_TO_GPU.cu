#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/functional.h>
#include <thrust/transform.h>
#include <iostream>
#include <fstream>
#include <chrono>

typedef std::chrono::high_resolution_clock Time;
using namespace std::chrono;

int main(){

	std::ifstream infile("stocks-google.txt");
	int value;
	thrust::host_vector<double> cpu_values;
	while (infile >> value){
		cpu_values.push_back(value);
	}
	Time::time_point t1 = Time::now();
	thrust::device_vector<double> gpu_values (cpu_values);
	Time::time_point t2 = Time::now();
	duration<double> time = duration_cast<duration<double>>(t2 - t1);
	std::cerr <<"Time: "<<"\n"<< time.count() << '\n';

	int sum_10 = thrust::reduce(thrust::host,gpu_values.begin(), gpu_values.end(), (int) 0, thrust::plus<int>());
	int mean_10 = sum_10/gpu_values.size();
	std::cerr << "MEAN_10: "<<mean_10<<"\n";
	
	int valid_days = 259;
	int sum_1 = thrust::reduce(thrust::host,gpu_values.begin(), gpu_values.begin()+valid_days, (int) 0, thrust::plus<int>());
	int mean_1 = sum_1/valid_days;
	std::cerr << "MEAN_1: "<<mean_1<<"\n";	
}
