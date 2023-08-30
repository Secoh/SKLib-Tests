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
#include <cmath>


#define SKLIB_DLL_EXPORT
#include "test-dll-library.h"


// internal shortcuts, to keep it valid for both enabled/disabled data segnemt
#ifdef SKLIB_DLL_ENABLE_INSTANCE_DATA_SEGMENT
#define IMPL_CARRY (INSTANCE_DATA->carry)
#else
#define IMPL_CARRY 0
#endif

SKLIB_DLL_FUNC(int, add, int a, int b)
{
    auto c = IMPL_CARRY;
    IMPL_CARRY = 0;
    return a + b + c;
}

SKLIB_DLL_FUNC(int, sub, int a, int b)
{
    auto c = IMPL_CARRY;
    IMPL_CARRY = 0;
    return a - b - c;
}


// Same idea, without shortcut

#ifdef SKLIB_DLL_ENABLE_INSTANCE_DATA_SEGMENT
SKLIB_DLL_FUNC_OPT(int, coco)
{
    return std::lround(100 / INSTANCE_DATA->fcoco);
}
#else
SKLIB_DLL_FUNC_OPT(int, coco)
{
    return -1;
}
#endif


// Functions below are supported only if data segment is present.
// Normally, the user of the dll library shall select the operating mode, and adhere to it at all times.

#ifdef SKLIB_DLL_ENABLE_INSTANCE_DATA_SEGMENT
SKLIB_DLL_FUNC_OPT(void, put, const char* str)
{
    INSTANCE_DATA->payload = str;
}

SKLIB_DLL_FUNC_OPT(const char*, get)
{
    return INSTANCE_DATA->payload.c_str();  // const pointer has extended lifetime ?!
}
#endif

