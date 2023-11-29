// Heshirovanie_main.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include "Algorithm.h"
// Слово хешируем слово "Bitcoin";


template <typename InputIterator>
std::uint_fast32_t crc(InputIterator first, InputIterator last);



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
