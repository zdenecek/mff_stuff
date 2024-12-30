

#include <lib/string.h>
#include <types.h>

/**
 * @brief compares two strings
 *
 * @return true if strings match
 * @return false else
 */
bool strcmp(const char* one, const char* two) {
    while (*one++ != '\0') {
        if (*++two != *one)
            return false;
    }
    return true;
}

/**
 * @brief calculates string length
 * returns 0 if str is NULL
 * @param str string
 * @return int string length
 */
int strlen(const char* str) {
    int len = 0;
    if (str == NULL)
        return 0;
    while (*str++ != 0) {
        len++;
    }
    return len;
}

/**
 * @brief Reverses a string in place
 * Implementation from https://stackoverflow.com/questions/8534274/is-the-strrev-function-not-available-in-linux/8534275#8534275
 * @param str string to reverse
 * @return char* reversed string
 */
char* strrev(char* str) {
    char *p1, *p2;

    if (!str || !*str)
        return str;
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}