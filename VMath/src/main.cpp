#include <iostream>

#include "../include/vmath.hpp"

int main() {






    float1xN<4> vec1({ 1,1,1,1 });
    float1xN<4> vec2({ 5,6,7,8 });

    floatNxM<3,3> mat = floatNxM<3,3>({
        {1,2,3},
        {4,5,6},
        {7,8,9}
    });

    std::cout << length(vec1) << std::endl;

    return 0;
}