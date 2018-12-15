#include <cpr/cpr.h>
#include <iostream>
#include <string>
#include <boost/regex.hpp>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <random>
#include <fstream>
#include <algorithm>
#include "produce_consumer.h"
#include "produto.h"
#include "assert.h"

typedef std::chrono::high_resolution_clock Time;
using namespace std::chrono;

std::string getTagValue(std::string s,std::string regex){
    boost::smatch m; //match
    std::string ans;
    boost::regex self_regex(regex,
                        boost::regex_constants::ECMAScript | boost::regex_constants::icase);
    if (boost::regex_search(s,m,self_regex)) {
        ans =  m[1];
    } 
    return ans;
}

void prodConsumer::crawlSingleProduct(semaphore &VAZIOS,semaphore &COUNT) {

    while(true){

        if(stopConsume == 0 && COUNT.n == 0){
            COUNT.release();
            to_produce+=1;
            break;
        }

        COUNT.acquire();
        auto url = getFromBuffer(); 
        VAZIOS.release();

        std::string url_temp = url;

        Time::time_point t1Produto = Time::now();
        auto r = cpr::Get(cpr::Url{url});

        //used for final time per product  
        nProducts+=1;
        //std::cout << "url:  " <<url<<"\n";
        std::string s = r.text; //productPage text
        //nome
        std::string n = "<h1 class=\"header-product__title\" itemprop=\"name\">(.*?)</h1>";
        std::string nome = getTagValue(s,n);
        //descricao
        std::string d = "<p class=\"description__text\"></p>(.*?)<p class=\"description__text\"></p>";
        std::string descricao = getTagValue(s,d);
        //urlFoto
        std::string urlFoto = "<img class=\"showcase-product__big-img js-showcase-big-img\" src=(.*?) itemprop";
        std::string fotoURL = getTagValue(s,urlFoto);
        //urlPagina
        std::string paginaURL = url_temp;
        //categoria do produto
        std::string cat = "<a class=\"breadcrumb__item\" href=\"https://www.magazineluiza.com.br/celulares-e-smartphones/l/te/\" itemprop=\"item\"> (.*?) </a>";
        std::string categoria = getTagValue(s,cat);
        //preco a vista
        std::string vistaPreco = "<span class=\"price-template__text\">(.*?)</span>";
        std::string precoVista = getTagValue(s,vistaPreco);
        //preco Parcelado (consertar)
        std::string parceladoPreco = "parceladoPreco";
        std::string precoParcelado = getTagValue(s,parceladoPreco);

        //tempo por produto
        Time::time_point t2Produto = Time::now();
        duration<double> produtoTime = duration_cast<duration<double>>(t2Produto - t1Produto);
        file <<"Time of Product: " <<nome <<"\n"<< produtoTime.count() << '\n';
        sumOfProductsTime += produtoTime.count();
        //produto
        Produto p = Produto(nome,descricao,fotoURL,paginaURL,categoria,precoVista,precoParcelado);
        std::cout << p.toJson() << "\n";   
    }
}

void prodConsumer::crawlAllProducts(std::string productsURL,semaphore &VAZIOS,semaphore &COUNT) {
    
    Time::time_point t1Total= Time::now();
    auto r = cpr::Get(cpr::Url{productsURL});
    Time::time_point t2Total= Time::now();

    std::string s = r.text; //productPage text
    //urlPagina
    boost::regex regex ("name=\"linkToProduct\" href=\"(.*?)\">");
    boost::smatch sm;
    while (boost::regex_search (s,sm,regex)) {
        //produz

        std::string match = sm[1];
        if (match[0] == 'h'){

            duration<double> TotalTime = duration_cast<duration<double>>(t2Total - t1Total);
            waitTime+=TotalTime.count();
            VAZIOS.acquire();           
            addToBuffer(match);
            COUNT.release();

        }
        s = sm.suffix().str();
    }  
}

void prodConsumer::crawlAllPages(std::string baseURL,int number,std::string firstPageURL,semaphore &VAZIOS,semaphore &COUNT){

    std::string tempURL = baseURL;
    std::string fullURL = tempURL.insert(baseURL.length(),std::to_string(number));
    std::cout << "fullURL: " <<fullURL<< "\n";

    crawlAllProducts(fullURL,VAZIOS,COUNT);       
}

