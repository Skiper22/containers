#include "s21_vector.h"

#include <iostream>

void print(S21Vector<int> vec) {
    for (auto &el : vec) {
        std::cout << el << " ";
    }
    std::cout << "\n";
}

int main() {
    S21Vector<int> vec = {1, 2, 3, 4, 5};

    S21Vector<int>::iterator it_begin = vec.begin();
    S21Vector<int>::iterator it_end = vec.end();
    /* for (S21Vector<int>::iterator it = it_begin; it != it_end; ++it) { */
    /*     std::cout << *it << " "; */
    /* } */
    /* std::cout << "\n"; */
    /* for (int i = 0; i < 5; ++i) { */
    /*     vec.push_back(i); */
    /* } */
    auto it = vec.begin();
    /* for (int i = 0; i < vec.size() - 1; ++i) { */
    /*     it[i] = it[i + 1]; */
    /* } */
    
    vec.insert(it + 6, 25);
    print(vec);

    return 0;
}
