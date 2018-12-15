#include <cpr/cpr.h>
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <boost/regex.hpp>
#include "crawler.h"
#include "produto.h"

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

int numberPages(std::string &firstPageURL){
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

void producer(int &fim,std::string allProdURL,organization &og) {

    // put it into the queue
    for (int i = 1; i <= fim; i++){
        crawlAllPages(i,allProdURL,og);
    }
}

std::string baseURLDiscover(std::string &firstPageURL){

    std::string correctFullURL = firstPageURL.substr(0, firstPageURL.size()-1);
    std::string baseURL = correctFullURL + "?page="; 
    return baseURL;
}

void crawlAllPages(int number,std::string firstPageURL,organization &og){

    std::string tempURL = baseURLDiscover(firstPageURL);
    std::string fullURL = tempURL.insert(tempURL.length(),std::to_string(number));
    std::cout << "fullURL: " <<fullURL<< "\n";

    crawlAllProducts(fullURL,og);       
}

void crawlAllProducts(std::string productsURL,organization &og) {
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
            og.sumOfProductsTime+=TotalTime.count();
            crawlSingleProduct(match,og);
        }
        s = sm.suffix().str();
    }  
}

typedef std::chrono::high_resolution_clock Time;
using namespace std::chrono;

void crawlSingleProduct(std::string fullURL,organization &og) {

    Time::time_point t1Produto = Time::now();
    auto r = cpr::Get(cpr::Url{fullURL});
    std::string s = r.text; //productPage text

    og.numberOfProducts+=1;
    
    //nome
    std::string n = "<h1 class=\"header-product__title\" itemprop=\"name\">(.*?)</h1>";
    std::string nome = getTagValue(s,n);
    //acesso a pagina de descricao
    std::string descricaoAnchor = "<a href=(.*?) class=\"header-product__text-interation js-floater-menu-link\">Ver descrição completa</a>";
    std::string descricaoLink = "https://www.magazineluiza.com.br/smartphone-samsung-galaxy-s9-128gb-preto-4g-cam-12mp-selfie-8mp-tela-5-8-quad-hd-octa-core/p/220282800/te/gas9/"
                + descricaoAnchor; 
    auto descricaoPage = cpr::Get(cpr::Url{descricaoLink});
    //descricao
    std::string d = "<p class=\"description__text\"></p>(.*?)<p class=\"description__text\"></p>";
    std::string descricao = getTagValue(s,d);
    //urlFoto (consertar)
    std::string urlFoto = "urlFoto";
    std::string fotoURL = getTagValue(s,urlFoto);
    //urlPagina
    std::string paginaURL = fullURL;
    //categoria do produto
    std::string cat = "<a class=\"breadcrumb__item\" href=\"https://www.magazineluiza.com.br/celulares-e-smartphones/l/te/\" itemprop=\"item\"> (.*?) </a>";
    std::string categoria = getTagValue(s,cat);
    //preco a vista
    std::string vistaPreco = "<span class=\"price-template__text\">(.*?)</span>";
    std::string precoVista = getTagValue(s,vistaPreco);
    //preco Parcelado (consertar)
    std::string parceladoPreco = "parceladoPreco";
    std::string precoParcelado = getTagValue(s,parceladoPreco);

    Time::time_point t2Produto = Time::now();
    duration<double> produtoTime = duration_cast<duration<double>>(t2Produto - t1Produto);
    og.file <<"Time of Product: " <<nome <<"\n"<< produtoTime.count() << "\n";
    //produto
    Produto p(nome,descricao,fotoURL,paginaURL,categoria,precoVista,precoParcelado);
    std::cout << p.toJson() << "\n";
}

void startCrawl(std::string productsURL,organization &og) {
    og.file.open ("sequentialTimes.txt");
    og.file2.open ("WaitTimeSequential.txt");
    og.sumOfProductsTime = 0;
    int Npages = numberPages(productsURL);
    //start crawl
    producer(Npages,productsURL,og);

    og.file2 << "WaitTime: "<<"\n"<<og.sumOfProductsTime<< "\n";
    og.file2.close();
}