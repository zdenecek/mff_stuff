# Tasks 14 (student tomisz)

| Total                                            |    85 |
|--------------------------------------------------|------:|
| 14/shellcheck.sh (+ .gitlab-ci.yml)              |    45 |
| 14/command.txt                                   |    15 |
| 14/volume.txt                                    |    25 |

If you see an issue with the grading, please
[open a **Confidential Issue**](https://gitlab.mff.cuni.cz/teaching/nswi177/2022/common/forum/-/issues/new?issue[confidential]=true&issue[title]=Grading+Tasks+14)
in the _Forum_.


For assignments with automated tests you will see a TAP-style result output
that you are familiar with from your pipeline tests in GitLab.

The tests also contains information about points assigned (or subtracted)
for that particular test. There are also tests with _zero points_ that
are informative only (kind of like warnings from your compiler: you
should pay attention but they are not crucial).

## 14/shellcheck.sh (+ .gitlab-ci.yml)

✅ **Submitted** (passed, informative only)

✅ **Proper shebang** (passed, +0 points)

✅ **Executable bit** (passed, +0 points)

✅ **Shellcheck errors** (passed, +0 points)

✅ **Shellcheck warnings** (passed, +0 points)

✅ **Shellcheck infos** (passed, informative only)

✅ **Shellcheck stylistic** (passed, informative only)

✅ **Actually calls shellcheck** (passed, +0 points)

❌ **Terminates with zero on correct scripts I** (failed, worth 5 points) \

```
-- Unexpected exit code of 14/shellcheck.sh --
actual (1 lines):
  1
expected (1 lines):
  0
stderr (0 lines):
stdout (7 lines):
  In ./14/solution_for_40_points.sh line 1:
  ^-- SC2148: Tips depend on target shell and yours is unknown. Add a shebang or a 'shell' directive.
  For more information:
    https://www.shellcheck.net/wiki/SC2148 -- Tips depend on target shell and y...
repository (15 lines):
  .
  ./06
  ./06/sysinfo.sh
  ./14
  ./14/GRADING-before.md
  ./14/web
  ./14/web/bg.jpg
  ./14/web/main.css
  ./14/web/index.html
  ./14/before.en.md
  ./14/solution_for_40_points.sh
  ./14/command.txt
  ./14/before.md
  ./14/volume.txt
  ./14/shellcheck.sh
06/sysinfo.sh (3 lines):
  #!/bin/bash
  echo "This is a proper script"
--
```

❌ **Terminates with zero on correct scripts II** (failed, worth 10 points) \

```
-- Unexpected exit code of 14/shellcheck.sh --
actual (1 lines):
  1
expected (1 lines):
  0
stderr (0 lines):
stdout (7 lines):
  In ./14/solution_for_40_points.sh line 1:
  ^-- SC2148: Tips depend on target shell and yours is unknown. Add a shebang or a 'shell' directive.
  For more information:
    https://www.shellcheck.net/wiki/SC2148 -- Tips depend on target shell and y...
repository (17 lines):
  .
  ./06
  ./06/sysinfo.sh
  ./14
  ./14/GRADING-before.md
  ./14/web
  ./14/web/bg.jpg
  ./14/web/main.css
  ./14/web/index.html
  ./14/before.en.md
  ./14/solution_for_40_points.sh
  ./14/command.txt
  ./14/before.md
  ./14/volume.txt
  ./14/shellcheck.sh
  ./01
  ./01/factor.py
06/sysinfo.sh (3 lines):
  #!/bin/bash
  echo "This is a proper script"
--
```

✅ **Terminates with non-zero on bad scripts I** (passed, +5 points)

✅ **Terminates with non-zero on bad scripts II** (passed, +10 points)

✅ **Submitted** (passed, informative only)

✅ **Valid YAML** (passed, informative only)

✅ **Has shellcheck job** (passed, +10 points)

✅ **Shellcheck pipeline calls 14/shellcheck.sh** (passed, +10 points)

✅ **Shellcheck pipeline actually executed** (passed, +10 points)



### General notes (collected from all solutions)

There were two common mistakes that we have encountered. We had to update
the tests to explicitly check for them as we have not expected that you
would not try to run the script yourself.

The first issue is that the
**script must announce its success/failure via an exit code**.
That is what controls how GitLab will treat the pipeline.
Therefore, no shellcheck issues means the script terminates with zero
exit code, found issues means non-zero exit code.

The second issue is that we have encountered about 15 solutions where you
simply copied our assertion (as per hint) but without actually running it.
Because that is only possible explanation of keeping the call to `run` there
which is a BATS-specific function. Therefore, you scripts then never called
shellcheck and failed on each invocation.

Our solution as noted below is a bit more advanced as it actually generates
a standard BATS test that is executed. While it may seem as an overkill
it unifies the output with other tests and it allows easier reuse of the
function.

Note that we launch the script from `.gitlab-ci.yml` with `find` arguments
to make the script a bit more versatile.
Our tests tried to guess the expected launch syntax of `14/shellcheck.sh`
from your CI configuration -- if you have something wild there and you think
our tests were unable to detect that, please, let us know.

```shell
#!/bin/bash

# run as ./14/shellcheck.sh [01]*/*.sh
# or ./14/shellcheck.sh [01][0-9] -name '*.sh'

set -ueo pipefail

SHELLCHECK_SCRIPT_GENERATED="$( mktemp )"
trap 'rm -f "$SHELLCHECK_SCRIPT_GENERATED"' EXIT
export SHELLCHECK_SCRIPT_GENERATED

check_one_file() {
    local filename="$1"
    printf '@test "Shellcheck %q (error)" {\n' "$filename" >>"$SHELLCHECK_SCRIPT_GENERATED"
    printf '    assert_is_shellchecked %q "error"\n' "$filename" >>"$SHELLCHECK_SCRIPT_GENERATED"
    printf '}\n\n' >>"$SHELLCHECK_SCRIPT_GENERATED"

    printf '@test "Shellcheck %q (info)" {\n' "$filename" >>"$SHELLCHECK_SCRIPT_GENERATED"
    printf '    assert_is_shellchecked %q "info"\n' "$filename" >>"$SHELLCHECK_SCRIPT_GENERATED"
    printf '}\n\n' >>"$SHELLCHECK_SCRIPT_GENERATED"
}

cat >"$SHELLCHECK_SCRIPT_GENERATED" <<'EOF_HEADER'
#!/usr/bin/env bats

assert_is_shellchecked() {
    local script="${1}"
    local severity="${2:-info}"

    run shellcheck -S "$severity" "${script}"
    if [ "$status" -ne 0 ]; then
        echo >&2
        echo "-- Found Shellcheck issues in ${script} (severity ${severity}) --" >&2
        echo "$output" >&2
        echo >&2
        return 1
    fi
}

EOF_HEADER


export -f check_one_file

find "$@" -type f -print0 | xargs -0 -n 1 bash -c 'check_one_file "$@"' check_one_file

# Uncomment for debugging
# cat "$SHELLCHECK_SCRIPT_GENERATED"

bats -t "$SHELLCHECK_SCRIPT_GENERATED"
```


## 14/command.txt

✅ **Submitted** (passed, informative only)

✅ **Not executable** (passed, +0 points)

✅ **Correct answer** (passed, +15 points)



## 14/volume.txt

✅ **Submitted** (passed, informative only)

✅ **Not executable** (passed, +0 points)

✅ **Correct first string** (passed, +10 points)

✅ **Correct second string** (passed, +15 points)



