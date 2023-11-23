#include <iostream>
#include <string>
#include <bitset>
#include <cstdint>

// Функция для дополнения сообщения
std::string padMessage(const std::string& message, int rate) {
    std::string paddedMessage = message;

    // Добавляем бит "1" к сообщению
    paddedMessage += char(0x06); // В SHA-3 используется бит 0x06 вместо бита "1"

    // Вычисляем, сколько нулей нужно добавить до тех пор,
    // пока длина сообщения не станет кратной rate
    while ((paddedMessage.length() % rate) != (rate - 1)) {
        paddedMessage += char(0x00); // Добавляем нули
    }

    return paddedMessage;
}

// Функция ROL (циклический сдвиг влево)
constexpr uint64_t rotateLeft(uint64_t value, int shift) {
    return (value << shift) | (value >> (64 - shift));
}

// Функция впитывания
void absorb(uint64_t state[5][5][64], const std::string& paddedMessage) {
    int rate = 1600 / 8; // Размер блока (rate) для SHA-3-256 в байтах

    for (size_t i = 0; i < paddedMessage.length(); i += rate) {
        uint64_t block[5][5] = {0}; // Блок для хранения обрабатываемых данных

        // Преобразование блока из строки в нужный формат
        for (int j = 0; j < rate; j++) {
            int row = j / 8; // Определение строки в блоке (каждые 8 бит)
            int col = (j % 8) / 8; // Определение столбца в блоке (каждые 8 бит)
            block[row][col] |= (uint64_t(paddedMessage[i + j]) << ((j % 8) * 8));
        }

        // Впитывание блока в состояние
        for (int j = 0; j < 24; ++j) {
            // Theta
            uint64_t C[5], D[5];
            for (int x = 0; x < 5; ++x) {
                C[x] = block[x][0] ^ block[x][1] ^ block[x][2] ^ block[x][3] ^ block[x][4];
            }
            for (int x = 0; x < 5; ++x) {
                D[x] = C[(x + 4) % 5] ^ rotateLeft(C[(x + 1) % 5], 1);
            }
            for (int x = 0; x < 5; ++x) {
                for (int y = 0; y < 5; ++y) {
                    block[x][y] ^= D[x];
                }
            }

            // Ро
            uint64_t temp = block[0][1];
            for (int x = 0; x < 24; ++x) {
                int tX = x % 5, tY = x % 5;
                int newX = (2 * tX + 3 * tY) % 5;
                int newY = (tX + 2 * tY) % 5;
                uint64_t newTemp = block[newX][newY];
                block[newX][newY] = rotateLeft(temp, (x + 1) * (x + 2) / 2);
                temp = newTemp;
            }

            // Pi and Chi
            for (int j = 0; j < 5; ++j) {
                uint64_t tempArr[5];
                for (int x = 0; x < 5; ++x) {
                    tempArr[x] = block[x][j];
                }
                for (int x = 0; x < 5; ++x) {
                    block[x][j] = tempArr[x] ^ ((~tempArr[(x + 1) % 5]) & tempArr[(x + 2) % 5]);
                }
            }

            // Iota
            block[0][0] ^= (uint64_t(0x01) << j);
        }

        // Обновление состояния
        for (int x = 0; x < 5; ++x) {
            for (int y = 0; y < 5; ++y) {
                for (int z = 0; z < 64; ++z) {
                    state[x][y][z] ^= block[x][y];
                }
            }
        }
    }
}

const uint64_t RC[24] = {
    0x0000000000000001, 0x0000000000008082, 0x800000000000808A, 0x8000000080008000,
    0x000000000000808B, 0x0000000080000001, 0x8000000080008081, 0x8000000000008009,
    0x000000000000008A, 0x0000000000000088, 0x0000000080008009, 0x000000008000000A,
    0x000000008000808B, 0x800000000000008B, 0x8000000000008089, 0x8000000000008003,
    0x8000000000008002, 0x8000000000000080, 0x000000000000800A, 0x800000008000000A,
    0x8000000080008081, 0x8000000000008080, 0x0000000080000001, 0x8000000080008008
};

