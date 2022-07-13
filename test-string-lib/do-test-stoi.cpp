// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     string::stoi
//
// Written by Secoh, 2022 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

#include "do-test-stoi.h"

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

    std::cout << (int)v1 << ", " << p1 << "\n"
              << (int)uv1 << ", " << up1 << "\n"
              << (int)v2 << ", " << p2 << "\n"
              << (int)uv2 << ", " << up2 << "\n"
              << v4 << ", " << p4 << "\n"
              << uv4 << ", " << up4 << "\n"
              << v8 << ", " << p8 << "\n"
              << uv8 << ", " << up8 << "\n"
              << vd << ", " << pd << "\n";
}

void test_strcmp(const char* str)
{
    const auto zzz = sklib::strancmp(L"vvvc", "vVv", 3);
    const auto xxx = sklib::strlen(L"abcd");
    const auto yyy = sklib::strabeg(L"vvvc", "vVv");

    const auto sss = sklib::stod<double>("0xabpe");
    const auto uuu = sklib::stod<long double>("31.25e-3");

    std::cout << sklib::strlen(str) << ", "
              << sklib::straequ(str, L"aBcD") << ", "
              << sklib::strcmp(str, L"aBcD") << "\n";
}

