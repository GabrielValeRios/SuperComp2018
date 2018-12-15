#include <math.h>
#include <iostream>
#include <random>
#include <vector>
#include <chrono> 
#include <fstream>
using namespace std::chrono;

double inner_prod(double *a, double *b, unsigned long n) {
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += a[i] * b[i];
    }
    return sqrt(sum);
}

double sum_positive(double *a, unsigned long n) {
    double s = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] > 0) {
            s += a[i];
        }
    }
    return s;
}

void sqrt_element(double *a, unsigned long n) {
    for (int i = 0; i < n; i++) {
        a[i] = sqrt(a[i]);
    }
}

void exp_element(double *a, unsigned long n) {
    for (int i = 0; i < n; i++) {
        a[i] = exp(a[i]);
    }
}

void log_element(double *a, unsigned long n) {
    for (int i = 0; i < n; i++) {
        a[i] = log(a[i]);
    }
}

void gauss(double *a, double *b, unsigned long n) {
    double p = 1 / sqrt(2.0 * M_PI);
    for (int i = 0; i < n; i++) {
        b[i] = p * exp(-a[i]*a[i]/2);
    }
}

void intRandom(double *v, unsigned long n){
  // definir range de numeros randomicos
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(-10,10);
  // gerar numeros e guardar no array
  for(int i = 0; i < n; i++){
    v[i] = (distribution(generator));
  }
}

//highOrderFunction1
auto timeCalc_1(auto x(double*,unsigned long),unsigned long size,unsigned long max,unsigned long plus,std::ofstream &file){
    //medir o tempo criando 2 tempos (t1, t2)
    for(int i = size; i<= max; i+= plus){
        auto *a = new double[i];
        //randomização do array de ints
        intRandom(a, i);
        //medida do tempo
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        //function call do argumento
        x(a,i);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        file << '\n' << i << "," << time_span.count();
    }
}

//highOrderFunction2
auto timeCalc_2(auto x(double*,double*,unsigned long),unsigned long size,unsigned long max,unsigned long plus,std::ofstream &file){
    //medir o tempo criando 2 tempos (t1, t2)
    for(int i = size; i<= max; i+= plus){
        auto *a = new double[i];
        //randomização do array de ints
        intRandom(a, i);
        //medida do tempo
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        //function call do argumento
        x(a,a,i);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        file << '\n' << i << "," << time_span.count();
    }
}   

int main(int argc, char const *argv[])
{  
  if ((argv[1] <= 0 or argv[2] <= 0 or argv[3] <= 0) or (argv[1] == NULL or argv[2] == NULL or argv[3] == NULL)){
    printf("%s\n", "Inputs faltando ou irregulares. Apenas inteiros positivos maiores que '0'");
  }
  else{
    //output file
    std::ofstream file;
    file.open ("resultados.txt"); 
    // valor inicial do loop de testes
    unsigned long initialSize = atoi(argv[1]);
	unsigned long max = atoi(argv[2]);
	unsigned long plus = atoi(argv[3]);
    //func calls
    file << "***inner_prod***" << '\n';
    file << "Size" << "," << "Time";
    timeCalc_2(inner_prod,initialSize,max,plus,file);
    file << '\n';
    file << "***sum_positive***"<< '\n';
    file << "Size" << "," << "Time";
    timeCalc_1(sum_positive,initialSize,max,plus,file);
    file << '\n';
    file << "***sqrt_element***"<< '\n';
    file << "Size" << "," << "Time";
    timeCalc_1(sqrt_element,initialSize,max,plus,file);
    file << '\n';
    file << "***exp_element***"<< '\n';
    file << "Size" << "," << "Time";
    timeCalc_1(exp_element,initialSize,max,plus,file);
    file << '\n';
    file << "***log_element***"<< '\n';
    file << "Size" << "," << "Time";
    timeCalc_1(log_element,initialSize,max,plus,file);
    file << '\n';
    file << "***gauss***"<< '\n';
    file << "Size" << "," << "Time";
    timeCalc_2(gauss,initialSize,max,plus,file);

    file.close();
    }

  return 0;
}