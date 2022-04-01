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

//SK (!!) This file is WIP

// test-crypto-page-bijective.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<type_traits>
#include "SKLib/sklib.hpp"


        // in help lines: "\a" replaced by the Prefix letter; "\b" replaced by the parameter name (self)

// Designators: shortcuts used in help-related strings to indicate current option prefix and the option name in question

#define PARAM_DESG_PREFIX_S "\a"
#define PARAM_DESG_SELF_S   "\b"

//

template<unsigned U>
struct param_help_type
{
    const char* head = nullptr;
    const char* text[U] = { nullptr };

    constexpr param_help_type() = default;

    constexpr param_help_type(const char* hd, const ::sklib::supplement::collection_cstring_type<U>& ld)
        : head(hd)
    {
        ld.fill_array(text);
    }
};

struct param_help_receiver_type
{
    const char* head = nullptr;
    const char* const* text = nullptr;
    unsigned size = 0;

    template<unsigned U>
    constexpr param_help_receiver_type(const param_help_type<U>& ld)
        : head(ld.head)
        , text((U&& ld.text&& ld.text[0]) ? ld.text : nullptr)
        , size((U && ld.text && ld.text[0]) ? U : 0)
    {}
};


class params_table_base_type
{
private:
#ifdef _WIN32
    static void stdout_putc(char c) { putchar(c); }
    static constexpr auto def_putc = stdout_putc;
#else
    static constexpr auto def_putc = nullptr;
#endif

public:
    const char Prefix = '-';

    // help display

    static constexpr int def_min_help_spacing = 3;  // between names column and "required" column
    const int min_help_spacing = def_min_help_spacing;

    // tweaks in constructor

    params_table_base_type() = default;
    params_table_base_type(char option_prefix, int custom_help_spacing = def_min_help_spacing) : Prefix(option_prefix), min_help_spacing(custom_help_spacing) {}

    // global parser status after reading input

    unsigned parse_status = 0;
    static constexpr unsigned status_good       = 0x0001;  // indicates that parser has no problems
    static constexpr unsigned status_read       = 0x0002;  // parser has parameter(s) that are read (aka not empty), even with errors
    static constexpr unsigned status_help       = 0x0004;  // help is requested
    static constexpr unsigned status_errors     = 0x0008;  // parser has errors; other parse_status bits and/or status of specific parameter(s) may contain more details
    static constexpr unsigned status_incomplete = 0x0010;  // some of (or all) required parameters are not present
    static constexpr unsigned status_unknown    = 0x0020;  // error: unrecognized (or duplicate) parameter(s)
    static constexpr unsigned status_overflow   = 0x0040;  // error: too many plain parameters

    // per-parameter read status

    static constexpr uint8_t param_no_errors     = 0x00;   // option state is correct (even if not present)
    static constexpr uint8_t param_error_empty   = 0x01;   // option is present but data portion is not
    static constexpr uint8_t param_error_partial = 0x02;   // data portion shall be read in full but it was not
    static constexpr uint8_t param_is_help       = 0x80;   // hack: this bit signals that the parameter is used in help subsystem

    // parameters list

public:
    class param_switch;
    class param_help;

protected:
    param_switch* param_list_entry = nullptr;
    const param_help* param_help_entry = nullptr;

    // parameters descriptors
    //
    // all parameters have fields:
    //  - present     true if command line contains this parameter
    //  - status      result of reading this individual parameter
    //  - decode()    protected function that is used to read and interpret the value - advances pointer and sets error status
    //
    // all parameters except param_switch have:
    //  - value       the value associated with this parameter

public:
    class param_switch  // also, base type for all other parameters
    {
        friend params_table_base_type;

    public:
        bool present = false;
        uint8_t status = param_no_errors;

    protected:
        const char* const name = "";
        const unsigned name_len = 0;
        const bool required = false;
        const param_help_receiver_type descr;
        param_switch* const next = nullptr;

