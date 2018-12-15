#include "crawler.h"
#include <string>
#include <queue>
#include <chrono>
#include <fstream>
#include "crawler.h"

typedef std::chrono::high_resolution_clock Time;
using namespace std::chrono;

int main(int argc, char const *argv[])
{
    mpi::environment env(argc, argv);
    mpi::communicator world;

    organization og;
    og.numberOfProducts = 0;
    og.file.open ("sequentialTimes.txt");
    og.file2.open ("WaitTimeSequential.txt");
    og.sumOfProductsTime = 0;

    std::string allProdURL = argv[1]; //UrlProduto
    std::ofstream file1;

    file1.open ("averageTimePerProductSequential.txt");
    Time::time_point t1Total= Time::now();

    startCrawl(allProdURL,og);

    Time::time_point t2Total= Time::now();
    duration<double> TotalTime = duration_cast<duration<double>>(t2Total - t1Total);

    file1 <<"Average time per product: " <<"\n"<< TotalTime.count()/og.numberOfProducts << '\n';
    og.file2 << "WaitTime: "<<"\n"<<og.sumOfProductsTime<< "\n";
    og.file2.close();
    file1.close();

    return 0;
}