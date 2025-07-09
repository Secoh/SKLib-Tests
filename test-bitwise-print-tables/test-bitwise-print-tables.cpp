// Thie purpose of this software is to provide example how to use certain functions of SKLib:
//     bitwise::rank
//     bitwise::flip
//     bitwise::distance
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

#define SKLIB_TARGET_TEST
#include <SKLib/sklib.hpp>

int main()
{
    const auto a8 = sklib::bits_high_half_v<uint8_t>;
    const auto a16 = sklib::bits_high_half_v<uint16_t>;
    const auto a32 = sklib::bits_high_half_v<uint32_t>;
    const auto a64 = sklib::bits_high_half_v<uint64_t>;

    std::cout << std::bitset<sklib::OCTET_BITS>(a8) << "\n"
        << std::bitset<sklib::bits_width_v<int16_t>>(a16) << "\n"
        << std::bitset<sklib::bits_width_v<int32_t>>(a32) << "\n"
        << std::bitset<sklib::bits_width_v<int64_t>>(a64) << "\n";

    // Flip, Rank, Distance, Base64

    sklib::aux::table256_print("\nTable Flip\n", sklib::aux::bits_flip_get_table(), 16, true);
    sklib::aux::table256_print("\nTable Rank\n", sklib::aux::bits_rank_get_table(), 32);
    sklib::aux::table256_print("\nTable Distance\n", sklib::aux::bits_distance_get_table(), 32);
    sklib::aux::table256_print("\nTable Base64 Inverse\n", sklib::base64_type::get_inverse_table(), 16, true);

    return 0;
}

