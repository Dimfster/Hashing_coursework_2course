#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "sha2.h"


// MD-5 
std::string MD5(std::string data);

// SHA-1
std::string sha1(std::string input);

// SHA-2(256)
std::string sha256(std::string input);

// SHA-2(512)
std::string sha512(std::string input);

// SHA-3(256)
std::string SHA_3_256_ByPapple(std::string message);

// GOST 34.11-94
std::string gostHash(std::string input);

// CRC-32
std::string crc32(std::string input);

