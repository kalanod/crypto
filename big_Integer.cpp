#include <vector>
#include <string>
#include <ostream>
#include <iomanip>
#include <sstream>
#include <random>

class big_integer {
    // основание системы счисления (1 000 000 000)
    static const int BASE = 1000000000;

    // внутреннее хранилище числа
    std::vector<int> _digits;

    // знак числа
    bool _is_negative;

    void _remove_leading_zeros();

    void _shift_right();

public:
    big_integer(const big_integer& other) : _digits(other._digits), _is_negative(other._is_negative) {}
    // класс-исключение, бросаемое при делении на ноль
    class divide_by_zero : public std::exception {
    };

    big_integer();

    big_integer(std::string);

    big_integer(signed char);

    big_integer(unsigned char);

    big_integer(signed short);

    big_integer(unsigned short);

    big_integer(signed int);

    big_integer(unsigned int);

    big_integer(signed long);

    big_integer(unsigned long);

    big_integer(signed long long);

    big_integer(unsigned long long);

    friend std::ostream &operator<<(std::ostream &, const big_integer &);

    operator std::string() const;

    const big_integer operator+() const;

    const big_integer operator-() const;

    const big_integer operator++();

    const big_integer operator++(int);

    const big_integer operator--();

    const big_integer operator--(int);

    friend bool operator==(const big_integer &, const big_integer &);

    friend bool operator<(const big_integer &, const big_integer &);

    friend bool operator!=(const big_integer &, const big_integer &);

    friend bool operator<=(const big_integer &, const big_integer &);

    friend bool operator>(const big_integer &, const big_integer &);

    friend bool operator>=(const big_integer &, const big_integer &);

    friend const big_integer operator+(big_integer, const big_integer &);

    big_integer &operator+=(const big_integer &);

    friend const big_integer operator-(big_integer, const big_integer &);

    big_integer &operator-=(const big_integer &);

    friend const big_integer operator*(const big_integer &, const big_integer &);

    big_integer &operator*=(const big_integer &);

    friend const big_integer operator/(const big_integer &, const big_integer &);

    big_integer &operator/=(const big_integer &);

    friend const big_integer operator%(const big_integer &, const big_integer &);

    big_integer &operator%=(const big_integer &);

    bool odd() const;

    bool even() const;

    const big_integer pow(big_integer) const;

    long long int vectorToInt();

    long long int mod_pow(long long int exponent, long long int modulus);

    static big_integer random(int num_digits);

    big_integer copy() const;

    static big_integer random_prime(int num_digits);

    big_integer mod_pow(const big_integer &exponent, const big_integer &modulus) const;
};

// создает длинное целое число со значением 0
big_integer::big_integer() {
    this->_is_negative = false;
}
big_integer big_integer::mod_pow(const big_integer& exponent, const big_integer& modulus) const {
    big_integer result(1);
    big_integer base(*this);
    base %= modulus; // Reduce the base

    big_integer exp = exponent;
    while (exp > 0) {
        if (exp.odd()) {
            result *= base;
            result %= modulus;
        }
        base = base.pow(2);
        base %= modulus;
        exp /= 2;
    }
    return result;
}
// создает длинное целое число из C++-строки
big_integer::big_integer(std::string str) {
    if (str.length() == 0) {
        this->_is_negative = false;
    } else {
        if (str[0] == '-') {
            str = str.substr(1);
            this->_is_negative = true;
        } else {
            this->_is_negative = false;
        }

        for (long long i = str.length(); i > 0; i -= 9) {
            if (i < 9)
                this->_digits.push_back(atoi(str.substr(0, i).c_str()));
            else
                this->_digits.push_back(atoi(str.substr(i - 9, 9).c_str()));
        }

        this->_remove_leading_zeros();
    }
}

// удаляет ведущие нули
void big_integer::_remove_leading_zeros() {
    while (this->_digits.size() > 1 && this->_digits.back() == 0) {
        this->_digits.pop_back();
    }

    if (this->_digits.size() == 1 && this->_digits[0] == 0) this->_is_negative = false;
}
// Создает копию данного длинного целого числа
big_integer big_integer::copy() const {
    big_integer copy;
    copy._digits.resize(this->_digits.size()); // Размер массива копии равен размеру оригинала
    std::copy(this->_digits.begin(), this->_digits.end(), copy._digits.begin()); // Копируем цифры
    copy._is_negative = this->_is_negative; // Копируем знак
    return copy;
}

