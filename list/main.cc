#include <iostream>
#include "s21_list.h"
#include <list>

using namespace s21;

template <typename T>
void print(List<T>& l) {
    for (auto& el : l) {
        std::cout << el << " ";
    }
    std::cout << "\n";
}

int main() {
    List<int> l1 = {1, 2, 3, 4, 5}; 
    
    l1.pop_back();
    l1.push_front(5);
    /* auto it = l1.begin(); */
    /* auto it_end = l1.end(); */
    /* for (; it != it_end; ++it) { */
    /*     std::cout << *it << " "; */
    /* } */

    List<int> l2 = {10, 20, 30};
    
    auto cit = l1.cbegin();
    /* ++cit; */
    l1.splice(cit, l2);
    
    print(l1);

    return 0;
}
