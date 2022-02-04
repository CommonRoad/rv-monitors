#!/usr/bin/env bash

# NOTE: Adapted from run_tests.sh/test_single.sh in Hydra to test rvmonitors_eval

set -uo pipefail

HYDRA_DIR="/opt/hydra"
HYDRA_CMD="$HYDRA_DIR/hydra"
VYDRA_CMD="$HYDRA_DIR/vydra.opt"

RVM_HYDRA_CMD="$1"

export TERM=xterm-256color
# For color output

TEXT_GREEN=$(tput setaf 2)
TEXT_RED=$(tput setaf 1)
TEXT_BOLD=$(tput bold)
TEXT_DIM=$'\x1B[2m'
TEXT_RESET=$'\x1B[0m'

ARTIFACTS_PATH="artifacts/file/eval_results"

function cleanup() {
    # Make sure we leave the shell in a sane state
    printf "%b" "$TEXT_RESET"

    exit
}

function compare_output() {
    local HEAD_LINES=10
    if ! diff --brief "$1" "$2"; then
        printf "%b\nDiff preview: \n" \
            "${TEXT_RESET}"

        diff --side-by-side --suppress-common-lines --width=60 "$1" "$2" | head -n "$HEAD_LINES"
        DIFF_LINES="$(diff --side-by-side --suppress-common-lines --width=60 "$1" "$2" | wc -l)"
        if [[ "$DIFF_LINES" -gt "$HEAD_LINES" ]]; then
            printf "...and %d additional lines\n" $(( DIFF_LINES - "$HEAD_LINES" ))
        fi
        cp "$FML_FILE" "bug_${FID}.mdl"

        printf "%bNOTE%b: In order to review this test case, run the following command after this job has finished:\n\t%s %s %s %s\n\n" \
            "${TEXT_BOLD}" \
            "${TEXT_RESET}" \
            "ci/review-ci-test.sh" \
            "${FID}" \
            "\"${CI_JOB_URL}/${ARTIFACTS_PATH}/${FML_FILE}\"" \
            "\"${CI_JOB_URL}/${ARTIFACTS_PATH}/${LOG_FILE}\""
    else
        printf "%bOK%b\n" \
            "${TEXT_RESET}${TEXT_GREEN}" \
            "${TEXT_RESET}"
    fi
}

function test_single() {
    local SIZE=${1}
    local MAXR=${2}
    local SEED=${3}
    FID="${1}_${2}_${3}"
    LOG="logs/shared"
    local APS=8
    # See gen_fmla sources for meaning of TYPE
    local TYPE=2

    FML_FILE="fmlas/z_${FID}.mdl"
    LOG_FILE="${LOG}.log"
    ERROR_LOG_FILE="error.log"

    #gen_fmla PREFIX SIZE MAXR TYPE SCALE SEED APS
    ./gen_fmla "fmlas/z_${FID}" "${SIZE}" "${MAXR}" "${TYPE}" 1 "${SEED}" ${APS}

    printf "Testing \"%b%s%b\"... " \
        "${TEXT_BOLD}" \
        "$(cat "$FML_FILE")" \
        "${TEXT_RESET}"


    RVM_START="$(date +%s.%N)"
    if ! "$RVM_HYDRA_CMD" "$FML_FILE" "$LOG_FILE" \
        > "output/z_${FID}.rvm_hydra" \
        2>> "$ERROR_LOG_FILE"; then
        cp "$FML_FILE" "bug_${FID}.mdl"
        printf "Running %s failed on %s!\n" "$RVM_HYDRA_CMD" "$FML_FILE"
        return 1
    fi
    RVM_END="$(date +%s.%N)"

    HYDRA_START="$(date +%s.%N)"
    if ! $HYDRA_CMD "$FML_FILE" "$LOG_FILE" \
        > "output/z_${FID}.hydra" \
        2>> "$ERROR_LOG_FILE"; then
        cp "$FML_FILE" "bug_${FID}.mdl"
        printf "Running %s failed on %s!\n" "$HYDRA_CMD" "$FML_FILE"
        return 1
    fi
    HYDRA_END="$(date +%s.%N)"
    sed -i "/^[A-Z]/d" "output/z_${FID}.hydra"

    VYDRA_START="$(date +%s.%N)"
    if ! $VYDRA_CMD -fmla "$FML_FILE" -log "$LOG_FILE" \
        > "output/z_${FID}.vydra" \
        2>> "$ERROR_LOG_FILE"; then
        cp "$FML_FILE" "bug_${FID}.mdl"
        printf "Running %s failed on %s!\n" "$VYDRA_CMD" "$FML_FILE"
        return 1
    fi
    VYDRA_END="$(date +%s.%N)"

    TIMING="$(python -c "print(\"rvm/hydra/vydra {:.2f} s/{:.2f} s/{:.2f} s\".format(
                round(${RVM_END} - ${RVM_START}, 2),
                round(${HYDRA_END} - ${HYDRA_START}, 2),
                round(${VYDRA_END} - ${VYDRA_START}, 2)
            ))")"
    printf "finished! %b(timing: %s)%b\n" "$TEXT_DIM" "$TIMING" "$TEXT_RESET"

    printf "\tComparing \"%s\" against Hydra: %b" "$(basename "$RVM_HYDRA_CMD")" "${TEXT_RED}"
    compare_output "output/z_${FID}.rvm_hydra" "output/z_${FID}.hydra"

    printf "\tComparing \"%s\" against Vydra: %b" "$(basename "$RVM_HYDRA_CMD")" "${TEXT_RED}"
    compare_output "output/z_${FID}.rvm_hydra" "output/z_${FID}.vydra"
}


trap cleanup INT TERM

cd "$HYDRA_DIR/evaluation" || exit 1
# make test
./gen_log logs/shared 10000 4 4 0 16

mkdir -p fmlas

ITER=0
TOTAL=$((6*5*4))
DIGITS=$( python -c "import math; print(math.ceil(math.log10($TOTAL)))" )

for SIZE in 1 2 3 4 8 16; do
    for MAXR in 0 1 2 4 8; do
        for SEED in {0..3}; do
            ITER=$((ITER + 1))
            printf "%b(%${DIGITS}d/$TOTAL)%b " \
                "${TEXT_RESET}${TEXT_DIM}" \
                $ITER \
                "${TEXT_RESET}"

            test_single ${SIZE} ${MAXR} "${SEED}"
         done
     done
done

cleanup
