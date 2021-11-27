// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     checksum::crc_type; crc_fixed_type
//
// Written by Secoh, 2021 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

#include <iostream>

#include <SKLib/checksum.hpp>

int main()
{
    // example of constant expression calculations

    const auto X = sklib::crc_16_ccitt().update("ABCD");
    const auto Y = sklib::crc_16_ccitt().update_bruteforce("ABCD");
    const auto U = sklib::crc_fixed_type<uint8_t, 4, 0x3, true>().update("RAWR");
    const auto V = sklib::crc_fixed_type<uint8_t, 4, 0x3, true>().update_bruteforce("RAWR");
    std::cout << X << "," << Y << "," << (int)U << "," << (int)V << "\n\n";

    // the tables are now calculated on the fly at compile time, without preloading in the source code
    // tables are stored in constant area of program data (aka "ROM")

    sklib::supplement::table256_print("CRC16 LSB", sklib::crc_16_ccitt::get_table(), 8, true, 4);
    sklib::supplement::table256_print("\nCRC16 MSB", sklib::crc_16_ccitt_msb::get_table(), 8, true, 4);
    sklib::supplement::table256_print("\nCRC8", sklib::crc_8_ccitt::get_table(), 16, true, 2);
    sklib::supplement::table256_print("\nCRC32", sklib::crc_32_iso::get_table(), 8, true, 8);
    sklib::supplement::table256_print("\nCRC64", sklib::crc_64_iso::get_table(), 4, true, 16);

    return 0;
}

