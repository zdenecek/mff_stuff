#include "casem.hpp"
#include <sstream>
#include <string>
#include <limits>

namespace casem
{

    bool character = false;
    std::stringstream ss;
    int char_value;
    int char_len;

    void string_start()
    {
        character = false;
        ss.str("");
    }

    void string_append(char c)
    {
        ss << c;
    }

    std::string string_end()
    {
        std::string s = ss.str();
        return s;
    }

    void char_start()
    {
        character = true;
        char_value = 0;
        char_len = 0;
    }

    void char_append(char c)
    {
        char_value <<= 8;
        char_value |= c;
        char_len++;
    }

    bool is_char_too_long()
    {
        return char_len > 4;
    }

    int char_end()
    {
        return char_value;
    }

    int get_digit_value(char c, bool hex, bool &failed)
    {
        failed = false;
        if ('0' <= c && c <= '9')
            return c - '0';
        c = std::tolower(c);
        if ('a' <= c && c <= 'f' && hex)
            return c - 'a' + 10;
        failed = true;
        return 0;
    }

    int parse_int(std::string text, int base, bool &malformed, bool &too_long)
    {
        int value = 0;
        int add;

        too_long = false;

        for (char c : text)
        {
            add = get_digit_value(c, base == 16, malformed);
            if (malformed)
                break;

            if (value > INT_MAX / base)
                too_long = true;
            value *= base;

            if (value > INT_MAX - add)
                too_long = true;
            value += add;
        }

        return value;
    }

    int parse_int(std::string text, int base)
    {
        int value = 0;
        int add;
        bool failed;

        for (char c : text)
        {
            add = get_digit_value(c, base == 16, failed);
            if (failed)
                break;
            value *= base;
            value += add;
        }
        return value;
    }

    void append(char c)
    {
        if (character)
            char_append(c);
        else
            string_append(c);
    }

    void append_escaped_special(char c)
    {
        switch (c)
        {
        case 'a':
            append('\b');
            break;
        case 'b':
            append('\b');
            break;
        case 'f':
            append('\f');
            break;
        case 'n':
            append('\n');
            break;
        case 'r':
            append('\r');
            break;
        case 't':
            append('\t');
            break;
        case 'v':
            append('\v');
            break;
        default:
            break;
        }
    }
    void append_escaped_hex(std::string c)
    {
        append(parse_int(c, 16));
    }

}
