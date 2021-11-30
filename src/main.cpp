#include "matrix.h"
#include <tuple>
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <cassert>
#include <string>

template<std::size_t N>
std::ostream& operator<<(std::ostream& os, const std::array<std::size_t, N>& arr) {
    os << "[ ";
    for (auto& item : arr) {
        os << item << ", ";
    }
    os << "]";
    return os;
}

int main() {
    sparse_matrix::Matrix<int, 0> matrix;

    for(std::size_t i = 0; i <= 9; ++i) {
        matrix[i][i] = i;
        matrix[i][9 - i] = i;
    }

    for (auto& [multi_index, value] : matrix) {
        std::cout << multi_index << ' ' << value << '\n';
    }

    std::cout << matrix.size() << "\n";
}
