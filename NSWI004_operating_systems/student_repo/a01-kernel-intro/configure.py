#!/usr/bin/env python3

# SPDX-License-Identifier: Apache-2.0
# Copyright 2019 Charles University

import sys
import os
import argparse
import logging
import subprocess

KERNEL_TEST_EXTRAS = {
}

# Last one takes precedence
# (i.e. we prefer NSWI004 package over system installation)
TOOLCHAIN_SEARCH_DIRS = [
    '/usr/',
    '/opt/mff-nswi004/',
    os.path.expanduser("~/.local/"),
]

MSIM_VERSION_LINE_PREFIX = 'MSIM version '

CONFIG_MK_FMT = '''

TARGET = {toolchain_target}
TOOLCHAIN_DIR = {toolchain_dir}

CC = $(TOOLCHAIN_DIR)/bin/$(TARGET)-gcc
LD = $(TOOLCHAIN_DIR)/bin/$(TARGET)-ld
OBJCOPY = $(TOOLCHAIN_DIR)/bin/$(TARGET)-objcopy
OBJDUMP = $(TOOLCHAIN_DIR)/bin/$(TARGET)-objdump
GDB = $(TOOLCHAIN_DIR)/bin/$(TARGET)-gdb

KERNEL_TEST_SOURCES = {kernel_test_sources}
KERNEL_EXTRA_CFLAGS = {kernel_extra_cflags}
KERNEL_EXTRA_ASFLAGS = {kernel_extra_asflags}

'''

BUILD_DIR_MAKEFILE_FMT = '''

SRC_BASE = {src_base}/

vpath %.S $(SRC_BASE)
vpath %.c $(SRC_BASE)
vpath %.h $(SRC_BASE)

include $(SRC_BASE)/Makefile

'''

MSIM_CONF_FMT = '''

# Generated by configure.py

# Single processor machine
add dcpu cpu0

# Setup main memory for kernel
add rwm mainmem 0
mainmem generic {ram_size}K
mainmem load "kernel/kernel.bin"

# Setup memory for the loader
# (R4000 starts at hardwired address 0x1FC00000).
add rom loadermem 0x1FC00000
loadermem generic 4K
loadermem load "kernel/loader.bin"

# Console printer
add dprinter printer 0x10000000

'''

MIPSEL_TARGETS = ['mipsel-linux-gnu', 'mipsel-unknown-linux-gnu']

def copy_to_file(inp_fd, out_name, out_mode):
    with open(out_name, out_mode) as out:
        while True:
            buffer = inp_fd.read(512)
            if not buffer:
                break
            out.write(buffer)

def has_gcc_installed(prefix, target):
    gcc_path = os.path.join(prefix, 'bin', '{}-gcc'.format(target))
    return os.path.exists(gcc_path)

def get_msim_version():
    try:
        output = subprocess.check_output(
            ['msim', '--version'],
            stderr=subprocess.STDOUT
        ).decode('utf-8')
        for line in output.split("\n"):
            if not line.startswith(MSIM_VERSION_LINE_PREFIX):
                continue
            version_string = line[len(MSIM_VERSION_LINE_PREFIX):]
            nums = [int(i) for i in version_string.split(".")]
            # Unfortunately, MSIM before 1.4 used also patch versions
            if len(nums) == 3:
                nums.append(0)
            if len(nums) != 4:
                return None
            return nums
    except FileNotFoundError:
        return None
    except subprocess.CalledProcessError:
        return None

def get_toolchain_search_dirs():
    for i in TOOLCHAIN_SEARCH_DIRS:
        yield i
    extra_search_dir = os.getenv('NSWI004_TOOLCHAIN_DIR')
    if extra_search_dir:
        yield extra_search_dir


