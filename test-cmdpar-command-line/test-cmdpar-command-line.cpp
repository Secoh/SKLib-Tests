// This file is part of SKLib: https://github.com/Secoh/SKLib
// Copyright [2020-2022] Secoh
//
// Licensed under the GNU Lesser General Public License, Version 2.1 or later. See: https://www.gnu.org/licenses/
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// Special exception from GNU LGPL terms: you don't have to publish the compiled object binary file(s) for SKLib.
// Modified source code and/or any derivative work requirements are still in effect. All such file(s) must be openly
// published under the same terms as the original one(s), but you don't have to inherit the special exception above.
//

#include <iostream>

//#define SKLIB_CMDPAR_PARAM_PREFIX par_

#include <SKLib/include/cmdpar.hpp>

//  test [-a][-b][-ur] -z<char> -za<char> arg1 arg2 [num1]
SKLIB_DECLARE_CMD_PARAMS(test_list)
{
    SKLIB_OPTION_SWITCH(a);
    SKLIB_OPTION_SWITCH(b);
    SKLIB_OPTION_SWITCH(ur);

    SKLIB_PARAM_DOUBLE(f, 3.1416);
    SKLIB_OPTION_DOUBLE(fz, 2.71);
    SKLIB_PARAM_INT(i);
    SKLIB_PARAM_KEY(z, '0');
    SKLIB_PARAM_KEY(za);
    SKLIB_OPTION_STRING(src, "defsrc");

    SKLIB_PLAIN_STRING(r1, "aaa");
    SKLIB_PLAIN_STRING(r2, "bbb");

    SKLIB_PLAIN_INT_OPTIONAL(d1, 987);

//    SKLIB_OPTION_SWITCH_ALT_NAME(palka, L"палка");   // palka in Russian (English: stick)

    SKLIB_OPTION_HELP(h);

    SKLIB_CMD_PARAMS_ALT_PREFIX('?');       // <-- uncomment to change Prefix

    SKLIB_CMD_PARAMS_INITIALIZER(test_list);
}
PPP;

std::wstring str_AtoW(const std::string& str)
{
    std::wstring wstr;
    for (auto c : str) wstr.push_back((wchar_t)c);
    return wstr;
}

typedef ::sklib::implementation::cmdpar_param_base<char> param_base;

void show_option_status(const char* tstr, const param_base& param)
{
    typedef ::sklib::cmdpar_option_flags option_flag;    // "using" equivalent

    std::cout << "> ";

    auto* s1 = strrchr(tstr, ':');
    if (s1) while (sklib::is_c_name_token(*++s1)) std::cout << *s1;
    else std::cout << tstr;

    if (!param.get_status())                             std::cout << "; No Options";
    if (param.get_status() & option_flag::required)      std::cout << "; Required";
    if (param.get_status() & option_flag::present)       std::cout << "; Present";
    if (param.get_status() & option_flag::is_help)       std::cout << "; Option is Help";
    if (param.get_status() & option_flag::help_request)  std::cout << "; Help Requested";
    if (param.get_status() & option_flag::error_empty)   std::cout << "; Error: Empty";
    if (param.get_status() & option_flag::error_partial) std::cout << "; Error: Partial";
    std::cout << "\n";
}

auto check_name(const char* name) { return (name && *name) ? name : "<plain>"; }
auto check_name(const wchar_t* name) { return (name && *name) ? name : L"<plain>"; }

template<class T>
void show_param(const param_base& param, const char* tstr, const char* name, const T& value)
{
    std::cout << check_name(name) << " = " << value << "  --";  // "\n"
    show_option_status(tstr, param);
}
template<class T>
void show_param(const param_base& param, const char* tstr, const wchar_t* name, const T value)
{
    std::wcout << check_name(name) << L" = " << value << L"  --";
    show_option_status(tstr, param);
}
template<>
void show_param(const param_base& param, const char* tstr, const wchar_t* name, const char* value)
{
    std::wcout << check_name(name) << L" = " << str_AtoW(value) << L"  --";
    show_option_status(tstr, param);
}

void show_parser_status(bool ok, const test_list& root)
{
    typedef ::sklib::cmdpar_parser_flags parser_flag;    // another "using"

    std::cout << (ok ? "Parse OK\n" : "Parse Bad\n");

    auto status = root->get_status();

    if (status & parser_flag::good)                 std::cout << "Params Valid\n";
    if (status & parser_flag::empty)                std::cout << "Empty Input\n";
    if (status & parser_flag::error)                std::cout << "Error State\n";
    if (status & parser_flag::error_unknown_name)   std::cout << "Error: Unknown Name\n";
    if (status & parser_flag::error_missing_named)  std::cout << "Error: Required Named Parameter Not Present\n";
    if (status & parser_flag::error_missing_plain)  std::cout << "Error: Required Plain Parameter Not Present\n";
    if (status & parser_flag::error_overflow_named) std::cout << "Error: Too Many Named Parameters (Repeats)\n";
    if (status & parser_flag::error_overflow_plain) std::cout << "Error: Too Many Plain Parameters\n";
    if (status & parser_flag::error_malformed)      std::cout << "Error: Malformed Parameter Value\n";
    if (status & parser_flag::help_request)         std::cout << "Help Requested\n";
    if (status & parser_flag::help_banner)          std::cout << "Help: Banner\n";
    if (status & parser_flag::help_option)          std::cout << "Help: On Parameter\n";

    if (status) std::cout << "\n";
}

int main(int argn, char* argc[])
{
//    bool is_ok = PPP(argn, argc);
    test_list PPP(argn, argc);
    bool is_ok = PPP->is_good();

    show_parser_status(is_ok, PPP);

#define SHOW_PARAM(what) show_param((what), typeid(what).name(), (what).name(), (what).value())
    SHOW_PARAM(PPP.a);
    SHOW_PARAM(PPP.b);
    SHOW_PARAM(PPP.ur);
    SHOW_PARAM(PPP.f);
    SHOW_PARAM(PPP.fz);
    SHOW_PARAM(PPP.i);
    SHOW_PARAM(PPP.z);
    SHOW_PARAM(PPP.za);
    SHOW_PARAM(PPP.src);
    SHOW_PARAM(PPP.h);
    SHOW_PARAM(PPP.r1);
    SHOW_PARAM(PPP.r2);
    SHOW_PARAM(PPP.d1);
//    SHOW_PARAM(PPP.palka);

    const auto pflag = PPP->get_status();     // another way to access parser flags

  //  const auto oflag = decltype(PPP)::param_uint<wchar_t>::param_base::option_flag::is_help;  // example of inheritance chain

    PPP->reset();

//bug: unicode

}


