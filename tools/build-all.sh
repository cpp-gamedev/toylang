#!/bin/bash

[[ ! -f ./tools/build-all.sh ]] && echo "Must run script from repo root" && exit 1

[[ $# < 1 ]] && echo "Pass build dirs as args (-c to run clang-format first)" && exit

for arg in "$@"; do
  if [ "$arg" == "-c" ]; then
    echo "Running clang-format..." && ./tools/clang-format-all.sh
  else
    cmake --build "$arg"
  fi
done
