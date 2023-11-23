#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <iomanip>

using namespace std;

string sha1(string input) {
    // Инициализация начальных значений
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;

    // Представление входных данных в виде массива байтов
    const uint8_t* data = reinterpret_cast<const uint8_t*>(input.c_str());
    size_t length = input.length();

    // Добавление бита 1 в конец входных данных
    string paddedInput = input + static_cast<char>(0x80);

    // Добавление нулей в конец входных данных, чтобы их длина была кратна 64
    size_t paddedLength = paddedInput.length();
    while (paddedLength % 64 != 56) {
        paddedInput += static_cast<char>(0x00);
        paddedLength++;
    }

    // Добавление длины входных данных в конец в битах (big-endian)
    uint64_t bitLength = length * 8;
    for (int i = 56; i >= 0; i -= 8) {
        paddedInput += static_cast<char>((bitLength >> i) & 0xFF);
    }

    // Вычисление хэша
    for (size_t i = 0; i < paddedLength; i += 64) {
        const uint32_t* block = reinterpret_cast<const uint32_t*>(paddedInput.c_str() + i);

        uint32_t w[80];
        for (int j = 0; j < 16; j++) {
            w[j] = block[j];
        }
        for (int j = 16; j < 80; j++) {
            w[j] = (w[j - 3] ^ w[j - 8] ^ w[j - 14] ^ w[j - 16]);
            w[j] = (w[j] << 1) | (w[j] >> 31);
        }

        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;

        for (int j = 0; j < 80; j++) {
            uint32_t f, k;
            if (j < 20) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            } else if (j < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            } else if (j < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            } else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

            uint32_t temp = (a << 5) | (a >> 27);
            temp += f + e + k + w[j];
            e = d;
            d = c;
            c = (b << 30) | (b >> 2);
            b = a;
            a = temp;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    // Конкатенация хэш-значений
     stringstream ss;
    ss <<  hex <<  setfill('0');
    ss <<  setw(8) << h0;
    ss <<  setw(8) << h1;
    ss <<  setw(8) << h2;
    ss <<  setw(8) << h3;
    ss <<  setw(8) << h4;

    return ss.str();
}

