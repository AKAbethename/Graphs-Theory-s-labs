#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <iomanip>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> dist(0.0, 1.0);

double gener(double lambda, double alpha){
    double b = std::exp(1.0) / (std::exp(1.0) + alpha);
    double x;
    
    while(true){
        double r1 = dist(gen);
        double r2 = dist(gen);
        
        double v;
        
        if(r1 < b){
            v = std::pow(r1 / b, 1.0 / alpha);
            if(r2 <= std::exp(-v)) {
                x = v / lambda;
                break;
            }
        }
        else{
            v = 1.0 - std::log((1.0 - r1) / (1.0 - b));
            if(r2 <= std::pow(v, alpha - 1.0)) {
                x = v / lambda;
                break;
            }
        }
    }
    return x;
}

double calc_x(double lambda, double alpha){
    int m = static_cast<int>(alpha);
    double a = alpha - m;
    
    double y = gener(lambda, a);  // здесь a - дробная часть
    
    double product = 1.0;
    for(int i = 0; i < m; ++i){
        product *= dist(gen);
    }
    
    double x = y - (1.0 / lambda) * std::log(product);
    return x;
}

int main(){
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
    
    return 0;
}
