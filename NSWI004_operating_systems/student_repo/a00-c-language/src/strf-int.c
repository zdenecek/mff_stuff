
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

// Trivial, copied from https://stackoverflow.com/questions/8534274/is-the-strrev-function-not-available-in-linux/8534275#8534275
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

static char digits_upper[] = "0123456789ABCDEF";
static char digits_lower[] = "0123456789abcdef";

/**
 * @brief print long in hex base
 *
 * @param number to print
 * @param buffer to print to
 * @param buffersize size of buffer
 * @param upper use upper case letters
 * @return 0 if success
 */
int stringify_hex_long(long number, char* buffer, size_t buffersize, bool upper) {

    if (buffersize <= 1)
        return 1;

    char* digits = upper ? digits_upper : digits_lower;
    if (digits == NULL)
        return 1;

    // important to be unsigned, otherwise the shift will be signed on some platforms
    unsigned long unsigned_number = number;
    unsigned long mask = 0xF;
    int shift = 0;
    char* buffer_ptr = buffer;
    size_t used_buffer = 0;
    int leading_zeroes = 0;

    while (mask != 0) {
        int digit = (unsigned_number & mask) >> shift;
        if (digit == 0)
            leading_zeroes++;
        else
            leading_zeroes = 0;

        *buffer_ptr = *(digits + digit);
        if (++used_buffer >= buffersize) {
            return 1;
        }
        buffer_ptr++;
        shift += 4;
        mask <<= 4;
    }
    *(buffer_ptr - leading_zeroes) = '\0';

    strrev(buffer);
    return 0;
}

/**
 * @brief print number in hex base
 *
 * @param number to print
 * @param buffer to print to
 * @param buffersize size of buffer
 * @param upper use upper case letters
 * @return 0 if success
 */
int stringify_hex(int number, char* buffer, size_t buffersize, bool upper) {

    if (buffersize <= 1)
        return 1;

    char* digits = upper ? digits_upper : digits_lower;
    if (digits == NULL)
        return 1;

    // important to be unsigned, otherwise the shift will be signed on some platforms
    unsigned int unsigned_number = number;
    unsigned int mask = 0xF;
    int shift = 0;
    char* buffer_ptr = buffer;
    size_t used_buffer = 0;
    int leading_zeroes = 0;

    while (mask != 0) {
        int digit = (unsigned_number & mask) >> shift;
        if (digit == 0)
            leading_zeroes++;
        else
            leading_zeroes = 0;

        *buffer_ptr = *(digits + digit);
        if (++used_buffer >= buffersize) {
            return 1;
        }
        buffer_ptr++;
        shift += 4;
        mask <<= 4;
    }
    *(buffer_ptr - leading_zeroes) = '\0';

    strrev(buffer);
    return 0;
}

/**
 * @brief print number in decimal base
 *
 * @param number to print
 * @param buffer to print to
 * @param buffersize size of buffer
 * @return 0 if success
 */
int stringify_int(int number, char* buffer, size_t buffersize) {

    if (buffersize <= 1)
        return 1;

    if (number == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return 0;
    }

    long long_number = (long)number;

    if (number < 0) {
        long_number = -long_number;
        *buffer = '-';
        buffer++;
        buffersize--;

        if (buffersize == 1)
            return 1;
    }

    char* buffer_ptr = buffer;
    size_t used_buffer = 0;
    while (long_number != 0) {

        *buffer_ptr = '0' + (long_number % 10);
        long_number /= 10;
        if (++used_buffer >= buffersize) {
            return 1;
        }
        buffer_ptr++;
    }
    *buffer_ptr = '\0';
    strrev(buffer);
    return 0;
}