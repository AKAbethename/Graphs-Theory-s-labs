#include "graph.hpp"
#include "gamma.hpp"

/* std::random_device rd1;
std::mt19937 gen(rd1());
std::uniform_real_distribution<double> dist(0.0, 1.0); */

template <typename T>
void print_vector(const std::vector<T>& vec){
    std::cout << "{";
    for(auto it = vec.begin(); it != vec.end(); ++it){
        std::cout << *it;
        if(it != --vec.end()) std::cout << ",   "; 
    }
    std::cout << "}\n";
}


template <typename T>
void print_vector_of_vectors(std::vector<std::vector<T>> arr){
    std::cout << "{\n";
    for(const auto x: arr){
        print_vector(x);
    }
    std::cout << "}\n";
}

int randomBetween(int m, int n) {
    std::uniform_int_distribution<> dist(m, n);
    return dist(gen);
}

Graph::Graph(double lambda, double alpha, int n) : lambda(lambda), alpha(alpha), n(n), g(lambda, alpha, n) {
    create_baza(baza, n);
    deg_values = g.get_values();
}


std::string get_bin(int num, int length) {
    std::string result;
    
    // Сначала получаем двоичное представление без ведущих нулей
    for (int i = length - 1; i >= 0; i--) {
        result += ((num >> i) & 1) ? '1' : '0';
    }
    
    return result;
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

std::vector<std::string> get_right_vector(const std::vector<std::string>& vector_with_wrong, int i){  // i -- row index
    std::vector<std::string> right;
    for(auto x: vector_with_wrong){
        if(x[i] != '1') right.push_back(x);
    }
    return right;
}


bool check_one(std::vector<std::string> matrix, int n){
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(matrix[i][j] != matrix[j][i]) return false;
        }
    }
    return true;
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
    new_matrix.push_back(str);
    for(int i = 0; i < n; ++i){
        std::string new_str = str[i+1] + matrix[i];
        new_matrix.push_back(new_str);
    }
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



std::vector<std::string> create_matrix(const int& n, const std::vector<int>& degs, const std::vector<std::vector<std::string>>& baza){  // n - size of square matrix
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


void Graph::print_baza(){print_vector_of_vectors(baza);}

std::vector<std::string> Graph::get_matrix(){return matrix;}

std::vector<int> Graph::get_deg_values(){return deg_values;}

std::vector<std::vector<std::string>> Graph::get_baza(){return baza;}



// setters

void Graph::set_matrix(const std::vector<std::string>& M) {matrix.clear(); matrix = M;}

void print_matrix(const std::vector<std::string>& matrix){
    std::cout << "{\n";
    for(auto it = matrix.begin(); it != matrix.end(); ++it){
        std::cout << *it << "\n";
    }
    std::cout << "}\n";
}


std::vector<std::string> create_matrix_7x7(int n, std::vector<int> deg_vals,
                                         std::vector<std::vector<std::string>> baza){

    int i = 0;  // pos1
    int j = 0;  // pos2
    std::vector<int> deg_vals_1 = sigma_deg_vals(deg_vals, n, i, j);  // fir=1, sec>1
    int new_n = n - 1;
    create_baza(baza, new_n);
    std::vector<int> new_deg_vals = get_new_deg_vals(deg_vals_1, n);  //  cnt=n-1

    std::vector<std::string> help_matrix = create_matrix(new_n, new_deg_vals, baza);

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


std::vector<std::string> create_matrix_11x11(int n, std::vector<int> deg_vals,
                                         std::vector<std::vector<std::string>> baza){

    int i = 0;  // pos1
    int j = 0;  // pos2
    std::vector<int> deg_vals_1 = sigma_deg_vals(deg_vals, n, i, j);  // fir=1, sec>1
    int new_n = n - 1;
    create_baza(baza, new_n);
    std::vector<int> new_deg_vals = get_new_deg_vals(deg_vals_1, n);  //  cnt=n-1

    std::vector<std::string> help_matrix = create_matrix_10x10(new_n, new_deg_vals, baza);

    std::vector<std::string> help_matrix_1 = matrix_with_new_v(help_matrix, new_n, get_new_v(n));

    P_of_rows(help_matrix_1, n, i, j);
    P_of_cols(help_matrix_1, n, i, j);

    return help_matrix_1;

} 