        static constexpr auto help_null = param_help_type<1>();
        static constexpr auto help_help = param_help_type("Prints help on options. Use \"" PARAM_DESG_PREFIX_S PARAM_DESG_SELF_S " " PARAM_DESG_SELF_S "\" for more.",
                              ::sklib::collection_cstring(PARAM_DESG_PREFIX_S PARAM_DESG_SELF_S "             prints the list of available options",
                                                          PARAM_DESG_PREFIX_S PARAM_DESG_SELF_S " [" PARAM_DESG_PREFIX_S "]option   prints the help text on the selected option (if available)"));
    public:
        constexpr param_switch(params_table_base_type* root,
                               const char* param_name,
                               bool param_required = false)
            : param_switch(root, param_name, param_required, help_null)
        {}

        template<unsigned U>
        constexpr param_switch(params_table_base_type* root,
                               const char* param_name,
                               bool param_required,
                               const param_help_type<U>& param_descr)
            : name(param_name)
            , name_len((unsigned)::sklib::strlen(param_name))
            , required(param_required)
            , descr(param_descr)
            , next(root->param_list_entry)
        {
            root->param_list_entry = this;
        }

    protected:
        static constexpr int     global_defval_int       = int();
        static constexpr int64_t global_defval_int64     = int64_t();
        static constexpr double  global_defval_double    = double();
        static constexpr char    global_defval_key       = '\0';
        static constexpr char    global_defval_cstring[] = "";

        // read and accept the value of the parameter, escept for switch
        // derived classes for other parameter types shall override this fuction
        virtual const char* decode(const char* arg)
        {
            return nullptr;  // special case, no value for switch
        }
    };

    class param_int : public param_switch
    {
    protected:
        const char* decode(const char* arg)
        {
            char* pstop;
            value = strtol(arg, &pstop, 10);
            return pstop;
        }

    public:
        int value = global_defval_int;

        constexpr param_int(params_table_base_type* root,
                            const char* param_name,
                            bool param_required = false,
                            int defval = global_defval_int)
            : param_int(root, param_name, param_required, defval, help_null)
        {}

        template<unsigned U>
        constexpr param_int(params_table_base_type* root,
                            const char* param_name,
                            bool param_required,
                            int defval,
                            const param_help_type<U>& param_descr)
            : param_switch(root, param_name, param_required, param_descr)
            , value(defval)
        {}
    };

    class param_int64 : public param_switch
    {
    protected:
        const char* decode(const char* arg)
        {
            char* pstop;
            value = strtoll(arg, &pstop, 10);
            return pstop;
        }

    public:
        int64_t value = global_defval_int64;

        constexpr param_int64(params_table_base_type* root,
                              const char* param_name,
                              bool param_required = false,
                              int64_t defval = global_defval_int64)
            : param_int64(root, param_name, param_required, defval, help_null)
        {}

        template<unsigned U>
        constexpr param_int64(params_table_base_type* root,
                              const char* param_name,
                              bool param_required,
                              int64_t defval,
                              const param_help_type<U>& param_descr)
            : param_switch(root, param_name, param_required, param_descr)
            , value(defval)
        {}
    };

    class param_double : public param_switch
    {
    protected:
        const char* decode(const char* arg)
        {
            char* pstop;
            value = strtod(arg, &pstop);
            return pstop;
        }

    public:
        double value = global_defval_double;

        constexpr param_double(params_table_base_type* root,
                               const char* param_name,
                               bool param_required = false,
                               double defval = global_defval_double)
            : param_double(root, param_name, param_required, defval, help_null)
        {}

        template<unsigned U>
        constexpr param_double(params_table_base_type* root,
                               const char* param_name,
                               bool param_required,
                               double defval,
                               const param_help_type<U>& param_descr)
            : param_switch(root, param_name, param_required, param_descr)
            , value(defval)
        {}
    };

    class param_key : public param_switch
    {
    protected:
        const char* decode(const char* arg)
        {
            value = *arg;
            if (value) arg++;
            return arg;
        }

    public:
        char value = global_defval_key;

