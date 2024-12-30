#!/usr/bin/env python3

# SPDX-License-Identifier: Apache-2.0
# Copyright 2019 Charles University


import sys

PREFIX_BLOCK_EXPECTED = '[EXPECTED BLOCK]: '
PREFIX_EXPECTED = '[EXPECTED]: '
PREFIX_ACTUAL = '[ ACTUAL ]: '

def print_error(fmt, *args, **kwargs):
    print(fmt.format(*args, **kwargs), file=sys.stderr)

def main():
    expected_block = []
    expected_block_line = 0
    expected_block_frozen = False

    expected = None
    expected_line = 0

    line_number = 0
    ignored_line_count = 0
    exit_code = 0

    for line in sys.stdin:
        line = line.rstrip()
        line_number = line_number + 1
        if line.startswith(PREFIX_BLOCK_EXPECTED):
            if expected_block_frozen:
                print_error('Actual block on line {} ended too early on {} ("{}" not matched).',
                            expected_block_line, line_number - 1, expected_block[0])
                expected_block = []
                exit_code = 1
            if not expected_block:
                expected_block_line = line_number
            expected_block.append(line[len(PREFIX_BLOCK_EXPECTED):])
            expected_block_frozen = False
            continue
        else:
            if expected_block:
                expected_block_frozen = True

        if expected_block:
            if line != expected_block[0]:
                print_error('Mismatch on lines {} and {} ("{}" != "{}")',
                            expected_block_line, line_number, expected_block[0], line)
                exit_code = 1
            expected_block = expected_block[1:]
            expected_block_line = expected_block_line + 1
            if not expected_block:
                expected_block_frozen = False
            continue

        if line.startswith(PREFIX_EXPECTED):
            if not expected is None:
                print_error('Missing actual value for expected on line {} ({}).',
                            expected_line, expected)
                exit_code = 1
            expected = line[len(PREFIX_EXPECTED):]
            expected_line = line_number
        elif line.startswith(PREFIX_ACTUAL):
            actual = line[len(PREFIX_ACTUAL):]
            if expected is None:
                print_error('Missing expected value for actual on line {} ({}',
                            line_number, actual)
                exit_code = 1
                continue
            if actual != expected:
                print_error('Mismatch on lines {} and {} ("{}" != "{}")',
                            expected_line, line_number, expected, actual)
                exit_code = 1
            expected = None
        else:
            # Skip other lines
            ignored_line_count = ignored_line_count + 1

    if expected_block:
        print_error('Not enough actual lines for expected block at {} ("{}" not mached).',
                    expected_block_line, expected_block[0])

    if ignored_line_count == line_number:
        print_error('Warning: no lines compared, runaway test?')
        exit_code = 1

    return exit_code


if __name__ == "__main__":
    sys.exit(main())
