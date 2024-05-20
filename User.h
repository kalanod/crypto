//
// Created by yaidf on 30.04.2024.
//

#ifndef LAB19_USER_H
#define LAB19_USER_H

#include <openssl/bn.h>
#include <iostream>
#include "big_Integer.cpp"

class User {
private:
    BN_CTX *ctx;
    std::string name;
    int SIZE;
public:
    User(int size, const std::string &name, const big_integer &pv, const big_integer &gv) {
        SIZE = size;
        a = big_integer::random(size);
        b = big_integer::random(size);
        g = gv;
        p = pv;
        A = g.copy();
        K = a.copy();
        B = g.copy();
        this->name = name;
    }
    void calculate_A() {
        A = A.mod_pow(a, p);
    }
    void calculate_B() {
        B = B.mod_pow(a, p);
    }
    void calculate_K_by_A() {
        K = A.copy();
        K = K.mod_pow(a, p);
    }
    void calculate_K_by_B() {
        K = B.copy();
        K = K.mod_pow(a, p);
    }
    big_integer a;
    big_integer b;
    big_integer p;
    big_integer g;
    big_integer A;
    big_integer B;
    big_integer K;
};

#endif //LAB19_USER_H