        constexpr param_key(params_table_base_type* root,
                            const char* param_name,
                            bool param_required = false,
                            char defval = global_defval_key)
            : param_key(root, param_name, param_required, defval, help_null)
        {}

        template<unsigned U>
        constexpr param_key(params_table_base_type* root,
                            const char* param_name,
                            bool param_required,
                            char defval,
                            const param_help_type<U>& param_descr)
            : param_switch(root, param_name, param_required, param_descr)
            , value(defval)
        {}
    };

    class param_str : public param_switch
    {
    protected:
        const char* decode(const char* arg)
        {
            value = arg;
            return arg + ::sklib::strlen(arg);
        }

    public:
        const char* value = global_defval_cstring;

        constexpr param_str(params_table_base_type* root,
                            const char* param_name,
                            bool param_required = false,
                            const char* defval = global_defval_cstring)
            : param_str(root, param_name, param_required, defval, help_null)
        {}

        template<unsigned U>
        constexpr param_str(params_table_base_type* root,
                            const char* param_name,
                            bool param_required,
                            const char* defval,
                            const param_help_type<U>& param_descr)
            : param_switch(root, param_name, param_required, param_descr)
            , value(defval)
        {}
    };

    class param_help : public param_str
    {
        friend params_table_base_type;

    protected:
        params_table_base_type* const parent = nullptr;

        typedef sklib::internal::c_callback_type<void, char> help_putc_callback_type;
        const help_putc_callback_type putc_callback;

        void write(char c, const char* name = "") const
        {
            if (c == PARAM_DESG_SELF_S[0])
            {
                this->write(name);    // potential recursion breaks at step 3, because the substitution string will be empty
            }
            else
            {
                putc_callback(c == PARAM_DESG_PREFIX_S[0] ? parent->Prefix : c);
            }
        }
        void write(const char* str, const char* name = "") const
        {
            if (str && putc_callback) while(*str) write(*str++, name);
        }
        void writeln(const char* str, const char* name = "") const
        {
            write(str, name);
            write('\n');
        }

        // problem: parent class doesn't know where is the help class located - it only has L1 list of param_switch base classes
        // (extreme case - multiple help-bound parameters were programmed, one was called - which one?)
        // lets exploit the fact that virtual decode() is called for each parameter encountered in the line
        const char* decode(const char* arg)
        {
            parent->param_help_entry = this;
            return param_str::decode(arg);
        }

    public:
        constexpr param_help(params_table_base_type* root,
                             const char* param_name,
                             const help_putc_callback_type& print_line_func = help_putc_callback_type{ stdout_putc })
            : param_help(root, param_name, help_help, print_line_func)
        {}

        template<unsigned U>
        constexpr param_help(params_table_base_type* root,
                             const char* param_name,
                             const param_help_type<U>& param_descr,
                             const help_putc_callback_type& print_line_func = help_putc_callback_type{ stdout_putc })
            : param_str(root, param_name, false, global_defval_cstring, param_descr)
            , parent(root)
            , putc_callback(print_line_func)
        {
            status |= param_is_help;
        }
    };

    // command line parser, uses parameter parser

protected:
    // find the param_switch entry in the L1 list
    // criteria for selection: most distant, name match, and depth is strictly less than the given depth
    // if name is a null, any name is a match; if depth limit is negative, any depth is a match
    // returns pointer to the element or null if not found;
    // returns the current depth of the found element or negative if not found
    std::pair<const param_switch*, int> find_named_param(const char* name, int depth_cap = -1) const
    {
        int depth = 0;
        int depth_seen = -1;
        param_switch* ptr_select = nullptr;
        for (param_switch* ptr = param_list_entry; ptr && (depth_cap < 0 || depth < depth_cap); depth++, ptr = ptr->next)
        {
            if (ptr->name_len && (!name || ::sklib::strequ(name, ptr->name)))   // must have name, *and* either take any, or the name match
            {
                depth_seen = depth;
                ptr_select = ptr;
            }
        }
        return { ptr_select, depth_seen };
    }

