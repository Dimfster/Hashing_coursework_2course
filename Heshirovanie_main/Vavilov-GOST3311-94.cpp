#include <iostream>
#include <iomanip>
#include <cstring>

// Функция для выполнения преобразования по таблице замен
void substitutionTransform(const uint8_t* input, uint8_t* output) {
    static const uint8_t sBox[8][16] = {
        {4, 10, 9, 2, 13, 8, 0, 14, 6, 11, 1, 12, 7, 15, 5, 3},
        {14, 11, 4, 12, 6, 13, 15, 10, 2, 3, 8, 1, 0, 7, 5, 9},
        {5, 8, 1, 13, 10, 3, 4, 2, 14, 15, 12, 7, 6, 0, 9, 11},
        {7, 13, 10, 1, 0, 8, 9, 15, 14, 4, 6, 12, 11, 2, 5, 3},
        {6, 12, 7, 1, 5, 15, 13, 8, 4, 10, 9, 14, 0, 3, 11, 2},
        {4, 11, 10, 0, 7, 2, 1, 13, 3, 6, 8, 5, 9, 12, 15, 14},
        {13, 11, 4, 1, 3, 15, 5, 9, 0, 10, 14, 7, 6, 8, 2, 12},
        {1, 15, 13, 0, 5, 7, 10, 4, 9, 2, 3, 14, 6, 11, 8, 12}
    };

    for (int i = 0; i < 32; i += 4) {
        uint8_t row = (input[i] << 1) | (input[i + 1] << 0);
        uint8_t col = (input[i + 2] << 3) | (input[i + 3] << 2) | (input[i + 3] << 1) | (input[i + 3] << 0);

        uint8_t value = sBox[row][col];

        output[i / 2] = (value >> 3) & 0x1;
        output[i / 2 + 16] = (value >> 2) & 0x1;
        output[i / 2 + 32] = (value >> 1) & 0x1;
        output[i / 2 + 48] = value & 0x1;
    }
}

// Функция для выполнения перестановок
void permutationTransform(const uint8_t* input, uint8_t* output) {
    static const int permutationTable[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
    };

    for (int i = 0; i < 64; ++i) {
        output[i] = input[permutationTable[i] - 1];
    }
}

// Функция для выполнения одного раунда хэширования
void hashRound(const uint8_t* input, uint8_t* output) {
    uint8_t substituted[64];
    substitutionTransform(input, substituted);

    uint8_t permuted[64];
    permutationTransform(substituted, permuted);

    memcpy(output, permuted, 64);
}

// Функция для выполнения полного хэширования
void gostHash(const std::string& input, uint8_t* output) {
    // Инициализация начального блока данных
    uint8_t block[64];
    memset(block, 0, sizeof(block));
    memcpy(block, input.c_str(), std::min(input.size(), static_cast<size_t>(64)));

    // Выполнение 32 раундов хэширования
    for (int i = 0; i < 32; ++i) {
        hashRound(block, block);
    }

    // Результирующий хэш
    memcpy(output, block, 32);
}

int print() {
    std::string input = "bitcoin";
    uint8_t hash[32];

    gostHash(input, hash);

    std::cout << "Input: " << input << std::endl;
    std::cout << "Hash: ";
    for (int i = 0; i < 32; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    std::cout << std::endl;

    return 0;
}
