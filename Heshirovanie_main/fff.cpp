#include <iostream>
#include <string>

using namespace std;

uint32_t crc32(std::string input) {
    const uint32_t polynomial = 0xEDB88320u;
    uint32_t crc = 0xFFFFFFFFu;

    for (char c : input) {
        crc ^= static_cast<uint32_t>(c);

        for (int i = 0; i < 8; ++i) {
            crc = (crc >> 1) ^ ((crc & 1) ? polynomial : 0);
        }
    }

    return crc ^ 0xFFFFFFFFu;
}

string CRC32_string(std::string input)
{
    auto hash = crc32(input);
    +
}

int main() {
    std::string input = "Hello, World!";
    uint32_t result = crc32(input);

    std::cout << "CRC32 хэш для строки '" << input << "': " << std::hex << result << std::endl;

    return 0;
}

