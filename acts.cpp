#include <iostream>
#include <vector>
#include <limits>

#include "acts.hpp"
#include "gamma.hpp"
#include "graph.hpp"


template <typename T>
void print_vector(const std::vector<T>& vec){
    std::cout << "{";
    for(auto it = vec.begin(); it != vec.end(); ++it){
        std::cout << *it;
        if(it != --vec.end()) std::cout << ",   "; 
    }
    std::cout << "}\n";
}


void act1(){
    Gamma g;
    g.show();
}


void act2(){
    double lambda, alpha;
    int n;
    std::cout << "Введите параметр распредения \\ : ";
    std::cin >> lambda;
    std::cout << "Введите параметр распредения alpha : ";
    std::cin >> alpha;
    std::cout << "Введите количество вершин дерева: ";
    std::cin >> n;
    Graph tree(lambda, alpha, n);
//    tree.print_baza();   
    std::cout << "Вектор степеней вершин: \n";
    print_vector(tree.get_deg_values());
    std::vector<std::string> matrix;
    if(n <= 6) matrix = create_matrix(n, tree.get_deg_values(), tree.get_baza());
    else if(n == 7) matrix = create_matrix_7x7(n, tree.get_deg_values(), tree.get_baza());
    else if(n == 8) matrix = create_matrix_8x8(n, tree.get_deg_values(), tree.get_baza());
    else if(n == 9) matrix = create_matrix_9x9(n, tree.get_deg_values(), tree.get_baza());
    else if(n == 10) matrix = create_matrix_10x10(n, tree.get_deg_values(), tree.get_baza());
    else if(n == 11) matrix = create_matrix_11x11(n, tree.get_deg_values(), tree.get_baza());
    else std::cout << "Ошибка, более 10 вершин не может быть\n";
    
    tree.set_matrix(matrix);
    print_matrix(tree.get_matrix());
}
