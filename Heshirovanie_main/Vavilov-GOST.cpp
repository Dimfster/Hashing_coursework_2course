#include <iostream>
#include <iomanip>
#include <cstring>

typedef uint32_t word;

const int BLOCK_SIZE = 64;

void gost_3411_94_compress(word* h, const word* m) {
    // ���������� ������ ����� (�� ���������)

    // ... ��� ��� ������ �����

}

void gost_3411_94_hash(const char* message, size_t length, word* hash) {
    // ������������� ��������� ��������
    word h[8] = { 0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210,
                 0xf0e1d2c3, 0x4a5b6c7d, 0xc8d9e0f1, 0x01234567 };

    // ���������� ��������������� ���� 1
    char* padded_message = new char[length + 1];
    memcpy(padded_message, message, length);
    padded_message[length] = 0x01;

    // ���������� �����, ���� ����� ����� �� ������ ������ 448 �� ������ 512
    size_t padding_length = (448 - (length + 8) % 512 + 512) % 512;
    memset(padded_message + length + 1, 0, padding_length / 8);

    // ���������� ����� ��������� � ����� � ���� 64-������� �����
    size_t total_length_bits = length * 8;
    memcpy(padded_message + length + 1 + padding_length / 8, &total_length_bits, sizeof(size_t));

    // ��������� ������
    for (size_t i = 0; i < length + 1 + padding_length / 8 + 8; i += 64) {
        word block[16];
        memcpy(block, padded_message + i, BLOCK_SIZE);
        gost_3411_94_compress(h, block);
    }

    // ������������ ����
    memcpy(hash, h, sizeof(h));

    delete[] padded_message;
}
