#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "sha2.h"


using namespace std;
using namespace chrono;




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


using Hash = std::string(*)(std::string input);

void Run_Algoritm(Hash function, string name_method)
{
	cout << "Algorithm " << name_method << endl << endl;

	cout << "Determination:" << endl;
	// Детерминированность
	string hash_1 = function("Bitcoin");
	string hash_2 = function("Bitcoin");
	string hash_3 = function("Bitcoin");
	cout << "Hash: " << hash_1 << endl;
	cout << "Hash: " << hash_2 << endl;
	cout << "Hash: " << hash_3 << endl << endl;

	// Необратимость +

	// Уникальность +-

	cout << "Unpredictable:" << endl;
	// Непрогнозируемость 
	string hash_4 = function("Bitcoin");
	string hash_5 = function("Bicoin");
	string hash_6 = function("bitcoin");
	cout << "Hash: " << hash_4 << endl;
	cout << "Hash: " << hash_5 << endl;
	cout << "Hash: " << hash_6 << endl << endl;


	cout << "Speed:" << endl;

	auto start = system_clock::now();

	string hash = function("Bitcoin");

	auto end = system_clock::now();
	auto duration = duration_cast<microseconds>(end - start).count();

	cout << "Hash: " << hash << endl;

	cout << "Algorithm " << name_method << " worked in: " << duration << " ms" << endl << endl << endl << endl;
}