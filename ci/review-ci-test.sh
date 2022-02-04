#!/usr/bin/env bash

set -euo pipefail

TEST_ID="$1"
FML_URL="$2"
LOG_URL="$3"

TMPDIR="$(mktemp -d --tmpdir ci_fml_review.XXXXXX)"
FML_FILENAME="$TMPDIR/$TEST_ID.fml"
LOG_FILENAME="$TMPDIR/$TEST_ID.log"

RVMONITORS_EVAL="${BINDIR:-..}/rvmonitors_eval"

set +e
if [ ! -f "$RVMONITORS_EVAL" ]; then
    printf "We expected to find \"%s\", but it does not exist!\nPlease specify the directory in which \"rvmonitors_eval\" is located using BINDIR\n" \
        "$RVMONITORS_EVAL"
    exit 1
fi
set -e

printf "Testing %s" "$TEST_ID"

curl --fail-early \
    -o "$FML_FILENAME" "$FML_URL" \
    --next \
    -o "$LOG_FILENAME" "$LOG_URL"

"$RVMONITORS_EVAL" "$FML_FILENAME" "$LOG_FILENAME" || true

rm "$FML_FILENAME"
rm "$LOG_FILENAME"

