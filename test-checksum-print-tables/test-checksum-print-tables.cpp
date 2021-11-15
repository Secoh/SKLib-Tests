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
    if (argn < 2)
    {
        std::cout << "Calculates CCITT CRC16 from the first argument\n";
        return -1;
    }

    char *str = argc[1];
    sklib::crc16_ccitt R;
    while (*str) R.add(*str++);

    std::cout << std::hex << R.get() << "\n";

    return 0;
}

