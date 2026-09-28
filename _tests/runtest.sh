#!/bin/sh

#
# cnext - Modern "NonStandard" C library.
#
# Copyright (c) 2026-present Tanvir.
# SPDX-License-Identifier: MPL-2.0
#

# runtest.sh - compile each _tests/*.c, run, diff stdout vs *.expected
# usage: sh _tests/runtest.sh [testname...]

set -eu

ROOT=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
CC=${CC:-cc}
CFLAGS=${CFLAGS:--std=c17 -Wall -Wextra -I"$ROOT" -I"$(dirname "$ROOT")"}
TMPD=$(mktemp -d)
trap 'rm -rf "$TMPD"' EXIT INT TERM

if [ -t 1 ] && [ -z "${NO_COLOR:-}" ]; then
      GREEN=$(printf '\033[32m')
      RED=$(printf '\033[31m')
      RESET=$(printf '\033[0m')
else
      GREEN=''
      RED=''
      RESET=''
fi

pass=0
fail=0

run_one() {
      src=$1
      name=$(basename "$src" .c)
      exp="$ROOT/_tests/$name.expected"
      bin="$TMPD/$name"
      actual="$TMPD/$name.actual"

      if ! $CC $CFLAGS -I"$ROOT" "$src" -o "$bin" 2>"$TMPD/$name.build.log"; then
            printf '%sFAIL%s %s (compile)\n' "$RED" "$RESET" "$name"
            cat "$TMPD/$name.build.log"
            fail=$((fail + 1))
            return 0
      fi
      if ! "$bin" >"$actual" 2>&1; then
            printf '%sFAIL%s %s (non-zero exit)\n' "$RED" "$RESET" "$name"
            cat "$actual"
            fail=$((fail + 1))
            return 0
      fi
      if [ ! -f "$exp" ]; then
            printf '%sFAIL%s %s (missing %s.expected)\n' "$RED" "$RESET" "$name" "$name"
            fail=$((fail + 1))
            return 0
      fi
      if diff -u "$exp" "$actual"; then
            printf '%sPASS%s %s\n' "$GREEN" "$RESET" "$name"
            pass=$((pass + 1))
      else
            printf '%sFAIL%s %s (output mismatch)\n' "$RED" "$RESET" "$name"
            fail=$((fail + 1))
      fi
}

if [ "$#" -gt 0 ]; then
      for t in "$@"; do
            case $t in
            *.c) run_one "$ROOT/_tests/$t" ;;
            *) run_one "$ROOT/_tests/$t.c" ;;
            esac
      done
else
      for src in "$ROOT"/_tests/*.c; do
            [ -e "$src" ] || continue
            run_one "$src"
      done
fi

printf '%d passed, %d failed\n' "$pass" "$fail"
[ "$fail" -eq 0 ]
