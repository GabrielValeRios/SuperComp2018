#include "produce_consumer.h"
#include <string>
#include <queue>
#include <chrono>
#include <fstream>
#include <iostream>

typedef std::chrono::high_resolution_clock Time;
using namespace std::chrono;

int main(int argc, char const *argv[])
{
    int num_threads;
    if(const char* env_p = std::getenv("NUM_THREADS")){
		num_threads = atoi(env_p);
        std::string allProdURL = argv[1]; //UrlProduto
        prodConsumer pc(100); //buffer_size

        std::ofstream file2;
        file2.open ("AverageTimePerProductParallel.txt");
        Time::time_point t1Total= Time::now();
        pc.callThreads(allProdURL,num_threads);
        Time::time_point t2Total= Time::now();
        pc.file.close();
        duration<double> TotalTime = duration_cast<duration<double>>(t2Total - t1Total);
        file2 <<"Average time per product: " <<"\n"<< TotalTime.count()/pc.nProducts << '\n';
        file2.close();
    }
    else
        std::cout << "ENV NUM_THREADS IS OBLIGATORY"<<"\n";


    return 0;
}