#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <vector>

// Официальная таблица замен (S-блок) для ГОСТ 34.11-94
static const uint8_t Sbox[256] = {
    0x4, 0xA, 0x9, 0x2, 0xD, 0x8, 0x0, 0xE,
    0x6, 0xB, 0x1, 0xC, 0x7, 0xF, 0x5, 0x3,
    0xE, 0xB, 0x4, 0xD, 0x1, 0xA, 0x7, 0xC,
    0x5, 0x8, 0x3, 0xF, 0x9, 0x6, 0x0, 0x2,
    0x4, 0xA, 0x7, 0x0, 0x1, 0x3, 0x9, 0xC,
    0xE, 0x2, 0xB, 0xF, 0x8, 0x5, 0x6, 0xD,
    0xB, 0x8, 0xC, 0x7, 0x1, 0xE, 0x2, 0xD,
    0x6, 0xF, 0x0, 0x9, 0xA, 0x4, 0x5, 0x3,
    0xF, 0x2, 0xC, 0x7, 0xA, 0xD, 0x1, 0xE,
    0x0, 0xB, 0x9, 0x6, 0x4, 0x3, 0x5, 0x8,
    0x9, 0xE, 0xB, 0x7, 0x2, 0x0, 0x5, 0xA,
    0xC, 0x1, 0x4, 0x3, 0xD, 0x6, 0x8, 0xF,
    0x0, 0xE, 0xC, 0x2, 0xB, 0x1, 0x7, 0xD,
    0x8, 0xF, 0x5, 0xA, 0x6, 0x4, 0x9, 0x3,
    0xE, 0x7, 0x2, 0xD, 0x1, 0xB, 0x4, 0x8,
    0xC, 0x5, 0x6, 0x0, 0x9, 0xA, 0x3, 0xF
};


constexpr uint32_t A = 0x8E20FAA7;
constexpr uint32_t B = 0x9B05688C;
constexpr uint32_t C = 0x7EEC6A91;
constexpr uint32_t D = 0x2AAB10B6;
constexpr uint32_t E = 0x8E20FAA7;
constexpr uint32_t F = 0x9B05688C;
constexpr uint32_t G = 0x7EEC6A91;
constexpr uint32_t H = 0x2AAB10B6;

// Вспомогательные функции
uint32_t getWord(const char* data) {
    return static_cast<uint32_t>((static_cast<uint8_t>(data[0]) << 24) |
        (static_cast<uint8_t>(data[1]) << 16) |
        (static_cast<uint8_t>(data[2]) << 8) |
        static_cast<uint8_t>(data[3]));
}

void putWord(char* data, uint32_t word) {
    data[0] = static_cast<char>((word >> 24) & 0xFF);
    data[1] = static_cast<char>((word >> 16) & 0xFF);
    data[2] = static_cast<char>((word >> 8) & 0xFF);
    data[3] = static_cast<char>(word & 0xFF);
}

// Функция подстановки
uint32_t P(uint32_t x) {
    return (Sbox[static_cast<uint8_t>(x >> 24)] << 24) |
        (Sbox[static_cast<uint8_t>(x >> 16)] << 16) |
        (Sbox[static_cast<uint8_t>(x >> 8)] << 8) |
        Sbox[static_cast<uint8_t>(x)];
}

// Функция шифрования одного блока
void gostBlockEncrypt(const char* block, char* result) {
    uint32_t N1 = getWord(block);
    uint32_t N2 = getWord(block + 4);

    for (int i = 0; i < 3; ++i) {
        N2 ^= P(N1 + A);
        N1 ^= P(N2 + B);
        N2 ^= P(N1 + C);
        N1 ^= P(N2 + D);
        N2 ^= P(N1 + E);
        N1 ^= P(N2 + F);
        N2 ^= P(N1 + G);
        N1 ^= P(N2 + H);
    }

    N2 ^= P(N1 + A);
    N1 ^= P(N2 + B);

    putWord(result, N2);
    putWord(result + 4, N1);
}

// Хеширование
std::string gostHash(const std::string& input) {
    // Инициализация начального вектора
    uint32_t h[2] = { 0, 0 };

    // Дополнение сообщения нулями
    size_t paddedSize = (input.size() + 7) & ~0x7;
    char* paddedData = new char[paddedSize];
    std::memcpy(paddedData, input.c_str(), input.size());
    std::memset(paddedData + input.size(), 0, paddedSize - input.size());

    // Обработка блоков
    for (size_t i = 0; i < paddedSize; i += 8) {
        char block[8];
        std::memcpy(block, paddedData + i, 8);

        // Обновление вектора хеша
        gostBlockEncrypt(block, reinterpret_cast<char*>(h));
    }

    delete[] paddedData;

    // Преобразование хеша в строку
    std::string result;
    for (int i = 0; i < 2; ++i) {
        char buffer[9];
        std::sprintf(buffer, "%08x", h[i]);
        result += buffer;
    }

    return result;
}