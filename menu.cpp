#include "menu.hpp"
#include "gamma.hpp"
#include "acts.hpp"

#include <vector>
#include <limits>

//void menu::print_value_from_bdd(const std::string& value){

//}




void Menu::print_menu(){
    std::cout << "\n";
    std::cout << "|--------------------------------------------------------------------------|\n";
    std::cout << "|         Лабораторная 1. Представление деревьев в программе               |\n";
    std::cout << "|--------------------------------------------------------------------------|\n";

//    char ans = 'b';

    std::string ans = "b";

while(ans != "7"){

    std::cout << "Вариант 26: гамма-расределение, алгоритм 2;\n\n";

    std::cout << "             |----------------------------------|\n";
    std::cout << "             |              Menu                |\n";
    std::cout << "             |----------------------------------|\n\n";

    std::vector<int> values = {1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1};

    std::cout << "|=================================================================================|\n";

    std::cout << "|                     Выберите действие из списка ниже:                           |\n";

    std::cout << "| [1] > информация о гамма-распределении                                          |\n";
    std::cout << "| [2] > сформировать дерево согласно данному распределению                        |\n";
    std::cout << "|                                                                                 |\n";
    std::cout << "| [3] > построить СДНФ данной функции                                             |\n";
    std::cout << "| [4] > вычислить значение данной функции для введенных переменных при помощи БДР |\n";
    std::cout << "| [5] > построить полином Жегалкина по данной функции                             |\n";
    std::cout << "| [6] > вычислить значение полинома Жегалгина от введенных переменных             |\n";
    std::cout << "| [7] > выйти                                                                     |\n";

    std::cout << "|=================================================================================|\n";


    std::cout << "\n";
    std::cin >> ans;

    if(ans[0] >= 49 && ans[0] <= 55 && ans.length() == 1) std::cout << "\nВаше действие -- [" << ans << "]\n\n";

if(ans.length() == 1){

    switch (ans[0])
    {
    case '1':
        act1();
        std::cout << "\n";
        break;

    case '2':
        act2();
    //    std::cout << "empty\n";
        break;

#if 0        
    case '3':
        SDNF(values);
        break;
    
    case '4':
        print_values_from_bdd();
        std::cout << "\n";
        break;

    case '5':
        print_Zhegalkin();
        std::cout << "\n";
        break;

    case '6':
        print_val_Zhegalkin();
        std::cout << "\n";
        break;

#endif
    case '7':
        break;
    default:
        std::cout << "Некорректный ввод! Попробуйте выбрать действие еще раз!\n\n";
        break;
    }
}

else{std::cout << "Некорректный ввод! Попробуйте выбрать действие еще раз!\n\n";}

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


}


}