def main(argv):
    args = argparse.ArgumentParser(description='Configure the build')
    args.add_argument('--verbose',
        default=False,
        dest='verbose',
        action='store_true',
        help='Be verbose.'
    )
    args.add_argument('-f', '--force',
        default=False,
        dest='force',
        action='store_true',
        help='Overwrite existing files.'
    )
    args.add_argument('--toolchain',
        default=None,
        dest='toolchain_dir',
        help='Toolchain directory.'
    )
    args.add_argument('--target',
        default=None,
        dest='toolchain_target',
        help='Toolchain target triplet.'
    )
    args.add_argument('--debug',
        default=False,
        dest='debug',
        action='store_true',
        help='Build kernel in debug mode.'
    )
    args.add_argument('--memory-size',
        default=None,
        dest='memory_size',
        type=int,
        help='Base memory size (msim.conf) in KB'
    )
    args.add_argument('--kernel-cflags',
        default=[],
        dest='kernel_cflags',
        action='append',
        help='Extra kernel compiler flags'
    )

    tests = args.add_mutually_exclusive_group(required=False)
    tests.add_argument('--kernel-test',
        default=None,
        dest='kernel_test',
        help='Kernel test (name).'
    )

    config = args.parse_args(argv[1:])

    config.logging_level = logging.DEBUG if config.verbose else logging.INFO
    logging.basicConfig(format='[%(asctime)s %(name)-6s %(levelname)7s] %(message)s', level=config.logging_level)
    logger = logging.getLogger('main')
    
    # Get basic paths
    src_base = os.path.realpath(os.path.dirname(argv[0]))
    cwd = os.path.realpath(os.getcwd())
    is_out_of_tree_build = src_base != cwd

    # Cannot set memory size in in-tree build
    # (we do not want to modify msim.conf in place).
    if (config.memory_size is not None) and (not is_out_of_tree_build):
        logger.critical('Cannot change memory size when building in source tree.')
        sys.exit(1)
    if config.memory_size is None:
        config.memory_size = 1024

    # Detect toolchain
    if config.toolchain_dir is None:
        for tc_dir in get_toolchain_search_dirs():
            if config.toolchain_target is None:
                tc_possible_targets = MIPSEL_TARGETS
            else:
                tc_possible_targets = [config.toolchain_target]

            for tc_target in tc_possible_targets:
                if has_gcc_installed(tc_dir, tc_target):
                    config.toolchain_dir = tc_dir
                    if config.toolchain_target is None:
                        config.toolchain_target = tc_target
    elif config.toolchain_target is None:
        for tc_target in MIPSEL_TARGETS:
            if has_gcc_installed(config.toolchain_dir, tc_target):
                config.toolchain_target = tc_target

    if config.toolchain_dir is None:
        logger.critical("Failed to locate cross-compiler toolchain.")
        logger.critical("Have you installed the toolchain and/or have you specified --toolchain?")
        sys.exit(1)

    if config.toolchain_target is None:
        logger.critical("Failed to determine cross-compiler target.")
        logger.critical("Have you installed the toolchain and/or have you specified --target?")
        sys.exit(1)

    if not has_gcc_installed(config.toolchain_dir, config.toolchain_target):
        logger.critical("GCC for {} not found in {}.".format(config.toolchain_target, config.toolchain_dir))
        logger.critical("Have you installed the toolchain?")
        sys.exit(1)

    msim_version = get_msim_version()
    if not msim_version:
        logger.warning("MSIM not found on $PATH.")

    # Always create config.mk
    logger.debug('Creating config.mk')
    with open('config.mk', 'w') as f:
        kernel_test_sources = ''

        kernel_extra_cflags = []
        for flag in config.kernel_cflags:
            kernel_extra_cflags.append('-D{}'.format(flag))

        kernel_extra_asflags = []

        if config.debug:
            kernel_extra_cflags.append('-DKERNEL_DEBUG')

        if msim_version:
            # For simpler comparisons
            msim_version_str = '{:03d}-{:03d}-{:03d}-{:03d}'.format(*msim_version)
            if msim_version_str >= '001-004-001-000':
                kernel_extra_cflags.append('-DMSIM_HAS_XCRV')
                kernel_extra_asflags.append('-DMSIM_HAS_XCRV')

        if config.kernel_test is not None:
            kernel_test_sources = 'tests/{}/test.c'.format(config.kernel_test)
            if not os.path.isfile(os.path.join(src_base, 'kernel', kernel_test_sources)):
                logger.critical("Kernel test source {} not found.".format(kernel_test_sources))
                logger.critical("Have you specified both group and test name?")
                sys.exit(1)

            kernel_extra_cflags.append('-DKERNEL_TEST')
            if config.kernel_test in KERNEL_TEST_EXTRAS:
                test_extras = KERNEL_TEST_EXTRAS[config.kernel_test]
                if 'CFLAGS' in test_extras:
                    for i in test_extras['CFLAGS']:
                        kernel_extra_cflags.append(i.format(
                            mainmem_size=config.memory_size
                        ))

        f.write(CONFIG_MK_FMT.format(
            toolchain_dir=config.toolchain_dir,
            toolchain_target=config.toolchain_target,
            kernel_test_sources=kernel_test_sources,
            kernel_extra_cflags=' '.join(kernel_extra_cflags),
            kernel_extra_asflags=' '.join(kernel_extra_asflags),
        ))

    # Create rest of files only when building in different directory
    if is_out_of_tree_build:
        # Create Makefiles
        for sub in ['.', 'kernel']:
            logger.debug('Creating Makefile inside {}/'.format(sub))
            os.makedirs(sub, exist_ok=True)
            with open(os.path.join(cwd, sub, 'Makefile'), 'w') as f:
                f.write(BUILD_DIR_MAKEFILE_FMT.format(
                    src_base=os.path.join(src_base, sub)
                ))

        # Do not overwrite these
        for f in ['gdbinit']:
            logger.debug('Copying {}'.format(f))
            with open(os.path.join(src_base, f), 'rb') as inp:
                try:
                    copy_to_file(inp, f, 'xb')
                except FileExistsError:
                    if config.force:
                        logger.warning('Overwriting {}'.format(f))
                        copy_to_file(inp, f, 'w')
                    else:
                        logger.error('Refusing to overwrite {}'.format(f))

        # Write msim.conf
        msim_conf_data = MSIM_CONF_FMT.format(
            ram_size=config.memory_size
        )
        msim_conf_path = os.path.join(cwd, 'msim.conf')
        try:
            with open(msim_conf_path, 'x') as f:
                f.write(msim_conf_data)
        except FileExistsError:
            if config.force:
                logger.warning('Overwriting msim.conf')
                with open(msim_conf_path, 'w') as f:
                    f.write(msim_conf_data)
            else:
                logger.error('Refusing to overwrite msim.conf')

if __name__ == "__main__":
    main(sys.argv)
