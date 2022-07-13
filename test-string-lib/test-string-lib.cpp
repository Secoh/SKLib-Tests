// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     string library
//
// Written by Secoh, 2022 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

#include <iostream>
#include "do-test-stoi.h"

int main(int argn, const char *argc[])
{
    std::cout << "Hello World!\n";

    if (argn > 1)
    {
        test_stoi(argc[1]);
        test_strcmp(argc[1]);
    }
    else
    {
        std::cout << argc[0] << " number\n";
    }

    return 0;
}

