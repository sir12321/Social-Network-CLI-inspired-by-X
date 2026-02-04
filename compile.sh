
#!/bin/bash
# Portable compile script for Unix-like shells
# Builds long_ass.cpp into long_ass executable
set -euo pipefail

SRC="long_ass.cpp"
OUT="long_ass"

if ! command -v g++ >/dev/null 2>&1; then
    echo "g++ not found in PATH"
    exit 1
fi

echo "Compiling $SRC -> $OUT"
g++ -std=c++17 -O2 "$SRC" -o "$OUT"
echo "Compilation successful. Output: $OUT"