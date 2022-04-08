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
#include <bitset>

#include <SKLib/cmdpar.hpp>


SKLIB_DECLARE_OPTION_HELP(help_b, "demo B", "Lorem", "ipsum", "dolor");

SKLIB_DECLARE_OPTION_HELP(help_p1, "input");
SKLIB_DECLARE_OPTION_HELP(help_p3, "number");

SKLIB_DECLARE_HELP_BANNER(ppp_banner, "test [-a][-b][-ur] -z<char> -za<char> arg1 arg2 [num1] | -h", "\a\a cancels special");

SKLIB_DECLARE_CMD_PARAMS(test_list)
{
    SKLIB_OPTION_SWITCH(a);
    SKLIB_OPTION_SWITCH(b, help_b);
    SKLIB_OPTION_SWITCH(ur);

    SKLIB_PARAM_KEY(z, '0');
    SKLIB_PARAM_KEY(za);
    SKLIB_OPTION_STRING(src, "defsrc");

    SKLIB_PLAIN_STRING(r1, "aaa", help_p1);
    SKLIB_PLAIN_STRING(r2, "bbb");
    SKLIB_PLAIN_OPT_INT(d1, 987, help_p3);

    SKLIB_OPTION_HELP(h);

//    SKLIB_PARAMS_ALT_PREFIX('?');       // <-- uncomment to change Prefix
    SKLIB_PARAMS_ALT_BANNER(ppp_banner);
}
PPP;


int main(int argn, char* argc[])
{
    PPP.parse(argn, argc);

    std::cout << std::bitset<16>(PPP.parse_status) << "\n";

    std::cout << "A = " << PPP.a.present << "; B = " << PPP.b.present << "; UR = " << PPP.ur.present << "\n";
    std::cout << "z=" << PPP.z.present << "," << PPP.z.value << "; za=" << PPP.za.present << "," << PPP.za.value << "; src=" << PPP.src.present << "," << PPP.src.value << "\n";
    std::cout << "p1=" << PPP.r1.present << "," << PPP.r1.value << "; p2=" << PPP.r2.present << "," << PPP.r2.value << "; p3=" << PPP.d1.present << "," << PPP.d1.value << "\n";

    if (PPP.is_help_requested())
    {
        PPP.show_help('#');
    }
    else if (PPP.is_good())
    {
        std::cout << "Parse OK\n";
    }
}


