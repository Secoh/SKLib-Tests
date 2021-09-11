// Thie purpose of this software is to provide example how to use certain functions of SKLib:
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
#include <bitset>
#include <cstdio>

#include <SKLib/bitwise.hpp>

int main()
{
    const auto a8 = sklib::supplement::bits_data_high_half<uint8_t>();
    const auto a16 = sklib::supplement::bits_data_high_half<uint16_t>();
    const auto a32 = sklib::supplement::bits_data_high_half<uint32_t>();
    const auto a64 = sklib::supplement::bits_data_high_half<uint64_t>();

    std::cout << std::bitset<sklib::OCTET_BITS>(a8) << "\n"
        << std::bitset<sklib::supplement::bits_data_width<int16_t>()>(a16) << "\n"
        << std::bitset<sklib::supplement::bits_data_width<int32_t>()>(a32) << "\n"
        << std::bitset<sklib::supplement::bits_data_width<int64_t>()>(a64) << "\n";

    uint8_t TAB[sklib::OCTET_ADDRESS_SPAN];
    char TABC[sklib::OCTET_ADDRESS_SPAN];

    // Flip

    sklib::supplement::bits_flip_generate_table(TAB);

    std::cout << "\nTable Flip\n\n";

    for (int k = 0, j = 0; j < 16; j++)
    {
        for (int i = 0; i < 16; i++) printf(" 0x%02X,", TAB[k++]);
        printf("\n");
    }

    // Rank

    sklib::supplement::bits_rank_generate_table(TABC);

    std::cout << "\nTable Rank\n\n";

    for (int k = 0, j = 0; j < 8; j++)
    {
        for (int i = 0; i < 32; i++) printf(" %d,", (int)TABC[k++]);
        printf("\n");
    }

    // Distance

    sklib::supplement::bits_distance_generate_table(TABC);

    std::cout << "\nTable Distance\n\n";

    for (int k = 0, j = 0; j < 8; j++)
    {
        for (int i = 0; i < 32; i++) printf(" %d,", (int)TABC[k++]);
        printf("\n");
    }

    return 0;
}

