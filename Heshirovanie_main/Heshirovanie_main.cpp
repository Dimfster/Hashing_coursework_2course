#define _CRT_SECURE_NO_WARNINGS
#include "Algorithm.h"
// Слово хешируем слово "Bitcoin";

using namespace std;
using namespace chrono;


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

int main()
{
    //1. MD-5; Байрамова Ксения
    Run_Algoritm(MD5, "MD5");

    //2. SHA-1 Марченко Владлен
    Run_Algoritm(sha1, "SHA-1");

    //3. SHA-2(256) Зимин Дмитрий
    Run_Algoritm(sha256, "SHA-2(256)");

    //4. SHA-2(512) Теняева Варвара
    Run_Algoritm(sha512, "SHA-2(512)");

    //5. SHA-3 Павел Павлов
    Run_Algoritm(SHA_3_256_ByPapple, "SHA-3(256)");

    //6. ГОСТ 34.11-94 Глеб Вавилов
    Run_Algoritm(gostHash, "GOST 34.11-94");

    //7. CRC-32 Клюев 
    //Run_Algoritm(crc32, "CRC-32");
}
