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
    std::cout << "vector \n";
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

std::vector<std::string> get_right_vector(const std::vector<std::string>& vector_with_wrong, int i){  // i -- row index
    std::vector<std::string> right;
    for(auto x: vector_with_wrong){
        if(x[i] != '1') right.push_back(x);
    }
    return right;
}

bool two_v(const std::vector<std::string>& matrix, int n, int i, int j, std::vector<bool>& visited) {
    if(i == j) return true;
    visited[i] = true;
    
    for(int k = 0; k < n; ++k) {
        if(!visited[k] && matrix[i][k] == '1') {
            if(two_v(matrix, n, k, j, visited)) return true;
        }
    }
    return false;
}

bool connectivity(const std::vector<std::string>& matrix, int n) {
    int i = 0;
    for(int j = 1; j < n; ++j) {
        std::vector<bool> visited(n, false);
        if(!two_v(matrix, n, i, j, visited)) {
            return false;
        }
    }
    return true;
}


std::vector<std::string> create_maxtix(const int& n, const std::vector<int>& degs, const std::vector<std::vector<std::string>>& baza){  // n - size of square matrix
    std::vector<std::string> matrix;
    bool flag = true;
    int k = 1;
    while(flag){
        matrix.clear();
        bool flag1 = false;
        for(int i = 0; i < n; ++i){    
            int cur_deg = degs[i];
            std::vector<std::string> right_vector = get_right_vector(baza[cur_deg], i);
            int rand = randomBetween(0, right_vector.size()-1);

            matrix.push_back(right_vector[rand]);

        }

        if(flag1) continue;

        if(/* check_two(matrix, n) &&   check_one(main_minor, n/2) && */  check_one(matrix, n) && connectivity(matrix, n)) flag = false;
        ++k;
    }
    std::cout << k << "\n";
    return matrix;
}


void create_baza(std::vector<std::vector<std::string>>& baza, int n){
    baza.clear();
    baza.resize(n+1);
    for(int i = 0; i < static_cast<int>(pow(2, n)); ++i){
        std::string binary = get_bin(i, n);
        for(int j = 0; j <= n; ++j){
            if(is_k_1(binary, j)) baza[j].push_back(binary);
        }
    }
}


/*

bool two_v(std::vector<std::string> matrix, int n, int i, int j, std::vector<bool>& visited){  // i - cur v, j -- final v
    if(matrix[i][j] == '1') return true;
    visited[i] = true;
    for(int k = 0; k < n; ++k){
        if(i != k && matrix[i][k] == '1'){
            if(two_v(matrix, n, k, j, visited)) return true;
        }
    }
    return false;
}


bool connectivity(std::vector<std::string> matrix, int n, std::vector<int>& degs){
    int i = 0;
    bool flag = true;
    std::vector<bool> visited(n, false);
    for(int j = 1; j < n; ++j){
        if(!two_v(matrix, n, i, j, visited)) {flag = false; break;}
    }
    return flag;
}

*/


std::vector<int> sigma_deg_vals(const std::vector<int>& deg_vals, int n, int& i, int& j){  // i and j are poses for P
    i = 0;
    j = 0;
    std::vector<int> new_deg_vals;
    if(deg_vals[0] > 1 && deg_vals[1] == 1){
        new_deg_vals.push_back(deg_vals[1]);
        new_deg_vals.push_back(deg_vals[0]);
        for(int k = 2; k < n; ++k){
            new_deg_vals.push_back(deg_vals[k]);
        }
        i = 0;
        j = 1;
    }

    else if(deg_vals[0] > 1 && deg_vals[1] > 1){
        new_deg_vals = deg_vals;
        for(int k = 2; k < n; ++k){
            if(deg_vals[k] == 1) {std::swap(new_deg_vals[0], new_deg_vals[k]); i = 0; j = k; break;}
        }
    }


    else if(deg_vals[0] == 1 && deg_vals[1] == 1){
        new_deg_vals = deg_vals;
        for(int k = 2; k < n; ++k){
            if(deg_vals[k] > 1) {std::swap(new_deg_vals[1], new_deg_vals[k]); i = 1; j = k; break;}
        }
    }

    else if(deg_vals[0] == 1 && deg_vals[1] > 1){
        new_deg_vals = deg_vals;
    }
    ++i;
    ++j;
    if(new_deg_vals == deg_vals){i = -1; j = -1;}
    return new_deg_vals;
}

