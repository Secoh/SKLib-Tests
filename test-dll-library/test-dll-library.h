// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     DLL import/export, shared DLL header
//
// Written by Secoh, 2022 // https://github.com/Secoh/SKLib-Tests
//
// This is free and unencumbered software released into the public domain.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// For more information, please refer to: http://unlicense.org/
//

// comment or uncomment the interface option to modify DLL behavior

//#define SKLIB_DLL_DISABLE_RUNTIME_CHECK
#define SKLIB_DLL_ENABLE_INSTANCE_DATA_SEGMENT
//#define SKLIB_DLL_ENABLE_THROW

#include <SKLib/include/dll.hpp>

SKLIB_DLL_INTERFACE(example_type)
{
    // Change between //* and /* to enable/disable TLS-like Data Segment
    //*
    SKLIB_DLL_DATA_SEGMENT
    {
        int a = 5;
        double fff = 3.14;
    };  // */

    SKLIB_DLL_CLASS_IMPLEMENT;

    SKLIB_DLL_FUNC(int, add, int, int);
    SKLIB_DLL_FUNC(int, coco);

};