// ...для получения окончательного хеша
std::string squeeze(uint64_t state[5][5][64], int hashLength) {
    std::string hashResult;

    int stateBits = 1600; // Размер состояния SHA-3 в битах (1600)
    int bitsProcessed = 0;

    while (bitsProcessed < hashLength) {
        // Добавляем первые r бит состояния к хешу
        for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
        for (int k = 0; k < 64; ++k) {
            uint64_t cellValue = state[i][j][k];
            for (int b = 0; b < 64; ++b) {
                hashResult += ((cellValue >> (63 - b)) & 1) ? '1' : '0';
                ++bitsProcessed;
                if (bitsProcessed >= hashLength) {
                    break;
                }
            }
            if (bitsProcessed >= hashLength) {
                break;
            }
        }
        if (bitsProcessed >= hashLength) {
            break;
        }
    }
    if (bitsProcessed >= hashLength) {
        break;
    }
}

        // Применяем функцию перестановок
        for (int round = 0; round < 24; ++round) {
            // Функция Theta
            uint64_t C[5], D[5];
            for (int x = 0; x < 5; ++x) {
                C[x] = state[x][0][0] ^ state[x][1][0] ^ state[x][2][0] ^ state[x][3][0] ^ state[x][4][0];
            }
            for (int x = 0; x < 5; ++x) {
                D[x] = C[(x + 4) % 5] ^ rotateLeft(C[(x + 1) % 5], 1);
            }
            for (int x = 0; x < 5; ++x) {
                for (int y = 0; y < 5; ++y) {
                    state[x][y][0] ^= D[x];
                }
            }

            // Функция Rho и Pi
            uint64_t temp = state[1][0][0];
            for (int i = 0; i < 24; ++i) {
                int newX = (2 * i + 3 * i * i) % 5;
                int newY = (i + 2 * i * i) % 5;
                uint64_t newTemp = state[newX][newY][0];
                state[newX][newY][0] = rotateLeft(temp, (i + 1) * (i + 2) / 2);
                temp = newTemp;
            }

            // Функция Chi
            for (int y = 0; y < 5; ++y) {
                for (int x = 0; x < 5; ++x) {
                    uint64_t temp[5];
                    for (int z = 0; z < 5; ++z) {
                        temp[z] = state[(x + 1) % 5][y][z] ^ ((~state[(x + 2) % 5][y][z]) & state[(x + 3) % 5][y][z]);
                    }
                    for (int z = 0; z < 5; ++z) {
                        state[x][y][z] = temp[z];
                    }
                }
            }

            // Функция Iota
            state[0][0][0] ^= RC[round];
        }

        // Обрезаем хеш до требуемой длины
        if (bitsProcessed > hashLength) {
            hashResult = hashResult.substr(0, hashLength);
            break;
        }
    }

    return hashResult;
}


std::string binaryToHex(const std::string& binaryString) {
    std::bitset<4> bits;
    std::string hexResult;
    
    for (size_t i = 0; i < binaryString.length(); i += 4) {
        bits = std::bitset<4>(binaryString.substr(i, 4)); // Получаем четыре бита
        int decimalValue = bits.to_ulong(); // Переводим в десятичное значение
        char hexChar = (decimalValue < 10) ? (char)(decimalValue + '0') : (char)(decimalValue - 10 + 'A'); // Преобразуем в символы '0'-'9' или 'A'-'F'
        hexResult += hexChar;
    }
    
    return hexResult;
}

std::string SHA_3_256_ByPapple(std::string message){
    int hashLength = 256; // Длина хеша для SHA-3-256)

    int rate = 1600 / 8; // Размер блока (rate) для SHA-3-256 в байтах
    std::string paddedMessage = padMessage(message, rate); // Дополненное сообщение

    uint64_t state[5][5][64] = {0}; // Начальное состояние

    absorb(state, paddedMessage); // Вызов функции впитывания
    std::string hash = squeeze(state, hashLength); // Получение хеша

    hash = binaryToHex(hash); //перевод из двоичного в 16-ный код
    return hash; 
}