std::string get_new_v(int n){
    std::string ans;
    ans.resize(n);
    ans = "01" + (std::string(n-2, '0'));
    return ans;
}


std::vector<std::string> matrix_with_new_v(std::vector<std::string> matrix, int n, std::string str){  // n -- size of small matrix
    std::vector<std::string> new_matrix;
//    new_matrix.resize(n+1);
    new_matrix.push_back(str);
    for(int i = 0; i < n; ++i){
        std::string new_str = str[i+1] + matrix[i];
        new_matrix.push_back(new_str);
    }
//    std::cout << "it is check\n";
//    print_matrix(new_matrix);
    return new_matrix;
}


void P_of_rows(std::vector<std::string>& matrix, int n, int i, int j){  // (i) <-> (j) 
    if(i == -1 && j == -1) return;
    std::string tmp = matrix[i-1];
    matrix[i-1].clear();
    matrix[i-1] = matrix[j-1];
    matrix[j-1].clear();
    matrix[j-1] = tmp;
}

void P_of_cols(std::vector<std::string>& matrix, int n, int i, int j){  // [i] <-> [j]
    if(i == -1 && j == -1) return;
    for(int k = 0; k < n; ++k){
        std::swap(matrix[k][i-1], matrix[k][j-1]);
    }
    
}

std::vector<int> get_new_deg_vals(const std::vector<int>& deg_vals, int n){  // deg_vals is kanon: {1, k, ...}, k > 1
    std::vector<int> new_deg_vals;
    new_deg_vals.push_back(deg_vals[1]-1);
    for(int i = 2; i < n; ++i) new_deg_vals.push_back(deg_vals[i]);
    return new_deg_vals;
}


std::vector<std::string> create_matrix_7x7(int n, std::vector<int> deg_vals,
                                         std::vector<std::vector<std::string>> baza){

    int i = 0;  // pos1
    int j = 0;  // pos2
    std::vector<int> deg_vals_1 = sigma_deg_vals(deg_vals, n, i, j);  // fir=1, sec>1
    int new_n = n - 1;
    create_baza(baza, new_n);
    std::vector<int> new_deg_vals = get_new_deg_vals(deg_vals_1, n);  //  cnt=n-1

    std::vector<std::string> help_matrix = create_maxtix(new_n, new_deg_vals, baza);

    std::vector<std::string> help_matrix_1 = matrix_with_new_v(help_matrix, new_n, get_new_v(n));

    P_of_rows(help_matrix_1, n, i, j);
    P_of_cols(help_matrix_1, n, i, j);

    return help_matrix_1;

} 


std::vector<std::string> create_matrix_8x8(int n, std::vector<int> deg_vals,
                                         std::vector<std::vector<std::string>> baza){

    int i = 0;  // pos1
    int j = 0;  // pos2
    std::vector<int> deg_vals_1 = sigma_deg_vals(deg_vals, n, i, j);  // fir=1, sec>1
    int new_n = n - 1;
    create_baza(baza, new_n);
    std::vector<int> new_deg_vals = get_new_deg_vals(deg_vals_1, n);  //  cnt=n-1

    std::vector<std::string> help_matrix = create_matrix_7x7(new_n, new_deg_vals, baza);

    std::vector<std::string> help_matrix_1 = matrix_with_new_v(help_matrix, new_n, get_new_v(n));

    P_of_rows(help_matrix_1, n, i, j);
    P_of_cols(help_matrix_1, n, i, j);

    return help_matrix_1;

} 


