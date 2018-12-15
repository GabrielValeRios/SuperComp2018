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
std::string crawlSingleProduct(std::string fullURL);
std::vector<std::string> crawlAllProducts(std::vector<std::string> &urls, double &waitSum, double &totalTimeZero);
std::string crawlAllPages(int number,std::string firstPageURL);
std::string startCrawl(std::string productsURL);
std::vector<std::vector<std::string>> split_vector(const std::vector<std::string>& vec, size_t n);
int numberPages(std::string &firstPageURL);

std::string baseURLDiscover(std::string &firstPageURL);

#endif