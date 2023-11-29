#include <iostream>
#include <string>
#include <bitset>
#include <cstdint>

using namespace std;


std::string binary_ToHex(const std::string& binaryString) {
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

string crc32(std::string input) {
    const uint32_t polynomial = 0xEDB88320u;
    uint32_t crc = 0xFFFFFFFFu;

    for (char c : input) {
        crc ^= static_cast<uint32_t>(c);

        for (int i = 0; i < 8; ++i) {
            crc = (crc >> 1) ^ ((crc & 1) ? polynomial : 0);
        }
    }
    int res = crc ^ 0xFFFFFFFFu;
    return binary_ToHex(to_string(res));
}