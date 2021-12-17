#!/usr/bin/env bash

# NOTE: Adapted from run_tests.sh/test_single.sh in Hydra to test rvmonitors_eval

set -uo pipefail

HYDRA_DIR="/opt/hydra"
# HYDRA_CMD="$HYDRA_DIR/hydra"
HYDRA_CMD="$1"
VYDRA_CMD="$HYDRA_DIR/vydra.opt"

function test_single() {
    local SIZE=${1}
    local MAXR=${2}
    local SEED=${3}
    local FID="${1}_${2}_${3}"
    local LOG="logs/shared"
    local APS=8
    local TYPE=2

    #gen_fmla PREFIX SIZE MAXR TYPE SCALE SEED APS
    ./gen_fmla "fmlas/z_${FID}" "${SIZE}" "${MAXR}" "${TYPE}" 1 "${SEED}" ${APS}

    printf "Testing %s\n" "$(cat fmlas/z_${FID}.mdl)"

    # 2>> error.log
    if ! $HYDRA_CMD "fmlas/z_${FID}.mdl" "${LOG}.log" > "output/z_${FID}.hydra"; then
        cp "fmlas/z_${FID}.mdl" "bug_${FID}.mdl"
        printf "Running %s failed on %s!\n" "$HYDRA_CMD" "fmlas/z_${FID}.mdl"
        return 1
    fi

    # 2>> error.log
    if ! $VYDRA_CMD -fmla "fmlas/z_${FID}.mdl" -log "${LOG}.log" > "output/z_${FID}.vydra"; then
        cp "fmlas/z_${FID}.mdl" "bug_${FID}.mdl"
        printf "Running %s failed on %s!\n" "$VYDRA_CMD" "fmlas/z_${FID}.mdl"
        return 1
    fi

    sed -i "/^[A-Z]/d" "output/z_${FID}.hydra"

    if ! diff --brief "output/z_${FID}.hydra" "output/z_${FID}.vydra"; then
        echo "DIFFERENT"
        echo "Diff preview: "
        diff --side-by-side --suppress-common-lines --width=60 \
            "output/z_${FID}.hydra" "output/z_${FID}.vydra" | head
        cp "fmlas/z_${FID}.mdl" "bug_${FID}.mdl"
    else
        echo "OK"
    fi
}

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
            printf "(%${DIGITS}d/$TOTAL)" $ITER

            test_single ${SIZE} ${MAXR} "${SEED}"
         done
     done
done