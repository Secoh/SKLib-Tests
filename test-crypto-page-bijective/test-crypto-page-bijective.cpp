// This file is part of SKLib: https://github.com/Secoh/SKLib
// Copyright [2020-2021] Secoh
//
// Licensed under the GNU Lesser General Public License, Version 2.1 or later. See: https://www.gnu.org/licenses/
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// Special exception from GNU LGPL terms: you don't have to publish the compiled object binary file(s) for SKLib.
// Modified source code and/or any derivative work requirements are still in effect. All such file(s) must be openly
// published under the same terms as the original one(s), but you don't have to inherit the special exception above.
//


#include <iostream>
#include <SKLib/cmdpar.hpp>

/*
    param_switch a{ this, "a", true };
    param_switch b{ this, "b" };
    param_switch ur{ this, "ur" };

    param_key z{ this, "z", true, '0' };
    param_key za{ this, "za" };
    param_str src{ this, "src", false, "src" };

    param_str r1{ this, "", true, "aaa" };
    param_str r2{ this, "", true, "bbb" };
    param_int d1{ this, "", false, 987 };

    param_help hh{ this, "h" };
*/

SKLIB_DECLARE_OPTION_HELP(help_b, "demo B", "Lorem", "ipsum", "dolor");

SKLIB_DECLARE_OPTION_HELP(help_p1, "input");
SKLIB_DECLARE_OPTION_HELP(help_p2, "output");

SKLIB_DECLARE_CMD_PARAMS(test_list)
{

    SKLIB_OPTION_SWITCH(a);
    SKLIB_OPTION_SWITCH(b, help_b);
    SKLIB_OPTION_SWITCH(ur);

    SKLIB_PARAM_KEY(z, '0');
    SKLIB_PARAM_KEY(za);
    SKLIB_OPTION_STRING(src, "defsrc");

    SKLIB_PLAIN_STRING(r1, "aaa", help_p1);
    SKLIB_PLAIN_STRING(r2, "bbb", help_p2);
    SKLIB_PLAIN_OPT_INT(d1, 987);

    SKLIB_OPTION_HELP(h);
}
PPP;


int main(int argn, char *argc[])
{
    std::cout << (PPP.parse(argn,argc), PPP.parse_status) << "; A=" << PPP.a.present << "; B=" << PPP.b.present << "; UR=" << PPP.ur.present << "\n";
    std::cout << "z=" << PPP.z.present << "," << PPP.z.value << "; za=" << PPP.za.present << "," << PPP.za.value << "; src=" << PPP.src.present << "," << PPP.src.value << "\n";
    std::cout << "p1=" << PPP.r1.present << "," << PPP.r1.value << "; p2=" << PPP.r2.present << "," << PPP.r2.value << "; p3=" << PPP.d1.present << "," << PPP.d1.value << "\n";

    if (PPP.parse_status & PPP.status_help)
    {
        PPP.show_help();
    }
}



