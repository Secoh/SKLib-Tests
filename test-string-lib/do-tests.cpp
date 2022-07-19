// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     string library
//
// Written by Secoh, 2022 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

#include "do-tests.h"

//#include <SKLib/sklib.hpp>
#include <SKLib/include/string.hpp>

#include <iostream>

void test_stoi(const char* str)
{
    int p1, p2, p4, p8, up1, up2, up4, up8, pd;
    auto v1 = sklib::stoi<char>(str, &p1);
    auto uv1 = sklib::stoi<unsigned char>(str, &up1);
    auto v2 = sklib::stoi<int16_t>(str, &p2);
    auto uv2 = sklib::stoi<uint16_t>(str, &up2);
    auto v4 = sklib::stoi<int32_t>(str, &p4);
    auto uv4 = sklib::stoi<uint16_t>(str, &up4);
    auto v8 = sklib::stoi<int64_t>(str, &p8);
    auto uv8 = sklib::stoi<uint64_t>(str, &up8);
    auto vd = sklib::stoi<double>(str, &pd);

    int16_t p_stod;
    auto v_stod = sklib::stod<double>(str, &p_stod);

    std::cout << "char: " << (int)v1 << ", " << p1 << "\n"
              << "uchar: " << (int)uv1 << ", " << up1 << "\n"
              << "int16: " << (int)v2 << ", " << p2 << "\n"
              << "uint16: " << (int)uv2 << ", " << up2 << "\n"
              << "int32: " << v4 << ", " << p4 << "\n"
              << "uint32: " << uv4 << ", " << up4 << "\n"
              << "int64: " << v8 << ", " << p8 << "\n"
              << "uint64: " << uv8 << ", " << up8 << "\n"
              << "stoi/f: " << vd << ", " << pd << "\n"
              << "stod: " << v_stod << ", " << p_stod << "\n";
}

void test_strcmp(const char* str)
{
    const auto zzz = sklib::strancmp(L"vvvc", "vVv", 3);
    const auto xxx = sklib::strlen(L"abcd");
    const auto yyy = sklib::strabeg(L"vvvc", "vVv");

    const auto sss = sklib::stod<double>("0xabpe");
    const auto uuu = sklib::stod<long double>("31.25e-3");

    std::cout << "strlen:" << sklib::strlen(str) << ", "
              << "straequ/L_aBcD:" << sklib::straequ(str, L"aBcD") << ", "
              << "strcmp/L_aBcD:" << sklib::strcmp(str, L"aBcD") << ", "
              << "strancmp/U_aBcD/3:" << sklib::strancmp(str, U"aBcD", 3) << "\n";
}

