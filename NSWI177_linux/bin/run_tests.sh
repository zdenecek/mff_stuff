#!/bin/bash

set -ueo pipefail

base_cache_dir="${HOME}/.cache/mff_nswi177/"
tests_repository="https://gitlab.mff.cuni.cz/teaching/nswi177/2022/common/tests.git"
repository_cache_dir="${base_cache_dir}/tests/repo"
next_update_file="${base_cache_dir}/tests/timestamp"
next_update_interval_sec=$(( 60 * 60 * 4 ))

null_logger() {
    cat >/dev/null
}

verbose_logger() {
    cat >&2
}

mark_next_tests_update() {
    local now
    mkdir -p "$( dirname "${next_update_file}" )"
    now="$( date '+%s' )"
    echo "$(( now + next_update_interval_sec ))" >"${next_update_file}"
}

update_tests() {
    if ! [ -f "${repository_cache_dir}/.git/config" ]; then
        rm -rf "${repository_cache_dir}"
        mkdir -p "$( dirname "${repository_cache_dir}" )"
        git clone "${tests_repository}" "${repository_cache_dir}" 2>&1 | ${logger}
        mark_next_tests_update
        return 0
    fi

    local needs_update="${1:-false}"
    if ! [ -f "${next_update_file}" ]; then
        needs_update=true
    else
        if [ "$( cat "${next_update_file}" )" -lt "$( date '+%s' )" ]; then
            needs_update=true
        fi
    fi
    if ${needs_update}; then
        (
            cd "${repository_cache_dir}"
            git pull  2>&1 | ${logger}
        )
        mark_next_tests_update
    fi
}

check_git_origin() {
    if ! git config remote.origin.url | grep -q 'teaching/nswi177/'; then
        echo "WARNING"
        echo "WARNING: This repository is not under teaching/nswi177 group."
        echo "WARNING"
        echo
    fi >&2
}

logger=null_logger
force_update=false
try_update=true
while [ $# -gt 0 ]; do
    case "$1" in
        --update)
            force_update=true
            ;;
        --no-update)
            try_update=false
            ;;
        --verbose)
            logger=verbose_logger
            ;;
        --)
            shift
            break
            ;;
        *)
            break
            ;;
    esac
    shift
done

check_git_origin

if ${try_update}; then
    update_tests "${force_update}"
fi

if [ $# -eq 0 ]; then
    rc=0
    "${repository_cache_dir}/run.sh" tasks quizzes
else
    "${repository_cache_dir}/run.sh" "$@"
fi