// Генерирует случайное простое число заданной длины
big_integer big_integer::random_prime(int num_digits) {
    // Инициализация генератора случайных чисел
    static std::mt19937 gen(std::time(nullptr)); // Static ensures that the generator is initialized only once
    std::uniform_int_distribution<int> distrib(0, BASE - 1);

    // Функция для проверки числа на простоту
    auto is_prime = [](const big_integer& n) {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (big_integer i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    };

    // Генерация случайного числа и проверка его на простоту
    big_integer prime_candidate;
    do {
        prime_candidate._digits.clear();
        prime_candidate._is_negative = false; // Простые числа всегда неотрицательные

        // Генерация случайных цифр для каждой позиции числа
        for (int i = 0; i < num_digits; ++i) {
            prime_candidate._digits.push_back(distrib(gen)%10);
        }

        // Убираем ведущие нули
        prime_candidate._remove_leading_zeros();

        // Проверяем на простоту
    } while (!is_prime(prime_candidate));

    return prime_candidate;
}

big_integer big_integer::random(int num_digits) {
    // Инициализация генератора случайных чисел
    static std::mt19937 gen(std::time(nullptr)); // Static ensures that the generator is initialized only once
    std::uniform_int_distribution<int> distrib(0, BASE - 1);

    // Создание случайного числа
    big_integer result;
    result._is_negative = false; // Случайные числа всегда неотрицательные

    // Генерация случайных цифр для каждой позиции числа
    int ro = 1;
    for (int i = 0; i < num_digits; i++) ro *= 10;
    for (int i = 0; i < num_digits / 8 + 1; ++i) {
        result._digits.push_back(distrib(gen) % ro);
    }

    // Убираем ведущие нули
    result._remove_leading_zeros();

    return result;
}
// печатает число в поток вывода
std::ostream &operator<<(std::ostream &os, const big_integer &bi) {
    if (bi._digits.empty()) os << 0;
    else {
        if (bi._is_negative) os << '-';
        os << bi._digits.back();
        for (long long i = static_cast<long long>(bi._digits.size()) - 2; i >= 0; --i)
            os << std::setw(1) << bi._digits[i];
    }
    return os;
}

// сравнивает два числа на равенство
bool operator==(const big_integer &left, const big_integer &right) {
    if (left._is_negative != right._is_negative) return false;
    if (left._digits.empty()) {
        if (right._digits.empty() || (right._digits.size() == 1 && right._digits[0] == 0)) return true;
        else return false;
    }

    if (right._digits.empty()) {
        if (left._digits.size() == 1 && left._digits[0] == 0) return true;
        else return false;
    }

    if (left._digits.size() != right._digits.size()) return false;
    for (size_t i = 0; i < left._digits.size(); ++i) if (left._digits[i] != right._digits[i]) return false;

    return true;
}

// возвращает копию переданного числа
const big_integer big_integer::operator+() const {
    return big_integer(*this);
}

// возвращает переданное число с другим знаком
const big_integer big_integer::operator-() const {
    big_integer copy(*this);
    copy._is_negative = !copy._is_negative;
    return copy;
}

// проверяет, является ли левый операнд меньше правого
bool operator<(const big_integer &left, const big_integer &right) {
    if (left == right) return false;
    if (left._is_negative) {
        if (right._is_negative) return ((-right) < (-left));
        else return true;
    } else if (right._is_negative) return false;
    else {
        if (left._digits.size() != right._digits.size()) {
            return left._digits.size() < right._digits.size();
        } else {
            for (long long i = left._digits.size() - 1; i >= 0; --i) {
                if (left._digits[i] != right._digits[i]) return left._digits[i] < right._digits[i];
            }

            return false;
        }
    }
}

// сравнивает два числа на неравенство
bool operator!=(const big_integer &left, const big_integer &right) {
    return !(left == right);
}

// проверяет, является ли левый операнд меньше либо равен правого
bool operator<=(const big_integer &left, const big_integer &right) {
    return (left < right || left == right);
}

// проверяет, является ли левый операнд больше правого
bool operator>(const big_integer &left, const big_integer &right) {
    return !(left <= right);
}

// проверяет, является ли левый операнд больше либо равен правого
bool operator>=(const big_integer &left, const big_integer &right) {
    return !(left < right);
}

// складывает два числа
const big_integer operator+(big_integer left, const big_integer &right) {
    if (left._is_negative) {
        if (right._is_negative) return -(-left + (-right));
        else return right - (-left);
    } else if (right._is_negative) return left - (-right);
    int carry = 0;
    for (size_t i = 0; i < std::max(left._digits.size(), right._digits.size()) || carry != 0; ++i) {
        if (i == left._digits.size()) left._digits.push_back(0);
        left._digits[i] += carry + (i < right._digits.size() ? right._digits[i] : 0);
        carry = left._digits[i] >= big_integer::BASE;
        if (carry != 0) left._digits[i] -= big_integer::BASE;
    }

    return left;
}

// прибавляет к текущему числу новое
big_integer &big_integer::operator+=(const big_integer &value) {
    return *this = (*this + value);
}

// префиксный инкремент
const big_integer big_integer::operator++() {
    return (*this += 1);
}

// преобразует число к строке
big_integer::operator std::string() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

// преобразует signed char к big_integer
big_integer::big_integer(signed char c) {
    if (c < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(c));
}

// преобразует unsigned char к big_integer
big_integer::big_integer(unsigned char c) {
    this->_is_negative = false;
    this->_digits.push_back(c);
}

// преобразует signed short к big_integer
big_integer::big_integer(signed short s) {
    if (s < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(s));
}

// преобразует unsigned short к big_integer
big_integer::big_integer(unsigned short s) {
    this->_is_negative = false;
    this->_digits.push_back(s);
}

// преобразует signed int к big_integer
big_integer::big_integer(signed int i) {
    if (i < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(i) % big_integer::BASE);
    i /= big_integer::BASE;
    if (i != 0) this->_digits.push_back(std::abs(i));
}

// преобразует unsigned int к big_integer
big_integer::big_integer(unsigned int i) {
    this->_digits.push_back(i % big_integer::BASE);
    i /= big_integer::BASE;
    if (i != 0) this->_digits.push_back(i);
}

// преобразует signed long к big_integer
big_integer::big_integer(signed long l) {
    if (l < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(l) % big_integer::BASE);
    l /= big_integer::BASE;
    if (l != 0) this->_digits.push_back(std::abs(l));
}

// преобразует unsigned long к big_integer
big_integer::big_integer(unsigned long l) {
    this->_digits.push_back(l % big_integer::BASE);
    l /= big_integer::BASE;
    if (l != 0) this->_digits.push_back(l);
}

// преобразует signed long long к big_integer
big_integer::big_integer(signed long long l) {
    if (l < 0) {
        this->_is_negative = true;
        l = -l;
    } else this->_is_negative = false;
    do {
        this->_digits.push_back(l % big_integer::BASE);
        l /= big_integer::BASE;
    } while (l != 0);
}

// преобразует unsigned long long к big_integer
big_integer::big_integer(unsigned long long l) {
    this->_is_negative = false;
    do {
        this->_digits.push_back(l % big_integer::BASE);
        l /= big_integer::BASE;
    } while (l != 0);
}

// постфиксный инкремент
const big_integer big_integer::operator++(int) {
    *this += 1;
    return *this - 1;
}

// префиксный декремент
const big_integer big_integer::operator--() {
    return *this -= 1;
}

// постфиксный декремент
const big_integer big_integer::operator--(int) {
    *this -= 1;
    return *this + 1;
}

// вычитает два числа
const big_integer operator-(big_integer left, const big_integer &right) {
    if (right._is_negative) return left + (-right);
    else if (left._is_negative) return -(-left + right);
    else if (left < right) return -(right - left);
    int carry = 0;
    for (size_t i = 0; i < right._digits.size() || carry != 0; ++i) {
        left._digits[i] -= carry + (i < right._digits.size() ? right._digits[i] : 0);
        carry = left._digits[i] < 0;
        if (carry != 0) left._digits[i] += big_integer::BASE;
    }

    left._remove_leading_zeros();
    return left;
}

// вычитает из текущего числа новое
big_integer &big_integer::operator-=(const big_integer &value) {
    return *this = (*this - value);
}

// перемножает два числа
const big_integer operator*(const big_integer &left, const big_integer &right) {
    big_integer result;
    result._digits.resize(left._digits.size() + right._digits.size());
    for (size_t i = 0; i < left._digits.size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < right._digits.size() || carry != 0; ++j) {
            long long cur = result._digits[i + j] +
                            left._digits[i] * 1LL * (j < right._digits.size() ? right._digits[j] : 0) + carry;
            result._digits[i + j] = static_cast<int>(cur % big_integer::BASE);
            carry = static_cast<int>(cur / big_integer::BASE);
        }
    }

    result._is_negative = left._is_negative != right._is_negative;
    result._remove_leading_zeros();
    return result;
}

