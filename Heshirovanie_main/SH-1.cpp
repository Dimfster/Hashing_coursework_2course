#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include <openssl/sha.h>

std::string sha1(const std::string& input) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(input.c_str()), input.length(), hash);

    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        ss << std::setw(2) << static_cast<unsigned>(hash[i]);
    }

    return ss.str();
}

int main() {
    std::string input = "Hello, world!";
    std::string hash = sha1(input);

    std::cout << "SHA-1 hash: " << hash << std::endl;

    return 0;
}
