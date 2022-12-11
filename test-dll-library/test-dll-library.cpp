// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     DLL export, shared DLL header
//
// Written by Secoh, 2022 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//


#include "pch.h"
#include <iostream>


#define SKLIB_DLL_EXPORT
#include "test-dll-library.h"


SKLIB_DLL_FUNC(int, add, int a, int b)
{
#ifdef SKLIB_DLL_ENABLE_INSTANCE_DATA_SEGMENT
    double x = INSTANCE_DATA->fff;
#else
    double x = 0;
#endif

    std::cout << "Dll called: add()\n";
    return a + b + (int)x;
}


SKLIB_DLL_FUNC(int, coco)
{
    return 333;
}

