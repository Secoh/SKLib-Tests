// Thie purpose of this software is to provide example how to use certain functions of SKLib:
//     bitwise::rank
//     bitwise::flip
//
// Written by Secoh, 2021
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

#include <iostream>
#include <bitset>
#include <cstdio>

#include <SKLib/bitwise.hpp>

int main()
{
    const auto a8 = sklib::bitwise::high_half_bits<uint8_t>();
    const auto a16 = sklib::bitwise::high_half_bits<uint16_t>();
    const auto a32 = sklib::bitwise::high_half_bits<uint32_t>();
    const auto a64 = sklib::bitwise::high_half_bits<uint64_t>();

    std::cout << std::bitset<sklib::bitwise::BYTE_BITS>(a8) << "\n"
        << std::bitset<sklib::bitwise::INT16_BITS>(a16) << "\n"
        << std::bitset<sklib::bitwise::INT32_BITS>(a32) << "\n"
        << std::bitset<sklib::bitwise::INT64_BITS>(a64) << "\n";

    uint8_t TAB[sklib::bitwise::BYTE_ADDRESS_SPAN];

    sklib::bitwise::generate_table_flip(TAB);

    std::cout << "\nTable Flip\n\n";

    for (int k = 0, j = 0; j < 16; j++)
    {
        for (int i = 0; i < 16; i++) printf(" 0x%02X,", TAB[k++]);
        printf("\n");
    }

    sklib::bitwise::generate_table_rank(TAB);

    std::cout << "\nTable Rank\n\n";

    for (int k = 0, j = 0; j < 16; j++)
    {
        for (int i = 0; i < 16; i++) printf(" 0x%02X,", TAB[k++]);
        printf("\n");
    }

    return 0;
}

