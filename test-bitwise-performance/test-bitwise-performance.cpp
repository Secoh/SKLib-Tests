// Thie purpose of this software is to benchmark certain functions of SKLib, and to provide example how to use them:
//     bitwise::rank
//     bitwise::flip
//
// Written by Secoh, 2021 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

#include <iostream>
#include <cstdio>
#include <ctime>
#include <chrono>

#include <SKLib/bitwise.hpp>

int main()
{
    // prepare

    size_t DLEN = sklib::supplement::bits_data_high_half<uint16_t>();
    uint8_t* DATA = new uint8_t[DLEN];
    srand((unsigned)time(nullptr));
    for (size_t i = 0; i < DLEN; i++) DATA[i] = (rand() % 0x100);

    // addressing data timing

    auto T_start = std::chrono::steady_clock::now();

    int64_t V = 0;
    for (size_t i = 0; i < DLEN; i++) V += DATA[i];

    auto T_finish = std::chrono::steady_clock::now();
    std::cout << "Length: " << DLEN << "\nAddress: " << V << "\n" << std::chrono::duration<double>(T_finish - T_start).count() * 1000 << " ms\n";

    // flip: 8 bit table

    T_start = std::chrono::steady_clock::now();

    V = 0;
    for (size_t i = 0; i < DLEN; i++) V += sklib::bits_flip<uint8_t>(DATA[i]);

    T_finish = std::chrono::steady_clock::now();
    std::cout << "Flip\nFast: " << V << "\n" << std::chrono::duration<double>(T_finish - T_start).count() * 1000 << " ms\n";

    // flip: 8 bit bruteforce

    T_start = std::chrono::steady_clock::now();

    V = 0;
    for (size_t i = 0; i < DLEN; i++) V += sklib::supplement::bits_flip_bruteforce<uint8_t>(DATA[i]);

    T_finish = std::chrono::steady_clock::now();
    std::cout << "Slow: " << V << "\n" << std::chrono::duration<double>(T_finish - T_start).count() * 1000 << " ms\n";

    // flip: 32 bit normal

    T_start = std::chrono::steady_clock::now();

    V = 0;
    for (size_t i = 0; i < DLEN; i++) V += sklib::supplement::bits_flip<4>(uint32_t(DATA[i]));

    T_finish = std::chrono::steady_clock::now();
    std::cout << "Fast: " << V << "\n" << std::chrono::duration<double>(T_finish - T_start).count() * 1000 << " ms\n";

    // flip: 32 bit bruteforce

    T_start = std::chrono::steady_clock::now();

    V = 0;
    for (size_t i = 0; i < DLEN; i++) V += sklib::supplement::bits_flip_bruteforce<uint32_t>(DATA[i]);

    T_finish = std::chrono::steady_clock::now();
    std::cout << "Slow: " << V << "\n" << std::chrono::duration<double>(T_finish - T_start).count() * 1000 << " ms\n";

    // rank: 8 bit table

    T_start = std::chrono::steady_clock::now();

    V = 0;
    for (size_t i = 0; i < DLEN; i++) V += sklib::bits_rank<uint8_t>(DATA[i]);

    T_finish = std::chrono::steady_clock::now();
    std::cout << "Rank\nFast: " << V << "\n" << std::chrono::duration<double>(T_finish - T_start).count() * 1000 << " ms\n";

    // rank: 8 bit bruteforce

    T_start = std::chrono::steady_clock::now();

    V = 0;
    for (size_t i = 0; i < DLEN; i++) V += sklib::supplement::bits_rank_bruteforce(DATA[i]);

    T_finish = std::chrono::steady_clock::now();
    std::cout << "Slow: " << V << "\n" << std::chrono::duration<double>(T_finish - T_start).count() * 1000 << " ms\n";

    // rank: 8 bit fork

    T_start = std::chrono::steady_clock::now();

    V = 0;
    for (size_t i = 0; i < DLEN; i++) V += sklib::supplement::bits_rank8_fork(DATA[i]);

    T_finish = std::chrono::steady_clock::now();
    std::cout << "Fork: " << V << "\n" << std::chrono::duration<double>(T_finish - T_start).count() * 1000 << " ms\n";

    // rank: 32 bit fast

    T_start = std::chrono::steady_clock::now();

    V = 0;
    for (size_t i = 0; i < DLEN; i++) V += sklib::bits_rank(uint32_t(DATA[i]) << 24);

    T_finish = std::chrono::steady_clock::now();
    std::cout << "Fast: " << V << "\n" << std::chrono::duration<double>(T_finish - T_start).count() * 1000 << " ms\n";

    // rank: 32 bit bruteforce

    T_start = std::chrono::steady_clock::now();

    V = 0;
    for (size_t i = 0; i < DLEN; i++) V += sklib::supplement::bits_rank_bruteforce(uint32_t(DATA[i]) << 24);

    T_finish = std::chrono::steady_clock::now();
    std::cout << "Slow: " << V << "\n" << std::chrono::duration<double>(T_finish - T_start).count() * 1000 << " ms\n";

    // distance: 8 bit table

    T_start = std::chrono::steady_clock::now();

    V = 0;
    for (size_t i = 0; i < DLEN; i++) V += sklib::bits_distance<uint8_t>(DATA[i]);

    T_finish = std::chrono::steady_clock::now();
    std::cout << "Distance\nFast: " << V << "\n" << std::chrono::duration<double>(T_finish - T_start).count() * 1000 << " ms\n";

    // distance: 8 bit bruteforce

    T_start = std::chrono::steady_clock::now();

    V = 0;
    for (size_t i = 0; i < DLEN; i++) V += sklib::supplement::bits_distance_bruteforce(DATA[i]);

    T_finish = std::chrono::steady_clock::now();
    std::cout << "Slow: " << V << "\n" << std::chrono::duration<double>(T_finish - T_start).count() * 1000 << " ms\n";

    // distance: 32 bit fast

    T_start = std::chrono::steady_clock::now();

    V = 0;
    for (size_t i = 0; i < DLEN; i++) V += sklib::bits_distance(uint32_t(DATA[i]) << 24);

    T_finish = std::chrono::steady_clock::now();
    std::cout << "Fast: " << V << "\n" << std::chrono::duration<double>(T_finish - T_start).count() * 1000 << " ms\n";

    // distance: 32 bit bruteforce

    T_start = std::chrono::steady_clock::now();

    V = 0;
    for (size_t i = 0; i < DLEN; i++) V += sklib::supplement::bits_distance_bruteforce(uint32_t(DATA[i]) << 24);

    T_finish = std::chrono::steady_clock::now();
    std::cout << "Slow: " << V << "\n" << std::chrono::duration<double>(T_finish - T_start).count() * 1000 << " ms\n";

    // cleanup

    delete[] DATA;
    return 0;
}

