#ifndef casem_hpp_
#define casem_hpp_

#include "cktables.hpp"
#include "ckcontext.hpp"
#include "ckgrptokens.hpp"
#include <string>

namespace casem
{

    void string_start();
    std::string string_end();

    void char_start();
    int char_end();
    bool is_char_too_long();

    void append(char c);
    void append_escaped_special(char c);
    void append_escaped_hex(std::string c);

    int parse_int(std::string text, int base, bool &malformed, bool &too_long);

}

#endif
