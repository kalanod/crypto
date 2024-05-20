#include <cstdio>
#include "hash.cpp"
#include "User.h"
#include "BigInt.cpp"
#include <stdlib.h>

void standard(int SIZE) {
    big_integer p = big_integer::random(SIZE);
    for (int i = 0; i<1000000; i++);
    big_integer g = big_integer::random(SIZE);
    User a(SIZE, "Alice", p, g);
    for (int i = 0; i<1000000; i++);
    User b(SIZE, "Bob", p, g);
    cout << "generated p:" << a.p << ", g:" << a.g << endl;
    std::cout << std::endl;
    cout << "generated Alice a:" << a.a << endl;
    cout << "generated Bob b:" << b.a << endl;
    cout << endl;
    //cout<<big_integer(222222222).mod_pow(16, 257)<<endl;

    a.calculate_A();
    cout << "generated Alice A:" << a.A << endl;
    std::cout << std::endl;

    b.A = a.A.copy();
    b.calculate_B();
    cout << "generated Bob B:" << b.B << endl;
    //cout << "generated Bob p:" << a.p << ", g:" << a.g << endl;
    //cout << "generated Bob A:" << a.A << endl;
    std::cout << std::endl;

    a.B = b.B.copy();
    // Alice generate K
    a.calculate_K_by_B();
    cout << "generated Alice K:" << a.K << endl;
    std::cout << std::endl;
    b.calculate_K_by_A();
    cout << "generated Bob K:" << a.K << endl;
    std::cout << std::endl;
    std::cout << std::endl;
}

void manInTheMiddle(int SIZE) {
    big_integer p = big_integer::random(SIZE);
    for (int i = 0; i<1000000; i++);
    big_integer g = big_integer::random(SIZE);
    cout << "generated p:" << p << ", g:" << g << endl;
    User a(SIZE, "Alice", p, g);
    for (int i = 0; i<1000000; i++);
    User b(SIZE, "Bob", p, g);

    std::cout << std::endl;
    cout << "generated Alice a:" << a.a << endl;
    cout << "generated Bob b:" << b.a << endl;
    cout << endl;
    //cout<<big_integer(222222222).mod_pow(16, 257)<<endl;

    a.calculate_A();
    cout << "generated Alice A:" << a.A << endl;
    std::cout << std::endl;

    User eva(SIZE, "E", p, g);
    eva.A = a.A.copy();
    big_integer fake_a = big_integer::random(SIZE);
    big_integer fake_A = g.mod_pow(fake_a, p);
    std::cout <<"Fake A: "<< fake_A << std::endl;
    b.A = fake_A.copy();

    b.calculate_B();
    b.calculate_K_by_A();

    eva.B = b.B.copy();

    big_integer fake_b = big_integer::random(SIZE);
    big_integer fake_B = g.mod_pow(fake_b, p);

    a.B = fake_B.copy();
    a.calculate_K_by_B();
    std::cout << std::endl;
    std::cout << a.K << " " <<b.K << std::endl;
}

int main() {
    manInTheMiddle(16);
    return 0;
}