std::vector<std::string> create_matrix_9x9(int n, std::vector<int> deg_vals,
                                         std::vector<std::vector<std::string>> baza){

    int i = 0;  // pos1
    int j = 0;  // pos2
    std::vector<int> deg_vals_1 = sigma_deg_vals(deg_vals, n, i, j);  // fir=1, sec>1
    int new_n = n - 1;
    create_baza(baza, new_n);
    std::vector<int> new_deg_vals = get_new_deg_vals(deg_vals_1, n);  //  cnt=n-1

    std::vector<std::string> help_matrix = create_matrix_8x8(new_n, new_deg_vals, baza);

    std::vector<std::string> help_matrix_1 = matrix_with_new_v(help_matrix, new_n, get_new_v(n));

    P_of_rows(help_matrix_1, n, i, j);
    P_of_cols(help_matrix_1, n, i, j);

    return help_matrix_1;

} 


std::vector<std::string> create_matrix_10x10(int n, std::vector<int> deg_vals,
                                         std::vector<std::vector<std::string>> baza){

    int i = 0;  // pos1
    int j = 0;  // pos2
    std::vector<int> deg_vals_1 = sigma_deg_vals(deg_vals, n, i, j);  // fir=1, sec>1
    int new_n = n - 1;
    create_baza(baza, new_n);
    std::vector<int> new_deg_vals = get_new_deg_vals(deg_vals_1, n);  //  cnt=n-1

    std::vector<std::string> help_matrix = create_matrix_9x9(new_n, new_deg_vals, baza);

    std::vector<std::string> help_matrix_1 = matrix_with_new_v(help_matrix, new_n, get_new_v(n));

    P_of_rows(help_matrix_1, n, i, j);
    P_of_cols(help_matrix_1, n, i, j);

    return help_matrix_1;

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
    int i, j = 0;
    std::cout << "check of new_deg_vals: \n";
    print_vector(sigma_deg_vals(deg_values, n, i, j));
    print_vector(get_new_deg_vals(sigma_deg_vals(deg_values, n, i, j), n));
    std::cout << "check is over, (" << i << "\t" << j << ")\n";

    std::vector<std::string> vector;  // has 2^(n) strings [n = 3: 000 001 010 011 100 101 110 111]

    std::vector<std::vector<std::string>> baza;

    create_baza(baza, n);

    std::cout << "\n\n";
 //   print_vector_of_vectors(baza);
    std::cout << "\n";
    std::vector<std::string> matrix;
    if(n == 7) matrix = create_matrix_7x7(n, deg_values, baza);
    else if(n == 8) matrix = create_matrix_8x8(n, deg_values, baza);
    else if(n == 9) matrix = create_matrix_9x9(n, deg_values, baza);
    else if(n == 10) matrix = create_matrix_10x10(n, deg_values, baza);
    else matrix = create_maxtix(n, deg_values, baza);

    print_matrix(matrix);

#if 0

    std::vector<std::string> matrix = create_maxtix(n, deg_values, baza);
    print_matrix(matrix);

    deg_values.clear();
    int d = 0;
    for(int j = 0; j < n-1; ++j){
        std::cout << "Введите степень вершины ";
        std::cin >> d;
        deg_values.push_back(d);
    }

    int new_n = n - 1;
    create_baza(baza, new_n);
    std::vector<std::string> matrix_help = create_maxtix(new_n, deg_values, baza);
    print_matrix(matrix_help);


    std::vector<std::string> matrix1 = {
                                        "001100",
                                        "000100",
                                        "100000",
                                        "110011",
                                        "000100",
                                        "000100"
                                        };
    if(connectivity(matrix1, new_n)) std::cout << "yeeeeeeeeeeeah\n";

    std::vector<std::string> new_matrix = matrix_with_new_v(matrix1, new_n, "0100000");

    print_matrix(new_matrix);

    P_of_rows(new_matrix, n, 2, 3);

    print_matrix(new_matrix);

    P_of_cols(new_matrix, n, 2, 3);

    print_matrix(new_matrix);


    if(connectivity(new_matrix, n)) std::cout << "all right\n";

#endif

}


// baza -- (vec_1, vec_2, ..., vec_n), где vec_i -- вектор, содержащий строки с i количеством единиц
// is_k_1(st) -- проверка, что в строке содержится k единиц