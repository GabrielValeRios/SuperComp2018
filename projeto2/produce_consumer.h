#ifndef PRODUCE_CONSUMER_H
#define PRODUCE_CONSUMER_H

#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>
#include <fstream>

class semaphore {
public:
	int n;
	semaphore(int _n);

    void acquire();
    void release();

private:
    std::mutex m;
    std::condition_variable cv;
};

class prodConsumer {
private:
    std::mutex bufferMutex;
    int stopConsume = 0;

public:
    std::ofstream file;
    std::ofstream file3;
    int to_produce = 0;
    int to_consume = 0;
    double nProducts;
    double sumOfProductsTime=0;
    double waitTime = 0;

	unsigned int bufferSize;
    std::vector<std::string> buffer;
    void addToBuffer(std::string);
    std::string getFromBuffer();

	prodConsumer(int _size);
    
    void producer(std::string baseURL,int inicio,int fim,std::string allProdURL,semaphore &s1,semaphore &s2);
    void consumer(semaphore &s1,semaphore &s2);

    int numberPages(std::string &firstPageURL);
    std::string baseURLDiscover(std::string &firstPageURL);

    void callThreads(std::string allProdURL,int num_threads);
    void crawlAllProducts(std::string productsURL,semaphore &VAZIOS,semaphore &COUNT);
    void crawlAllPages(std::string baseURL,int number,std::string firstPageURL,semaphore &VAZIOS,semaphore &COUNT);
    void crawlSingleProduct(semaphore &VAZIOS,semaphore &COUNT);
};


std::string getTagValue(std::string s,std::string regex);
#endif 