semaphore::semaphore(int _n) :
    n(_n){};
            
void semaphore::acquire(){

    {   
        std::unique_lock<std::mutex> lk(m);
        if (n <= 0){
            cv.wait(lk,[&]{return n>0;});
 
        }
        n-=1;
    }
}

void semaphore::release(){

    {
        std::unique_lock<std::mutex> lk(m);
        n += 1;
    } 
    cv.notify_one();
}

prodConsumer::prodConsumer(int _size) :
    bufferSize(_size)
    {
        buffer = std::vector<std::string> (bufferSize);
    };

void prodConsumer::producer(std::string baseURL,int inicio,int fim,std::string allProdURL,semaphore &VAZIOS,semaphore &COUNT) {
    stopConsume+=1;
    for (int i = inicio; i <= fim; i++){
        crawlAllPages(baseURL,i,allProdURL,VAZIOS,COUNT);
    }
    stopConsume-=1;
}

void prodConsumer::consumer(semaphore &VAZIOS,semaphore &COUNT) {

    crawlSingleProduct(VAZIOS,COUNT);
}

int prodConsumer::numberPages(std::string &firstPageURL){
    std::vector<std::string> nPages;

    auto r = cpr::Get(cpr::Url{firstPageURL});
    std::string s = r.text; //productPage text
    //urlPagina
    boost::regex regex ("<a role=\"button\" tabindex=\"0\" aria-label=\".*?\">(.*?)<");
    boost::smatch sm;

    //matchs
    while (boost::regex_search (s,sm,regex)) {
        nPages.push_back(sm[1]);
        s = sm.suffix().str();
    }

    int lastNumber = stoi(nPages.back());

    return lastNumber;
}

void prodConsumer::addToBuffer(std::string url){

    {
        std::unique_lock<std::mutex> lk(bufferMutex);
        buffer[to_produce] = url;
        to_produce = (to_produce+1)%bufferSize;
        assert(to_produce!=to_consume);
    }
}

std::string prodConsumer::getFromBuffer(){

    {
        std::unique_lock<std::mutex> lk(bufferMutex);
        assert(to_produce!=to_consume);
        std::string url = buffer.at(to_consume);
        //buffer.erase(buffer.begin()+to_consume);
        to_consume = (to_consume+1)%bufferSize;
        return url;
    }
}

std::string prodConsumer::baseURLDiscover(std::string &firstPageURL){

    std::string correctFullURL = firstPageURL.substr(0, firstPageURL.size()-1);
    std::string baseURL = correctFullURL + "?page=";
    return baseURL;
}

void prodConsumer::callThreads(std::string allProdURL,int num_threads) {

    //time files
    file.open ("parallelTimes.txt");
    file3.open ("WaitTimeParallel.txt");
    //semaphore object
    semaphore VAZIOS(bufferSize-1); // size of semaphore
    semaphore COUNT(0);

    // number of product pages
    int Npages = numberPages(allProdURL);
    std::cout <<"Npages:  " <<Npages << "\n";
    //define baseURL fo access
    std::string baseURL = baseURLDiscover(allProdURL);

    //divide num_threads into consumer's and producer's
    std::vector<std::thread> threads;
    int consumer_threads = num_threads/2;
    int producer_threads = num_threads-consumer_threads;
    int inicio = 1;
    int divisor = Npages/producer_threads; 
    int fim;
    
    for(int i=1;i<=producer_threads;i++){
        if (i == producer_threads)
            fim = Npages;
        else
            fim = i*divisor;
        std::cout << "INICIO: "<<inicio<<" "<<"FIM: "<<fim<<"\n";
        threads.emplace_back(std::thread(&prodConsumer::producer,this,baseURL,inicio,fim,allProdURL,std::ref(VAZIOS),std::ref(COUNT)));
        inicio = fim+1;
    }
    for(int i=1;i<=consumer_threads;i++){
        threads.emplace_back(std::thread(&prodConsumer::consumer,this,std::ref(VAZIOS),std::ref(COUNT)));
    }
    for (std::thread & t : threads) {
        t.join();
    }

    file3<<"WaitTime"<<"\n"<<waitTime<<"\n";
    file3.close();
}