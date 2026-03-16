#ifndef ACTS_HPP
#define ACTS_HPP

#include <iostream>
#include <vector>


void act1();

void act2();

void SKNF(const std::vector<int>& table);

void SDNF(const std::vector<int>& table);

void get_table();

void print_values_from_bdd();


class ZhegalkinBuilder {

private:
    std::vector<int> coefficients;
    
public:
    ZhegalkinBuilder();
    
    int evaluate(int x, int y, int z, int t) const;
    
    std::string getPolynomial() const;
};


void print_Zhegalkin();

void print_val_Zhegalkin();

#endif