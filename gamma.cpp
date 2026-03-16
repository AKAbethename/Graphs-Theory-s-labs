#include "gamma.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <utility>
#include <algorithm>
#include <iomanip> 

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> dist(0.0, 1.0);


template <typename T>
void print_vector(const std::vector<T>& vec){
    std::cout << "{";
    for(auto it = vec.begin(); it != vec.end(); ++it){
        std::cout << *it;
        if(it != --vec.end()) std::cout << ",   "; 
    }
    std::cout << "}\n";
}


Gamma::Gamma(){}

Gamma::Gamma(double lambda, double alpha, int n) : lambda(lambda), alpha(alpha), alpha0_1(alpha - static_cast<int>(alpha)), n(n) {
    raw_values.resize(n);
    values.resize(n);
    bool flag = true;
    while(flag){
        raw_values.clear();
        values.clear();
//        normal.clear();
        for(int i = 0; i < n; ++i){
            raw_values.push_back(calc_x());
        }
        values.resize(n);

        auto it = raw_values.begin();
        for(auto& x: values){
            x = *it;
            ++it;
        //    std::cout << x << "\n";
        }
        

        flag = false;
        int summ = 0;
        for(auto& x: values){
            if(x == 0) x = 1;
            summ += x;
        }

        if(summ != 2*(n-1)) flag = true;
    }
}

double Gamma::gener(){
    double b = std::exp(1.0) / (std::exp(1.0) + alpha0_1);
    double x;
    
    while(true){
        double r1 = dist(gen);
        double r2 = dist(gen);
        
        double v;
        
        if(r1 < b){
            v = std::pow(r1 / b, 1.0 / alpha0_1);
            if(r2 <= std::exp(-v)) {
                x = v / lambda;
                break;
            }
        }
        else{
            v = 1.0 - std::log((1.0 - r1) / (1.0 - b));
            if(r2 <= std::pow(v, alpha0_1 - 1.0)) {
                x = v / lambda;
                break;
            }
        }
    }
    return x;
}

double Gamma::calc_x(){
    int m = static_cast<int>(alpha);
    double a = alpha - m;
    double y = gener();
    double product_of_r = 1;
    for(int i = 0; i < m; ++i){
        // Генерируем число
        double r1 = dist(gen);
        product_of_r *= r1;
    }

    double x = y - (1.0/lambda)*std::log(product_of_r);
    return x;
}


void Gamma::show(){
    double static_lambda = lambda;
    double static_alpha = alpha;
    double static_aplha0_1 = alpha0_1;
    std::cout << "\n\n";
    std::cout << "======================================\n";
    std::cout << "|Math expectation: x = alpha / lambda| \n"; 
    std::cout << "======================================\n";


    std::vector<double> lambdas = {1, 1.5, 2.5, 3};
    std::vector<double> alphas = {0.4, 1.2, 1.3, 1.5}; // , 1.6, 1.8, 2.5, 2.6, 3.4};
    int k = 100000;

    std::cout << "<Count of iterations -- " << k << ">\n\n";
    
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\\ \t\tα\t\tMath. expectation\t Res \t\t\t Difference, %\n";
    std::cout << "-----------------------------------------------------------------------------------------------\n";

    for(auto l: lambdas){
        for(auto a: alphas){
            double sum = 0.0;
            lambda = l;
            alpha = a;
            alpha0_1 = alpha - static_cast<int>(alpha);
            for(int i = 0; i < k; ++i){
                sum += calc_x();
            }
            
            double actual_mean = sum / k;
            double theor_mean = alpha / lambda;
            
            double diff = (actual_mean - theor_mean) / theor_mean * 100.0;
            
            std::cout << l << "\t" << alpha << "\t" 
                      << theor_mean << "\t\t" 
                      << actual_mean << "\t\t" 
                      << diff << "%\n";
        }
    }


    lambda = static_lambda;
    alpha = static_alpha;
}


std::vector<double> Gamma::set_normal(){
    double el_min = 1000000;
    double el_max = -1000000;
    for(auto x: raw_values){
        if(x < el_min) el_min = x;
        if(x > el_max) el_max = x;
    }
    std::vector<double> normal;
    for(auto x: raw_values){
        double new_x = (x - el_min) / (el_max - el_min);
        // Масштабируем к [min_degree, max_degree]
        normal.push_back(new_x);
    }
    return normal;
}
#if 0
std::vector<int> Gamma::set_for_use(){
    std::vector<int> ans;
    double el_min = *std::min_element(normal.begin(), normal.end());
    double el_max = *std::max_element(normal.begin(), normal.end());
    int deg = 3;
    for(auto x: normal){
//        int scaled = static_cast<int>(el_min + x * (el_max - el_min));
        ans.push_back(static_cast<int>(x * deg));
    }
    return ans;
}
#endif

#if 0

std::vector<int> Gamma::set_raw_and_(){
//    std::vector<int> deg_values;
//    std::vector<double> raw_values;
//    std::vector<double> normal;
    bool flag = true;
    while(flag){
        raw_values.clear();
        values.clear();
//        normal.clear();
        for(int i = 0; i < n; ++i){
            raw_values.push_back(calc_x());
        }


        auto it = raw_values.begin();
        for(auto& x: values){
            x = *it;
            ++it;
        }

        flag = false;
        int summ = 0;
        for(auto& x: values){
            if(x == 0) x = 1;
            summ += x;
        }

        if(summ != 2*(n-1)) flag = true;

    }

    return values;
}

#endif

#if 0
std::vector<double> lambda_vector = {1, 1.5, 2.5, 3};
    std::vector<double> alpha_vector = {0.4, 1.2, 1.3, 1.5, 1.6, 1.8, 2.5, 2.6, 3.4};
    
    int numbers_per_pair = 100000;  // сколько чисел для каждой пары
    
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "λ\t\tα\t\tТеор. E[X]\t\tФакт. E[X]\t\tОтклонение %\n";
    std::cout << "------------------------------------------------\n";
    
    for(auto l: lambda_vector){
        for(auto alpha: alpha_vector){
            double sum = 0.0;
            
            for(int i = 0; i < numbers_per_pair; ++i){
                sum += calc_x(l, alpha);
            }
            
            double actual_mean = sum / numbers_per_pair;
            double theor_mean = alpha / l;
            
            double diff = (actual_mean - theor_mean) / theor_mean * 100.0;
            
            std::cout << l << "\t" << alpha << "\t" 
                      << theor_mean << "\t\t" 
                      << actual_mean << "\t\t" 
                      << diff << "%\n";
        }
    }    


    #endif


void Gamma::print_raw_values(){
    print_vector(raw_values);
}

void Gamma::print_values(){
    print_vector(values);
}

std::vector<int> Gamma::get_values(){return values;}