// домножает текущее число на указанное
big_integer &big_integer::operator*=(const big_integer &value) {
    return *this = (*this * value);
}

// сдвигает все разряды на 1 вправо (домножает на BASE)
void big_integer::_shift_right() {
    if (this->_digits.size() == 0) {
        this->_digits.push_back(0);
        return;
    }
    this->_digits.push_back(this->_digits[this->_digits.size() - 1]);
    for (size_t i = this->_digits.size() - 2; i > 0; --i) this->_digits[i] = this->_digits[i - 1];
    this->_digits[0] = 0;
}

// делит два числа
const big_integer operator/(const big_integer &left, const big_integer &right) {
    if (right == 0) throw big_integer::divide_by_zero();
    big_integer b = right;
    b._is_negative = false;
    big_integer result, current;
    result._digits.resize(left._digits.size());
    for (long long i = static_cast<long long>(left._digits.size()) - 1; i >= 0; --i) {
        current._shift_right();
        current._digits[0] = left._digits[i];
        current._remove_leading_zeros();
        int x = 0, l = 0, r = big_integer::BASE;
        while (l <= r) {
            int m = (l + r) / 2;
            big_integer t = b * m;
            if (t <= current) {
                x = m;
                l = m + 1;
            } else r = m - 1;
        }

        result._digits[i] = x;
        current = current - b * x;
    }

    result._is_negative = left._is_negative != right._is_negative;
    result._remove_leading_zeros();
    return result;
}

