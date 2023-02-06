#include "Matrix.hpp"

int main() {
    float data[] = {
        3, 0, 2,
        2, 0, -2,
        0, 1, 1
    };

    Matrix* m = new Matrix(
        data,
        3, 3
    );
    m->displayToConsole();

    std::cout << std::endl;

    Matrix* inverseM = m->inverse();

    inverseM->displayToConsole();

    std::cout << std::endl;

    m->multiply(inverseM)->displayToConsole();

    return 0;
}