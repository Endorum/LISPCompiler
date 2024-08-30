//
// Created by Alexander on 24.08.24.
//

#ifndef TESTCLASS_HPP
#define TESTCLASS_HPP

#include <cstdio>

class Object {
public:
    int parameter;
    explicit Object(int init) : parameter(init) {}

    void print() {
        // printf("parameter: %d\n",parameter);
        parameter += 1;
    }

};

#endif //TESTCLASS_HPP
