#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "gamma.hpp"
#include <vector>
#include <string>


class Graph{
  private:
    int n;   // количество вершин
    double lambda;
    double alpha;
    Gamma g; 

    std::vector<std::vector<std::string>> baza;  // baza -- (vec_1, vec_2, ..., vec_n), где vec_i -- вектор, содержащий строки с i количеством единиц

    std::vector<int> deg_values;  // вектор степеней вершин

    std::vector<std::string> matrix;  // матрица смежности

  public:
    Graph(double lambda, double alpha, int n);

    void set_matrix(const std::vector<std::string>& M);

    void print_baza();

    std::vector<std::string> get_matrix();

    std::vector<std::vector<std::string>> get_baza();

    std::vector<int> get_deg_values();
    
};

    bool is_k_1(std::string str, int k);

    std::string get_bin(int num, int n);


    void create_baza(std::vector<std::vector<std::string>>& baza, int n);


    bool check_one(std::vector<std::string> matrix, int n);

    bool two_v(const std::vector<std::string>& matrix, int n, int i, int j, std::vector<bool>& visited);

    bool connectivity(const std::vector<std::string>& matrix, int n);

    std::vector<std::string> get_right_vector(const std::vector<std::string>& vector_with_wrong, int i);

    std::vector<std::string> create_matrix(const int& n, const std::vector<int>& degs, const std::vector<std::vector<std::string>>& baza);

    void print_matrix(const std::vector<std::string>& matrix);

    std::vector<int> sigma_deg_vals(const std::vector<int>& deg_vals, int n, int& i, int& j);  // i and j are poses for P

    std::string get_new_v(int n);


    std::vector<std::string> matrix_with_new_v(std::vector<std::string> matrix, int n, std::string str);  // n -- size of small matrix


    void P_of_rows(std::vector<std::string>& matrix, int n, int i, int j);  // (i) <-> (j) 

    void P_of_cols(std::vector<std::string>& matrix, int n, int i, int j);  // [i] <-> [j]

    std::vector<int> get_new_deg_vals(const std::vector<int>& deg_vals, int n);  // deg_vals is kanon: {1, k, ...}, k > 1



    std::vector<std::string> create_matrix_7x7(int n, std::vector<int> deg_vals,
                                         std::vector<std::vector<std::string>> baza); 


    std::vector<std::string> create_matrix_8x8(int n, std::vector<int> deg_vals,
                                            std::vector<std::vector<std::string>> baza);


    std::vector<std::string> create_matrix_9x9(int n, std::vector<int> deg_vals,
                                            std::vector<std::vector<std::string>> baza);


    std::vector<std::string> create_matrix_10x10(int n, std::vector<int> deg_vals,
                                            std::vector<std::vector<std::string>> baza);
                                
    std::vector<std::string> create_matrix_11x11(int n, std::vector<int> deg_vals,
                                            std::vector<std::vector<std::string>> baza);



#endif