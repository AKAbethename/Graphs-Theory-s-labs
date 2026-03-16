#ifndef GAMMA_HPP
#define GAMMA_HPP

#include <vector>
#include <random>
#include <iostream>


extern std::random_device rd;
extern std::mt19937 gen;
extern std::uniform_real_distribution<double> dist;

class Gamma{
  private:
    double lambda;
    double alpha;
    double alpha0_1;
    int n;
    std::vector<int> values;
    std::vector<double> raw_values;


  public:
    Gamma();
    Gamma(double l, double a, int n);
    void show();
    double gener();
    double calc_x();
    double calc_f();
    std::vector<double> set_normal();
    std::vector<double> set_for_use();
    std::vector<int> set_degs();


    void print_raw_values();
    void print_values();

    std::vector<int> get_values();


};



#endif
