#!/usr/bin/env bash

# Small helper script for debugging ANTLR grammars
# Provided in the hope that it might be useful (use at your own risk etc.)

set -euo pipefail

# Depending on your ANTLR installation, you might need to adjust this path
ANTLR_JAR_PATH='/usr/share/java/antlr-complete.jar'

mkdir -p gen-test && cd gen-test

cp ../Log.g4 .
cp ../Formula.g4 .

antlr4 Log.g4
antlr4 Formula.g4

javac -cp ".:$ANTLR_JAR_PATH" ./*.java

echo "Testing Formula grammar"

grun Formula formula -gui <<EOF
p0 OR (p1 UNTIL[2,2] p3)
EOF

echo "Testing Log grammar"

grun Log log -gui <<EOF
@0 p1 p2
@0 p0 p2
@1
@4 p0 p1 p2
@4 p1 p2
@5 p0 p1 p2
EOF
