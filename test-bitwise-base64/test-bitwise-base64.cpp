// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     bitwise::base_64
//
// Written by Secoh, 2021 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

// test-bitwise-base64.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <chrono>

#define SKLIB_TARGET_TEST
#include <SKLib/SKLib.hpp>

std::string STOR;    // we keep here all input from command line parameters, for simplicity
size_t Stor_idx = 0;
std::chrono::steady_clock::time_point Will_read = std::chrono::steady_clock::now();

bool RD(sklib::base64_type*, int& data)
{
    using namespace std::chrono_literals;

    if (Will_read > std::chrono::steady_clock::now()) return false;  // next symbol "didn't arrive"

    Will_read = std::chrono::steady_clock::now() + std::chrono::steady_clock::duration(200ms);

    data = (Stor_idx < STOR.length() ? STOR[Stor_idx++] : EOF);
    return true;
}

void WR(sklib::base64_type*, int data)
{
    if (data < 0) std::cout << "<EOL>\n";
    else std::cout << (char)data;
}

int main(int argn, char *argc[])
{
    if (argn < 2)
    {
        std::cout << "base64.exe {e|d}{r|w}|g [string {string...}]\n"
                     "  e - encode function\n"
                     "  d - decode\n"
                     "  r - \"read\" - tester is reading from encoder; parameters are treated as input file\n"
                     "  w = \"write\" - tester sends all parameters to encoder in loop undil end\n"
                     "  g - generate table\n";
        return -1;
    }

    std::string par = argc[1];
    bool encode = (par.find('e') != std::wstring::npos);
    bool decode = (par.find('d') != std::wstring::npos);
    bool do_read = (par.find('r') != std::wstring::npos);
    bool do_write = (par.find('w') != std::wstring::npos);
    bool get_table = (par.find('g') != std::wstring::npos);

    if (get_table)
    {
        sklib::supplement::table256_print("\nBase64 Inverse Table\n", sklib::base64_type::get_inverse_table(), 16, true);
        return 0;
    }

    if ((int)encode + (int)decode != 1)
    {
        std::cout << "Specify either encode or decode, but not combinations\n";
        return -1;
    }
    if ((int)do_read + (int)do_write != 1)
    {
        std::cout << "Specify either encode or decode, but not combinations\n";
        return -1;
    }

    for (int k = 2; k < argn; k++) STOR += std::string(STOR.length() ? " " : "") + argc[k];

    sklib::base64_type Conv(RD, WR);

    if (encode)
    {
        if (do_read)
        {
            while (true)
            {
                int data;
                if (Conv.read_encode(data))
                {
                    std::cout << (char)data;
                    if (data == Conv.EOL_char) break;
                }
            }
        }
        if (do_write)
        {
            for (size_t k = 0; k < STOR.length(); k++) Conv.write_encode(STOR[k]);
            Conv.write_encode(EOF);
        }
    }
    if (decode)
    {
        if (do_read)
        {
            while (true)
            {
                int data;
                if (Conv.read_decode(data))
                {
                    if (data < 0) break;
                    std::cout << (char)data;
                }
            }
        }
        if (do_write)
        {
            for (size_t k = 0; k < STOR.length(); k++) Conv.write_decode(STOR[k]);
            if (STOR.length() && STOR[STOR.length()-1] != Conv.EOL_char) Conv.write_decode(Conv.EOL_char);
        }

    }

    return 0;
}