    // match the parameter name to opt argument (which is past prefix, and may be in the middle of original string)
    // function selects the longest match that wasn't already taken
    // if multiple matches are possible, selects the first in the declaration order
    //
    // the first entry must not be empty, checked by caller
    // the second entry is normally the next command line parameter, unless it is logically start of another option or if argument array ends prematurely
    // the second entry may be non-existent, in such case caller shall supply "" string; caller guarantees that argument is never nullptr
    //
    // if accepted, update state of the associated object, read value, set presense, errors, etc
    // return: 0 - parameter not read (string doesn't match option name), state of the object didn't change
    // <positive> - length in characters that is read from the input, parser still reads first string in pair (but may be did read to the end)
    // <negative> - detached parameter is seen: first string is read to the end, and the second string is the value, also taken
    //              if the second parameter cannot be parsed to the end, the rest of it is ignored and error state for this parameter is set
    int apply_named_param(const char* opt, const char* next)
    {
        param_switch* select = nullptr;
        for (param_switch* ptr = param_list_entry; ptr; ptr = ptr->next)
        {
            if (ptr->name_len && !ptr->present &&                   // named param, wasn't read
                (!select || ptr->name_len >= select->name_len) &&   // search for longest match
                ::sklib::strnequ(opt, ptr->name, ptr->name_len))
            {
                select = ptr;
            }
        }
        if (!select) return 0;   // didn't find

        select->present = true;

        unsigned len = select->name_len;
        bool same_argument = (::sklib::strlen(opt) > len);
        auto value_start = (same_argument ? opt+len : next);
        auto value_end = select->decode(value_start);

        if (!value_end) return len;    // it was switch

        if (same_argument)
        {
            if (value_start == value_end) select->status |= param_error_empty;   // if value is not read
            return (int)(value_end - opt);
        }

        if (value_end - value_start != ::sklib::strlen(next)) select->status |= param_error_partial;  // only the body of the value must be in the string, otherwise it is error
        return -1;
    }

    // enter unnamed (plain) parameter; string shall be parsed as the entire body
    // parameters are filled up in the declaration order
    // returns true if something is taken, false if the fist is full
    bool apply_plain_param(const char* opt)
    {
        param_switch* select = nullptr;
        for (param_switch* ptr = param_list_entry; ptr; ptr = ptr->next)
        {
            if (!ptr->name_len && !ptr->present) select = ptr;
        }
        if (!select) return false;   // didn't find

        select->present = true;

        auto value_end = select->decode(opt);
        if (!value_end || value_end - opt != ::sklib::strlen(opt)) select->status |= param_error_partial;  // only the body of the value must be in the string, otherwise it is error

        return true;
    }

    // help-related

    void help_diagnistics(const char* str, const char* name) const
    {
        param_help_entry->writeln(str);
        param_help_entry->writeln("  " PARAM_DESG_PREFIX_S PARAM_DESG_SELF_S, name);
    }
    void help_prhead(const char* name, int spacer, bool required, const char* head) const
    {
        param_help_entry->write(Prefix);
        param_help_entry->write(name);
        for (int i=0;i<spacer; i++) param_help_entry->write(" ");
        param_help_entry->write(required ? '*' : ' ');
        param_help_entry->write(' ');
        param_help_entry->writeln(head, name);
    }
    void help_show_for_one_param(const param_switch* subject_ptr) const
    {
        auto& subject_help = subject_ptr->descr;
        if (subject_help.size && subject_help.text)
        {
            for (unsigned k=0; k<subject_help.size; k++) param_help_entry->writeln(subject_help.text[k], subject_ptr->name);
        }
        else if (subject_help.head)
        {
            help_prhead(subject_ptr->name, min_help_spacing, subject_ptr->required, subject_help.head);
        }
        else
        {
            help_diagnistics("This option doesn\'t have help text:", subject_ptr->name);
        }
    }

public:
    // fill next argument which is not an option    // bruteforce, sorry
    // returns true if argument is used for an option entry (option may still have error code in the process)
    // false if command line has more parameters than the program can accept

