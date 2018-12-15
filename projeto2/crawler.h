#ifndef CRAWLER_H
#define CRAWLER_H
#include <string>
#include <queue>
#include <fstream>

struct organization {
  std::ofstream file;
  std::ofstream file2;
  int numberOfProducts;
  double sumOfProductsTime;
}; 

std::string getTagValue(std::string s,std::string regex);
void crawlSingleProduct(std::string fullURL,organization &og);
void crawlAllProducts(std::string productsURL,organization &og);
void crawlAllPages(int number,std::string firstPageURL,organization &og);
void startCrawl(std::string productsURL,organization &og);
int numberPages(std::string &firstPageURL);
void producer(int &fim,std::string allProdURL,organization &og);
std::string baseURLDiscover(std::string &firstPageURL);

#endif