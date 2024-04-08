#include <iostream>
#include "s21_list.h"
#include <list>

int main() {
    S21List<int> l1 = {1, 2, 3, 4, 5}; 
    
    /* l1.pop_back(); */
    l1.push_front(5);
    auto it = l1.begin();
    auto it_end = l1.end();
    for (; it != it_end; ++it) {
        std::cout << *it << " ";
    }

    return 0;
}
