#!/usr/bin/env bash

# Small helper script for generating parsers from ANTLR grammars
# Provided in the hope that it might be useful (use at your own risk etc.)

set -euo pipefail
set -x

function generate() {
    antlr4 -Dlanguage=Cpp -o ./gen -visitor -no-listener "$1"
}

generate "Formula.g4"
generate "Log.g4"