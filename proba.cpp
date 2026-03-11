#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>

// Генератор как глобальный объект (или можно передавать по ссылке)
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> dist(0.0, 1.0);


template <typename T>

void print_vector(std::vector<T> arr){
    std::cout << "vector ";
    for(const auto x: arr){
        std::cout << x << "\t";
    }
    std::cout << "\n";
}


double gener(double lambda, double alpha){
    double b = M_E / (M_E + alpha);

    double x;

    bool flag = true;


    while(flag){
        // Генерируем число
        double r1 = dist(gen);
        double r2 = dist(gen);

        double v;

        if(r1 < b){
            v = pow((r1 / b), 1.0/alpha);
            if(r2 <= exp(-v)) {x = v / lambda; flag = false;}
        }
        else{
            v = 1 - std::log((1-r1)/(1-b));
            if(r2 <= pow(v, alpha-1)) {x = v/lambda; flag = false;}
        }

    }
    return x;
}


double calc_x(double lambda, double alpha){
    int m = static_cast<int>(alpha);
    double a = alpha - m;
    double y = gener(lambda, a);
    double product_of_r = 1;
    for(int i = 0; i < m; ++i){
        // Генерируем число
        double r1 = dist(gen);
        product_of_r *= r1;
    }

    double x = y - (1.0/lambda)*std::log(product_of_r);
    return x;
}


double calc_f(double lambda, double alpha, double x){
    return (pow(lambda, alpha) / (std::tgamma(alpha))) * pow(x, alpha-1) * exp(-1 * lambda * x);
}


std::vector<double> set_normal(std::vector<double>& raw_values){
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

//int scaled = static_cast<int>(el_min + new_x * (el_max - el_min));


std::vector<int> set_for_use(std::vector<double>& normal){
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


std::vector<int> set_degs(int n, double lambda, double alpha){
    std::vector<int> deg_values;
    std::vector<double> raw_values;
    std::vector<double> normal;
    bool flag = true;
    while(flag){
        raw_values.clear();
        deg_values.clear();
        normal.clear();
        for(int i = 0; i < n; ++i){
            raw_values.push_back(calc_x(lambda, alpha));
        }

//        print_vector(raw_values);

        normal = set_normal(raw_values);
        deg_values = set_for_use(normal);

//        print_vector(normal);
//        print_vector(deg_values);
        auto it = raw_values.begin();
        for(auto& x: deg_values){
            x = *it;
            ++it;
        }

        flag = false;
        int summ = 0;
        for(auto& x: deg_values){
            if(x == 0) x = 1;
            summ += x;
        }

        if(summ != 2*(n-1)) flag = true;

    }

    return deg_values;
}





int main(){

/*

    double lambda;
    double alpha;
//    std::cout << "Введите значение lambda и aplpha:\n";
//    std::cin >> lambda >> alpha;

    std::vector <double> lambda_vector = {1, 1.5, 3};
    std::vector <double> alpha_vector = {4, 0.5, 1.5, 1.6};

    std::vector<std::pair <double, double>> answers;

    std::vector<std::pair <double, double>> answers_for_f;

        for(auto a: alpha_vector){
            for(auto l: lambda_vector){
    //            std::cout << l << " and " << a << " and x = " << gener(l, a) << "\n";
                double sum = 0;
                int cnt = 100000;
                for(int j = 0; j < cnt; ++j){
                    sum += calc_x(l, a);
                }
                std::vector<double> ans = {sum / cnt, a/l};
                answers.push_back(std::make_pair(sum / cnt, a/l));
                double f_value = calc_f(l, a, sum/cnt);
                answers_for_f.push_back(std::make_pair(sum/cnt, f_value));
            }
        }

    for(auto el: answers){
        std::cout << el.first << " and " << el.second << "\n";
    }


    std::vector <double> x_vector;

    for(auto el: answers){
        x_vector.push_back(el.first);
    }
    double el_min = 100000;
    double el_max = -100000;
    for(auto el: x_vector){
        std::cout << el << "\n";
        if(el < el_min) el_min = el;
        if(el > el_max) el_max = el;
    }

    std::cout << el_min << " and " << el_max << "\n";


    for(auto el: answers_for_f){
        std::cout << "x = " << el.first << " and f = " << el.second << "\n";
    }

*/

    std::cout << "Введите число вершин:\n";

    int n;

    std::cin >> n;

    double lambda = 2;
    double alpha = 3.5;

    std::vector<int> deg_values;

    deg_values = set_degs(n, lambda, alpha);

    for(auto x: deg_values){
        std::cout << x << "\t";
    }
    std::cout << "\n";

/*
    std::vector<int> deg_values;

    std::vector<double> raw_deg_values;




    for(int i = 0; i < n; ++i){
        int x = static_cast<int>(calc_x(lambda, alpha));
        deg_values.push_back(x);
        raw_deg_values.push_back(calc_x(lambda, alpha));
    }

    std::vector<double> normal = set_normal(raw_deg_values);

    deg_values = set_for_use(normal);

    for(auto x: deg_values){
        std::cout << x << ", ";
    }
    std::cout << "\n";
    for(auto x: raw_deg_values){
        std::cout << x << ", ";
    }
    std::cout << "\n";

*/

}