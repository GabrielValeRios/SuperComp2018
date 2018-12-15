#include <boost/serialization/string.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/operations.hpp>
#include <iostream>

#include <cpr/cpr.h>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <fstream>
#include <algorithm>
#include <boost/regex.hpp>
#include "crawler.h"
#include "produto.h"

namespace mpi = boost::mpi;
typedef std::chrono::high_resolution_clock Time;
using namespace std::chrono;

// ----------side functions----------------------

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

std::string baseURLDiscover(std::string &firstPageURL){

    std::string correctFullURL = firstPageURL.substr(0, firstPageURL.size()-1);
    std::string baseURL = correctFullURL + "?page="; 
    return baseURL;
}
// funcao cedida por Gustavo Efeiche (https://github.com/gustavoefeiche)
std::vector<std::vector<std::string>> split_vector(
    const std::vector<std::string>& vec, size_t n) {
    std::vector<std::vector<std::string>> splitted;

    int length = vec.size() / n;
    int remain = vec.size() % n;

    int begin = 0;
    int end = 0;

    for (unsigned int i = 0; i < std::min(n, vec.size()); ++i) {
        end += (remain > 0) ? (length + !!(remain--)) : length;

        splitted.push_back(
            std::vector<std::string>(vec.begin() + begin, vec.begin() + end)
        );

        begin = end;
    }

    return splitted;
}
//--------------end of side functions block--------------------------

// nodes
std::string crawlAllPages(int number,std::string firstPageURL){

    std::string tempURL = baseURLDiscover(firstPageURL);
    std::string fullURL = tempURL.insert(tempURL.length(),std::to_string(number));

    return fullURL;       
}
//nodes
std::vector<std::string> crawlAllProducts(std::vector<std::string> &urls, double &waitSum, double &totalTime) {

    Time::time_point t1TotalTime = Time::now();
    std::vector<std::string> jsons;
    //std::cout << "URLS_SIZE: "<<urls.size()<<"\n";
    for(unsigned i=0;i<urls.size();i++){
        Time::time_point t1ProdutoWait = Time::now();
        auto r = cpr::Get(cpr::Url{urls[i]});
        Time::time_point t2ProdutoWait = Time::now();
        duration<double> produtosWaitTime = duration_cast<duration<double>>(t2ProdutoWait - t1ProdutoWait);
        //throw result to cerr output
        waitSum += produtosWaitTime.count();

        //std::cout<<"STATUS: "<<r.status_code<<" "<<"URL: "<<urls[i]<<"\n";
        //productPage text
        std::string s = r.text;
        //urlPagina
        boost::regex regex ("name=\"linkToProduct\" href=\"(.*?)\">");
        boost::smatch sm;
        while (boost::regex_search (s,sm,regex)) {
            //produz jsons
            std::string match = sm[1];
            if (match[0] == 'h'){
                jsons.push_back(crawlSingleProduct(match));
                //std::cout << "JSON_SIZE: "<<jsons.size()<<"\n";
            }
            s = sm.suffix().str();
        } 
    }
    Time::time_point t2TotalTime = Time::now();
    //totalTime of Process
    duration<double> tempo = duration_cast<duration<double>>(t2TotalTime - t1TotalTime);
    totalTime = tempo.count();

    return jsons;
}
//nodes 
std::string crawlSingleProduct(std::string fullURL) {

    Time::time_point t1Produto = Time::now();
    auto r = cpr::Get(cpr::Url{fullURL});
    std::string s = r.text; //productPage text
    //std::cout<<"STATUS: "<<r.status_code<<" "<<"URL: "<<fullURL<<"\n";

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

    //time per product
    Time::time_point t2Produto = Time::now();
    duration<double> produtoTime = duration_cast<duration<double>>(t2Produto - t1Produto);
    std::cerr <<"Time of Product: " <<nome <<"\n"<< produtoTime.count() << '\n';

    //produto
    Produto p(nome,descricao,fotoURL,paginaURL,categoria,precoVista,precoParcelado);
    return p.toJson();
}

// master node and nodes
int main(int argc, char *argv[]) {
    mpi::environment env(argc, argv);
    mpi::communicator world;

    std::string productsURL = argv[1]; //UrlProduto

    // scatter array
    std::vector<std::string> urlsDiv;
    //matrix array
    std::vector<std::vector<std::string>> m;
    
    //scatter division.
    if(world.rank() == 0){
        // vector of urls
        std::vector<std::string> urls;

        // number of pages
        int Npages = numberPages(productsURL); 
        std::cout<<"NPAGES: "<<Npages<<"\n";  
        
        for (int i = 1; i <= Npages; i++){
            urls.push_back(crawlAllPages(i,productsURL));   
        }   

        //create matrix
        double divisions = world.size();
        m = split_vector(urls,divisions);
    }

    //waitTime vars
    double waitSumZero = 0;
    double waitSumOutput;
    //totalTime var
    double totalTime;
    //scatter of jsons
    mpi::scatter(world,m,urlsDiv,0);
    //vector of jsons
    std::vector<std::string> jsons = crawlAllProducts(urlsDiv,waitSumZero,totalTime);
    //reduce of WaitTime
    mpi::reduce(world, waitSumZero,waitSumOutput,std::plus<double>(), 0);

    //gather results
    if (world.rank() == 0) {
        //output gather data structure
        std::vector<std::vector<std::string>> results;
        //total time 
        std::vector<double> totalTimeGather;
        //gather of jsons
        mpi::gather(world, jsons, results, 0);
        //gather of totalTimes
        mpi::gather(world, totalTime, totalTimeGather, 0);   
        //time of all products
    
        double max_value = *std::max_element( totalTimeGather.begin(), totalTimeGather.end() ) ;
        std::cerr<<'\n'<<"TOTAL TIME: " <<"\n"<< max_value << '\n';

        // WaitReducedTime
        std::cerr <<'\n'<< "WAIT TIME: "<<waitSumOutput<<"\n";

        // print al jsons
        for ( const std::vector<std::string> &v : results )
        {
            for ( std::string x : v ) std::cout << x << ' '<<"\n";
            std::cout << std::endl;
        }
        
    } else {
        mpi::gather(world, jsons, 0);
        mpi::gather(world,totalTime,0);
    }
    
    return 0;
}