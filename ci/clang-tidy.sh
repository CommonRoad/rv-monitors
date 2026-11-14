#!/bin/bash

# set exit on error
# set -e

exit_flag=false

# preference list
clang_tidy_tool_candiates=(clang-tidy-13 clang-tidy-12 clang-tidy-11 clang-tidy-10 clang-tidy)

for candidate in "${clang_tidy_tool_candiates[@]}"; do

    if command -v "$candidate" >/dev/null; then
        echo "clang-tidy check with $candidate --version:"
        $candidate --version
        clang_tidy_tool=$candidate
        echo
        break
    fi
done

if [[ -z "$clang_tidy_tool" ]]; then
    echo "$clang_tidy_tool not correctly installed"
    exit 1
fi

if [[ "$CI" = "true" ]]; then
    # In CI environment
    if [ -z ${CI_MERGE_REQUEST_TARGET_BRANCH_NAME+x} ]; then
        # Outside of a merge request: Compare against default branch (usually master)
        diff_target_branch="${CI_DEFAULT_BRANCH}"
    else
        # In merge request: Compare against MR target branch
        diff_target_branch="${CI_MERGE_REQUEST_TARGET_BRANCH_NAME}"
    fi

    # Diff target has not been checked out in CI environment, therefore the remote tracking is not set up
    # (origin should be fine in 99% of all cases, unless Gitlab changes anything)
    diff_target_remote="origin"

else
    # Outside CI environment: Compare against current remote
    diff_target_branch="$(git branch --show-current)"

    # Fetch remote for current branch
    diff_target_remote="$(git config "branch.$diff_target_branch.remote")"
fi

diff_target="$diff_target_remote/$diff_target_branch"

# Retrieve list of cpp-files that were changed in source branch with respect to master (target branch)
mapfile -t filelist < <(git diff "${diff_target}" --name-only | grep ".cpp")

if [[ "${#filelist[@]}" -eq "0" ]]; then
    echo "==> No cpp files found"
    echo "==> clang-tidy has nothing to do, stop early"
    exit 0
else
    echo "==> Found ${#filelist[@]} cpp files"
    echo "==> ${filelist[*]}"
    echo "==> Let's start our clang-tidy check"
fi

echo
echo "clang-tidy checking changed files compared to target branch ${diff_target}"
echo

# function to check if C++ file (based on suffix)
function checkCPP(){
    if [[ -f $1 ]] && [[ $1 == *.cpp ]]; then
        return 0
    fi
    return 1
}


filesWithErrors=()

# check list of files
for f in "${filelist[@]}"; do
    # check if .cpp file and in compilation DB
    if checkCPP "$f" && grep -q "$f" build/compile_commands.json; then
        printf "   Checking matching file %s\t" "${f}"
        touch output.txt
        $clang_tidy_tool -quiet -p=build "${f}" --extra-arg=--cuda-host-only > output.txt

        # decide if error or warning fail
        if grep -Eq "(warning: )|(error: )" output.txt; then
            echo ""
            echo "You must pass the clang tidy checks before submitting a pull request"
            echo ""
            grep --color -E '^|warning: |error: ' output.txt
            if grep -q "error: " output.txt; then
                exit_flag=true
                filesWithErrors+=( "$f" )
                echo -e " \033[1;31m\xE2\x9C\x98 failed file $f\033[0m";
            else
                echo -e " \033[1;33m\xE2\x9C\x93 passed file $f with warnings\033[0m";
            fi
        else
            echo -e " \033[1;32m\xE2\x9C\x93 passed file $f\033[0m";
        fi
        rm output.txt
    else
        echo "$f not a C++ file or not in compilation database (compile_commands.json)"
    fi
done

echo

if [ "$exit_flag" = true ]; then
    echo -e "Error in file(s):"
    for f in "${filesWithErrors[@]}"; do
        printf "\t \u2022 %s\n" "$f"
    done
    exit 1
else
    echo "clang-tidy check passed"
fi
