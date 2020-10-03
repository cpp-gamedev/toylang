#!/bin/bash

[[ ! -f ./tools/clang-format-all.sh ]] && echo "Must run script from repo root" && exit 1

find ./src -name "*.?pp" | xargs clang-format -i
