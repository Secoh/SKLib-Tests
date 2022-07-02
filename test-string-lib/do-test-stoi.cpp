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

#include <SKLib/string.hpp>

#include <iostream>

void test_stoi()
{
    int p;
    int q = (int)stoi<double>(L"129=", &p);

    std::cout << q << ", " << p << "\n";
}

