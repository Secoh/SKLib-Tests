// Thie purpose of this software is to debug certain functions of SKLib, and to provide example how to use them:
//     DLL import/export, shared DLL header, options
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


// just in case
#include <string>

struct dll_sample_type;
template<class T>
class gvar
{
    friend dll_sample_type;

private:
    T value{};

public:
    T& operator() () { return value; }
    T& operator= (const T& x) { return (value = x), value; }
};

SKLIB_DLL_INTERFACE(dll_sample_type)
{
    // Change between //* and /* to enable/disable TLS-like Data Segment
    //*
    SKLIB_DLL_DATA_SEGMENT
    {
        int carry = 0;
        double fcoco = 3.141593;
        std::string payload;
    };  // */

    SKLIB_DLL_CLASS_IMPLEMENT;

    SKLIB_DLL_FUNC(int, add, int, int);
    SKLIB_DLL_FUNC(int, sub, int, int);

    SKLIB_DLL_FUNC_OPT(int, coco);

    SKLIB_DLL_FUNC_OPT(void, put, const char*);
    SKLIB_DLL_FUNC_OPT(const char*, get);

    gvar<int> iTest;
    gvar<double> dTest;
    gvar<std::string> sTest;

};

