#include <cstring>
#include <vector>

// Constants
const std::vector<uint32_t> T = []() {
    std::vector<uint32_t> temp(64);
    for (int i = 0; i < 64; ++i) {
        temp[i] = static_cast<uint32_t>(std::abs(std::sin(i + 1)) * std::pow(2, 32)) & 0xFFFFFFFF;
    }
    return temp;
}();

const std::vector<std::vector<int>> s = {
        {7, 12, 17, 22}, {7, 12, 17, 22}, {7, 12, 17, 22}, {7, 12, 17, 22},
        {5, 9, 14, 20}, {5, 9, 14, 20}, {5, 9, 14, 20}, {5, 9, 14, 20},
        {4, 11, 16, 23}, {4, 11, 16, 23}, {4, 11, 16, 23}, {4, 11, 16, 23},
        {6, 10, 15, 21}, {6, 10, 15, 21}, {6, 10, 15, 21}, {6, 10, 15, 21}
};

uint32_t left_rotate(uint32_t n, int b) {
    return (n << b) | (n >> (32 - b));
}

std::string md5(const std::string& input) {
    std::vector<uint8_t> message(input.begin(), input.end());
    uint64_t length = (8 * message.size()) & 0xFFFFFFFFFFFFFFFF;
    message.push_back(0x80);
    while (message.size() % 64 != 56) {
        message.push_back(0x00);
    }
    for (int i = 0; i < 8; ++i) {
        message.push_back(static_cast<uint8_t>(length >> (i * 8)));
    }

    uint32_t A = 0x67452301, B = 0xEFCDAB89, C = 0x98BADCFE, D = 0x10325476;

    for (size_t i = 0; i < message.size(); i += 64) {
        std::vector<uint32_t> X(16);
        for (int j = 0; j < 16; ++j) {
            X[j] = message[i + 4 * j] | (message[i + 4 * j + 1] << 8) | (message[i + 4 * j + 2] << 16) | (message[i + 4 * j + 3] << 24);
        }

        uint32_t A_ = A, B_ = B, C_ = C, D_ = D;

        for (int j = 0; j < 64; ++j) {
            uint32_t F, F_index;
            if (j < 16) {
                F = (B & C) | ((~B) & D);
                F_index = j;
            } else if (j < 32) {
                F = (D & B) | ((~D) & C);
                F_index = (5 * j + 1) % 16;
            } else if (j < 48) {
                F = B ^ C ^ D;
                F_index = (3 * j + 5) % 16;
            } else {
                F = C ^ (B | (~D));
                F_index = (7 * j) % 16;
            }

            uint32_t dTemp = D;
            D = C;
            C = B;
            B = B + left_rotate((A + F + T[j] + X[F_index]), s[j / 16][j % 4]);
            A = dTemp;
        }

        A = (A + A_) & 0xFFFFFFFF;
        B = (B + B_) & 0xFFFFFFFF;
        C = (C + C_) & 0xFFFFFFFF;
        D = (D + D_) & 0xFFFFFFFF;
    }

    char buffer[33];
    snprintf(buffer, sizeof(buffer), "%08x%08x%08x%08x", A, B, C, D);
    return std::string(buffer);
}


