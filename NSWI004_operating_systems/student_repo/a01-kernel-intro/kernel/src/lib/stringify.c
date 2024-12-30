
#include <lib/string.h>
#include <lib/stringify.h>
#include <types.h>

static char digits_upper[] = "0123456789ABCDEF";
static char digits_lower[] = "0123456789abcdef";

int stringify_hex(long number, char* buffer, size_t buffersize, bool upper, bool remove_zeroes) {
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
    if (leading_zeroes == 8)
        leading_zeroes--;
    if (remove_zeroes)
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

/**
 * @brief print number in decimal base
 *
 * @param number to print
 * @param buffer to print to
 * @param buffersize size of buffer
 * @return 0 if success
 */
int stringify_uint(unsigned int number, char* buffer, size_t buffersize) {

    if (buffersize <= 1)
        return 1;

    if (number == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return 0;
    }

    char* buffer_ptr = buffer;
    size_t used_buffer = 0;
    while (number != 0) {

        *buffer_ptr = '0' + (number % 10);
        number /= 10;
        if (++used_buffer >= buffersize) {
            return 1;
        }
        buffer_ptr++;
    }
    *buffer_ptr = '\0';
    strrev(buffer);
    return 0;
}