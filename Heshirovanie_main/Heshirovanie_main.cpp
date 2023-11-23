﻿// Heshirovanie_main.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include "Algorithm.h"


#include <iostream>
#include <iomanip>
#include <cstring>
// Слово хешируем слово "Bitcoin";


void gostHash(std::string& input, uint8_t* output);


int main()
{
    //1. MD-5; Байрамова Ксения
   // Run_Algoritm(MD5, "MD5");

    //2. SHA-1 Марченко Владлен
    Run_Algoritm(sha1, "SHA-1");

    //3. SHA-2(256) Зимин Дмитрий
    Run_Algoritm(sha256, "SHA-2(256)");

    //3. SHA-2(512) Теняева Варвара
    Run_Algoritm(sha512, "SHA-2(512)");

    //4. SHA-3 Павел Павлов
    Run_Algoritm(SHA_3_256_ByPapple, "SHA-3(256)");

    //5. ГОСТ 34.11-94 Глеб Вавилов

    //6. BLAKE Клюев Сергей

    std::string input = "bitcoin";
    uint8_t hash[32];

    gostHash(input, hash);

    std::cout << "Input: " << input << std::endl;
    std::cout << "Hash: ";
    for (int i = 0; i < 32; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    std::cout << std::endl;

}
