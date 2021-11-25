// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     checksum::crc16_ccitt
//
// Written by Secoh, 2021 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

#include <iostream>

#include <SKLib/checksum.hpp>

int main(int argn, char *argc[])
{
    const auto X = sklib::crc_16_ccitt().update("ABCD");
    const auto Y = sklib::crc_21_can_lsb().update("XYZ");
    const auto Z = sklib::crc_16_ccitt_msb().update("Lorem Upsum Dolor");

    const auto X1 = sklib::crc_16_ccitt().update_bruteforce("ABCD");
    const auto Y1 = sklib::crc_21_can_lsb().update_bruteforce("XYZ");
    const auto Z1 = sklib::crc_16_ccitt_msb().update_bruteforce("Lorem Upsum Dolor");

    const auto W = sklib::crc_16_ccitt().update(argn>1 ? argc[1] : "");
    const auto V = sklib::crc_3_gsm().update("ZZRT");

    const auto U = sklib::crc_3_gsm().update(argn > 1 ? argc[1] : "");

    const auto P1 = sklib::crc_8_ccitt().update("RAWR");
    const auto P2 = sklib::crc_8_ccitt().update_bruteforce("RAWR");

    const auto P3 = sklib::crc_8_ccitt_msb().update("RAWR");
    const auto P4 = sklib::crc_8_ccitt_msb().update_bruteforce("RAWR");

    const auto R1 = sklib::crc_fixed_type<uint8_t, 4, 0x3, true>().update("ZZX1111");
    const auto R2 = sklib::crc_fixed_type<uint8_t, 4, 0x3, true>().update_bruteforce("ZZX1111");

    const auto Q1 = sklib::crc_fixed_type<uint8_t, 4, 0x3, false>().update("ZZXA");
    const auto Q2 = sklib::crc_fixed_type<uint8_t, 4, 0x3, false>().update_bruteforce("ZZXA");


//    const auto xx1 = sklib::crc_16_ccitt::MSB;

/*
    const auto a = sklib::crc_21_can::high_bit;
    const auto b = sklib::crc_21_can::mask;
    const auto c = sklib::crc_21_can::polynom;
    const auto d = sklib::crc_21_can::polynom_flip;
    const auto e = sklib::crc_21_can::length;

    uint16_t TAB[sklib::OCTET_ADDRESS_SPAN];
    sklib::crc_generate_table_msb<sklib::crc16_ccitt_type>(TAB);
    sklib::supplement::table256_print("CRC16 CCITT", TAB, 8, true, 4);
*/

    sklib::supplement::table256_print("CRC16", sklib::crc_16_ccitt::get_table(), 8, true, 4);
    sklib::supplement::table256_print("CRC16 MSB", sklib::crc_16_ccitt_msb::get_table(), 8, true, 4);
    sklib::supplement::table256_print("CRC8", sklib::crc_8_ccitt::get_table(), 8, true, 2);
    sklib::supplement::table256_print("CRC32", sklib::crc_32_iso::get_table(), 8, true, 8);

    sklib::supplement::table256_print("Flip", sklib::supplement::bits_flip_get_table(), 8, true, 2);

    std::cout << std::hex << X << ", " << Y << ", " << Z << "," << W << "," << (int)V << "," << (int)U << "," << (int)R1 << "\n"; //  ", " << a << ", " << b << ", " << c << ", " << d << ", " << e << "\n";

    if (argn < 2)
    {
        std::cout << "Calculates CCITT CRC16 from the first argument\n";
        return -1;
    }

    char *str = argc[1];
//    sklib::crc16_ccitt R;
//    while (*str) R.add(*str++);

//    std::cout << std::hex << R.get() << "\n";

    return 0;
}