    // parse the command line arguments into the parameter set (defined in the class)
    // - named option can be represented in 2 ways:
    //   1) -a##b##... all switches, keys and numbers in the same string without separation (only logical separation)
    //   2) -a ### - option in one command line argument, its value (assuming the option has a value) in the next argument (detached parameter)
    // - plain (not named) option is 

    bool parse(int argn, const char* const* argc, int arg_start = 1)
    {
        // note that this function is intended for single call, but lets clear this variable anyway
        parse_status = 0;

        bool signal_plain = false;  // if true, next arument is interpreted as plain parameter ("GNU style")

        for (int k = arg_start; k < argn; k++)
        {
            auto arg_cur = argc[k];
            if (!arg_cur || !*arg_cur) continue;  // this cannot happen

            if (*arg_cur == Prefix && !signal_plain)
            {
                unsigned arg_len = (unsigned)::sklib::strlen(arg_cur);
                for (unsigned t = 1; t < arg_len; t++)
                {
                    if (arg_cur[t] == Prefix)
                    {
                        signal_plain = true;    // second prefix supresses next parameter: -- -string in command line => "-string" becomes plain parameter
                        continue;
                    }

                    // examine placement of prefix and arguments
                    // 0) if parameter value appears in the same argument with parameter name: -d6 => d is name, 6 is value
                    // 1) normal way to give parameter: -d 6 => when string ends after "-d", "6" is expected in the next argument
                    // 2) value is empty/skipped, if next argument is another parameter: -d -x => parameter d has empty value
                    // 3) value starting with "-" (prefix) can be forced by double-prefix: -d -- -x => "-x" becomes value of d
                    // 4) for completeness, tested in other place, use double-prefix outside parameter context to force next argument to be plain parameter: -- -d => "-d" is entered as plain parameter

                    auto kval = k + 1;
                    auto arg_val = (kval < argn ? argc[kval] : nullptr);

                    if (!arg_val)
                    {
                        arg_val = "";   // empty
                    }
                    else if (arg_val[0] != Prefix)
                    {
                        ;   // keep current arg_val, even if empty
                    }
                    else if (arg_val[1] != Prefix)    // before check, arg_val = "-..."
                    {
                        arg_val = "";   // next element is parameter, no value
                    }
                    else if (arg_val[2] == '\0')    // arg_val = "--"
                    {
                        arg_val = (++kval < argn ? argc[kval] : "");    // use second extension as value, if present
                    }
                    else    // arg_val = "--something"
                    {
                        arg_val = "";   // for completeness, one shall not use that
                    }

                    auto ds = apply_named_param(arg_cur + t, arg_val);
                    if (ds > 0)
                    {
                        t += ds - 1;
                    }
                    else if (ds < 0)
                    {
                        t = arg_len - 1;
                        if (*arg_val) k = kval;   // if there is a command line entry(ies) that we need to skip // if empty, we DON'T need to skip the next argument
                    }
                    else // ds == 0
                    {
                        parse_status |= status_unknown;
                    }
                }
            }
            else
            {
                if (!apply_plain_param(arg_cur)) parse_status |= status_overflow;
                signal_plain = false;
            }
        }

        bool event_present = false;
        bool event_help = false;

        for (param_switch* ptr = param_list_entry; ptr; ptr = ptr->next)
        {
            if (ptr->present)
            {
                event_present = true;
                if (ptr->status & param_is_help) event_help = true;
                if (ptr->status & ~param_is_help) parse_status |= status_errors;
            }
            else
            {
                if (ptr->required) parse_status |= status_incomplete;
            }
        }

        parse_status |= (parse_status ? status_errors : status_good);   // force "have errors" bit, or "good" bit if no errors

        if (event_present) parse_status |= status_read;
        if (event_help) parse_status |= status_help;

        return (parse_status & (status_good | status_help));
    }