// делит текущее число на указанное
big_integer &big_integer::operator/=(const big_integer &value) {
    return *this = (*this / value);
}

// возвращает остаток от деления двух чисел
const big_integer operator%(const big_integer &left, const big_integer &right) {
    big_integer result = left - (left / right) * right;
    if (result._is_negative) result += right;
    return result;
}

// присваивает текущему числу остаток от деления на другое число
big_integer &big_integer::operator%=(const big_integer &value) {
    return *this = (*this % value);
}

// проверяет, является ли текущее число нечетным
bool big_integer::odd() const {
    if (this->_digits.size() == 0) return false;
    return this->_digits[0] & 1;
}

// проверяет, является ли текущее число четным
bool big_integer::even() const {
    return !this->odd();
}

// возводит текущее число в указанную степень
const big_integer big_integer::pow(big_integer n) const {
    big_integer a(*this), result(1);
    while (n != 0) {
        if (n.odd()) result *= a;
        a *= a;
        n /= 2;
    }
    return result;
}

long long big_integer::mod_pow(long long exponent, long long modulus) {
    if (modulus == 1) return 0; // Невозможно определить по модулю 1
    big_integer result(1);
    big_integer base(*this);
    base %= modulus; // Сокращение базы
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    return result.vectorToInt();
}

long long big_integer::vectorToInt() {
    long long result = 0;
    for (int i = _digits.size() - 1; i >= 0; --i) {
        result = result * 10 + _digits[i];
    }
    return result;
}