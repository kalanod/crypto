#include <cstdio>
#include "hash.cpp"
#include <stdlib.h>


int hexDig(char ch) {
    if ((ch >= '0') && (ch <= '9')) return ch - '0';
    if ((ch >= 'A') && (ch <= 'F')) return ch - 'A' + 10;
    if ((ch >= 'a') && (ch <= 'f')) return ch - 'a' + 10;
    return -1;
}
bool compareBytes(const unsigned char* array1, const unsigned char* array2, int n) {
    for (int i = 0; i < n; ++i) {
        if (array1[i] != array2[i]) {
            return false; // Если найдено несовпадение, возвращаем false
        }
    }
    return true; // Если все байты совпадают, возвращаем true
}
int main() {
    int l = 5;
    unsigned char buf[] = {"kalan"};
    unsigned char datehash[] = {"f514646340b6b311191d8ecdc599fc2afb089d8d5ef4278698e3b3f1e359983"};
    cout<<datehash[0]<<endl;
    unsigned char test[32];
    unsigned char string1[64];
    GOST hash;
    cout << buf << sizeof(buf)<<endl;
    int ir = 0;
    hash.read(buf, sizeof(buf)-1);
    hash.gost();
    hash.print();
    cout << string1<<endl;
    while (!compareBytes(string1, datehash, 2)){
        if (ir % 10000 == 0) cout << ir << endl;

        for (unsigned char & i : test) {
            i = (1 + std::rand())%64;
        }
        hash.read(test, 32);
        hash.gost();
        hash.convertToHexArray(string1);
        ir++;
    }
    cout << ir << endl;


    return 0;
}