    bool show_help() const
    {
        if (!param_help_entry) return false;

        if (param_help_entry->value && *param_help_entry->value)
        {
            auto subject_ptr = find_named_param(param_help_entry->value).first;
            if (!subject_ptr)
            {
                help_diagnistics("This option doesn\'t exist:", param_help_entry->value);
                return false;
            }

            help_show_for_one_param(subject_ptr);
            return true;
        }

        // else, if another option is entered, print specifc help and exit
        // keep track of maximum name length for the next operation

        unsigned max_name_length = 0;
        int depth = -1;
        while (true)
        {
            auto entry = find_named_param(nullptr, depth);
            depth = entry.second;

            auto subject_ptr = entry.first;
            if (!subject_ptr) break;

            max_name_length = std::max(max_name_length, subject_ptr->name_len);

            if (subject_ptr->present && !(subject_ptr->status & param_is_help))
            {
                help_show_for_one_param(subject_ptr);
                return true;
            }
        }

        // else, help alone - print list of all options

        depth = -1;
        while (true)
        {
            auto entry = find_named_param(nullptr, depth);
            depth = entry.second;

            auto subject_ptr = entry.first;
            if (!subject_ptr) break;

            int spacing = min_help_spacing + max_name_length - subject_ptr->name_len;
            help_prhead(subject_ptr->name, spacing, subject_ptr->required, (subject_ptr ? subject_ptr->descr.head : nullptr));
        }

        return true;
    }
};




#define DECLARE_CMD_PARAMS(name) struct name : public params_table_base_type


#define PARAM_SWITCH(x,...)     param_switch x{ this, #x, __VA_ARGS__ };
#define PARAM_INT(x,...)     param_int x{ this, #x , __VA_ARGS__ };
#define PARAM_KEY(x,...)     param_key x{ this, #x , __VA_ARGS__ };
#define PARAM_STRING(x,...)     param_str x{ this, #x , __VA_ARGS__ };

#define PARAM_PLAIN_INT(x,...)     param_int x{ this, "", __VA_ARGS__ };    // __VA_OPT__(,)
#define PARAM_PLAIN_KEY(x,...)     param_key x{ this, "", __VA_ARGS__ };
#define PARAM_PLAIN_STRING(x,...)     param_str x{ this, "", __VA_ARGS__ };

//#define PARAM_HELP(keyword)  param_help keyword{ this, #keyword };  //    <-- need special type with processing

// name, [ req, [ def, [ help ]]]

// PARAM_* - required
// OPT_* - optional

/*
DECLARE_CMD_PARAMS(test_list)
{
//    PARAM_HELP(help);

    PARAM_SWITCH( a );
    PARAM_SWITCH(b);
    PARAM_SWITCH(ur);

    PARAM_KEY(z, '0');
    PARAM_INT(za);              // was: (za, 7)
    PARAM_STRING(src, "src");

    PARAM_PLAIN_STRING(r1, "aaa");
    PARAM_PLAIN_STRING(r2, "bbb");
    PARAM_PLAIN_INT(d1, 987);
};
*/

//IMPLEMENT_CMD_PARAMS(test_list) PPP;

struct test_list : public params_table_base_type
{
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
}
PPP;


int main(int argn, char *argc[])
{
    std::cout << (PPP.parse(argn,argc), PPP.parse_status) << "; A=" << PPP.a.present << "; B=" << PPP.b.present << "; UR=" << PPP.ur.present << "\n";
    std::cout << "z=" << PPP.z.present << "," << PPP.z.value << "; za=" << PPP.za.present << "," << PPP.za.value << "; src=" << PPP.src.present << "," << PPP.src.value << "\n";
    std::cout << "p1=" << PPP.r1.present << "," << PPP.r1.value << "; p2=" << PPP.r2.present << "," << PPP.r2.value << "; p3=" << PPP.d1.present << "," << PPP.d1.value << "\n";

    if (PPP.parse_status & PPP.status_help)
    {
        std::cout << "hlp>\n";
//        PPP.hh.show();
        PPP.show_help();
    }
}



