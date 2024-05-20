#include <iostream>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include "mb5.cpp"
#include "big_Integer.cpp"

using namespace std;

// Функция для проверки числа на простоту
bool isPrime(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Функция для поиска простого числа
long long generatePrime(long long min, long long max) {
    long long num = rand() % (max - min + 1) + min;
    while (!isPrime(num)) {
        num = rand() % (max - min + 1) + min;
    }
    return num;
}

// Функция для вычисления модулярного обратного
long long modInverse(long long a, long long m) {
    a = a % m;
    for (long long x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // Обратного элемента не существует
}

long long mod_pow(long long base, long long exponent, long long modulus) {
    if (modulus == 1) return 0; // Невозможно определить по модулю 1

    long long result = 1;
    base = base % modulus; // Сокращение базы

    while (exponent > 0) {
        // Если степень нечётная, умножаем результат на базу
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        // Сокращаем степень вдвое и базу возводим в квадрат
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    return result;
}
int generateRandomNumber(int length) {
    int min = 1;
    for (int i = 1; i < length; ++i) {
        min *= 10;
    }
    int max = min * 10 - 1;
    return min + rand() % (max - min + 1);
}
// Функция для восстановления секретного ключа x
long long recoverPrivateKey(long long r, long long s, long long k, long long Hm, long long q) {
    long long x = (s * k - Hm);
    x = (x * modInverse(r, q)) % q + q;
    return x;
}
// Генерация ключей DSA
void generateDSAKeys(long long &p, long long &q, long long &g, long long &x, long long &y) {
    // Генерация простых чисел p и q
    q = generatePrime(10, 100);
    big_integer bq(q);
    do {
        p = generatePrime(100000, 1000000);
        //cout << (p - 1) % q << endl;
    } while ((p - 1) % q != 0);
    big_integer bp(p);
    // Выбор генератора g
    g = mod_pow(2, (p - 1) / q, p);

    // Генерация закрытого ключа x
    x = generateRandomNumber(4) % (q - 2) + 1;

    // Вычисление открытого ключа y
    y = mod_pow(g, x, p);
}


// Создание подписи для сообщения
void createDSASignature(long long p, long long q, long long g, long long x, long long &r, long long &s, long long message) {
    long long k;
    big_integer rbig(g);
    big_integer sbig(g);
    do {
        do {
            k = rand() % q;
            k = 6;
            rbig = g;
            rbig = rbig.mod_pow(k, p) % q;
        } while (rbig == 0);
        uint64_t hash_as_int = message;
//    for (size_t i = 0; i < 16; ++i) {
//        hash_as_int += static_cast<uint64_t>(message[i]) << (8 * i);
//    }
        sbig = (modInverse(k, q) * (hash_as_int + x * rbig)) % q;
    } while (sbig == 0);
    cout << "k: "<<k<<endl;
    s = sbig.vectorToInt();
    r = rbig.vectorToInt();
}

// Проверка подписи
bool
verifyDSASignature(long long p, long long q, long long g, long long y, long long r, long long s, long long message) {
    if (r < 0 || r >= q || s < 0 || s >= q) return false;

    long long w = modInverse(s, q);
    big_integer u1((message * w) % q);
    big_integer u2((r * w) % q);
    big_integer gp(g);
    gp = gp.pow(u1);
    big_integer yp(y);
    yp = yp.pow(u2);
    gp = ((gp * yp) % p) % q;
    long long v = gp.vectorToInt();
    cout << v << " " << u2 << " " << r << endl;
    return v == r;
}

uint32_t hexToDec(const std::string &hexString) {
    std::stringstream ss;
    ss << std::hex << hexString; // Устанавливаем флаг для считывания в шестнадцатеричном формате
    uint32_t decValue;
    ss >> decValue; // Считываем в десятичном формате
    return decValue;
}

int main() {
    srand(time(NULL));
    long long p, q, g, x, y;
    string hash = md5("Андрей очень хороший студент");
    cout << hash << endl;
    generateDSAKeys(p, q, g, x, y);
    cout << "privat key: " << x << endl;
    cout << "public key: " << y << " p: " << p << " q: " << q << " g: " << g << endl;
    long long r, s;
    createDSASignature(p, q, g, x, r, s, hexToDec(hash));
    cout << "r: " << r << " s: " << s << endl;
    bool isValid = verifyDSASignature(p, q, g, y, r, s, hexToDec(hash));

    cout << "attack result: " << recoverPrivateKey(r, s, 6, hexToDec(hash), q) << endl;
    if (isValid) {
        cout << "Signature is valid!" << endl;
    } else {
        cout << "Signature is not valid!" << endl;
    }

    return 0;
}
