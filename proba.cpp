#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>
#include <bitset>
#include <cstdlib>
#include <format>

// Генератор как глобальный объект (или можно передавать по ссылке)
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> dist(0.0, 1.0);


int randomBetween(int m, int n) {
    std::uniform_int_distribution<> dist(m, n);
    return dist(gen);
}


template <typename T>

void print_vector(std::vector<T> arr){
    std::cout << "vector ";
    for(const auto x: arr){
        std::cout << x << "\t";
    }
    std::cout << "\n";
}


template <typename T>

void print_vector_of_vectors(std::vector<std::vector<T>> arr){
    for(const auto x: arr){
        print_vector(x);
    }
    std::cout << "\n";
}


void print_matrix(std::vector<std::string> matrix){
    std::cout << "{\n";
    for(auto it = matrix.begin(); it != matrix.end(); ++it){
        std::cout << *it << "\n";
    }
    std::cout << "}\n";
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


std::vector<double> set_normal(const std::vector<double>& raw_values){
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


std::vector<int> set_for_use(const std::vector<double>& normal){
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



bool is_k_1(std::string str, int k){
    auto it = str.begin();
    int cnt = 0;
    for(auto it = str.begin(); it != str.end(); ++it){
        if(*it == '1') ++cnt;
    }
    if(cnt == k) return true;
    return false;
}

std::string get_bin(int num, int length) {
    std::string result;
    
    // Сначала получаем двоичное представление без ведущих нулей
    for (int i = length - 1; i >= 0; i--) {
        result += ((num >> i) & 1) ? '1' : '0';
    }
    
    return result;
}

bool check_one(std::vector<std::string> matrix, int n){
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(matrix[i][j] != matrix[j][i]) return false;
        }
    }
    return true;
}

bool check_two(std::vector<std::string> matrix, int n){
    for(int i = 0; i < n; ++i){
        if(matrix[i][i] == '1') return false;
    }
    return true;
}

std::vector<std::string> get_minor(const std::vector<std::string>& matrix, int size){
    std::vector<std::string> minor;
    for(int i = 0; i < size; ++i){
        std::string str = matrix[i].substr(0, size);
        minor.push_back(str);
    }
    return minor;
}


std::vector<std::string> create_maxtix(const int& n, const std::vector<int>& degs, const std::vector<std::vector<std::string>>& baza){  // n - size of square matrix
    std::vector<std::string> matrix;
    bool flag = true;
    int k = 1;
    while(flag){
        matrix.clear();
        for(int i = 0; i < n; ++i){
            int cur_deg = degs[i];
            int rand = randomBetween(0, baza[cur_deg-1].size()-1);
            matrix.push_back(baza[cur_deg-1][rand]);
        }
//        std::cout << "ex number " << k << "\n";
//        print_matrix(matrix);
        std::vector<std::string> main_minor = get_minor(matrix, n/2);
        if(check_two(matrix, n) &&  check_one(main_minor, n/2) &&  check_one(matrix, n)) flag = false;
        ++k;
    }
    std::cout << k << "\n";
    return matrix;
}






int main(){


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

    if(is_k_1("10011", 3)) std::cout << "yeeeah\n";

    std::vector<std::string> vector;  // has 2^(n) strings [n = 3: 000 001 010 011 100 101 110 111]

    std::vector<std::vector<std::string>> baza;
    baza.resize(n);



    for(int i = 1; i <= static_cast<int>(pow(2, n)); ++i){
        std::string binary = get_bin(i, n);
        for(int j = 1; j <= n; ++j){
            if(is_k_1(binary, j)) baza[j-1].push_back(binary);
        }
    }
    std::cout << "\n\n";
    print_vector_of_vectors(baza);
    std::cout << "\n";

//    std::cout << randomBetween(2, 10);

    std::vector<std::string> matrix = create_maxtix(n, deg_values, baza);
    print_matrix(matrix);




}


// baza -- (vec_1, vec_2, ..., vec_n), где vec_i -- вектор, содержащий строки с i количеством единиц
// is_k_1(st) -- проверка, что в строке содержится k единиц