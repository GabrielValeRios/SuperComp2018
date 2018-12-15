#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/functional.h>
#include <thrust/transform.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <math.h>

typedef std::chrono::high_resolution_clock Time;
using namespace std::chrono;

int main(){

	std::ifstream infile("stocks2.csv");
	int aV,mV;
	thrust::host_vector<double> cpu_values;
	while (infile >> aV >> mV){
		int dif = aV-mV;
		cpu_values.push_back(sqrt((dif*dif)));
	}
	Time::time_point t1 = Time::now();
	thrust::device_vector<double> gpu_values (cpu_values);
	Time::time_point t2 = Time::now();
	duration<double> time = duration_cast<duration<double>>(t2 - t1);
	std::cerr <<"Time: "<<"\n"<< time.count() << '\n';

	int sharesDifMean = thrust::reduce(gpu_values.begin(), gpu_values.end(), (int) 1, thrust::multiplies<int>());
	std::cout << "MEDIA: " << sharesDifMean <<"\n"; 
