#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <algorithm> // for std::copy
#include <stdlib.h>  

int main() {
  double dou;
  thrust::host_vector<double> vec_cpu;
  	// Open the File
	std::ifstream in("stocks-google.txt");
 
	// Check if object is valid
	if(!in)
	{
		std::cerr << "Cannot open the File : "<<"stocks-google.txt"<<std::endl;
		return false;
	}
 
	std::string str;
	// Read the next line from File untill it reaches the end.
	while (std::getline(in, str))
	{
		// Line contains string of length > 0 then save it in vector
		if(str.size() > 0)
      double dou =atof(str);
			vec_cpu.push_back(dou);
	}
	//Close The File
	in.close();


  for ( int i =0; i<vec_cpu.size(); i++){
      std::cout<<vec_cpu[i];
    }

thrust::device_vector<double> vec_gpu (vec_cpu);
  

}
