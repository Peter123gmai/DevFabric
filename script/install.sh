#!/usr/bin/env bash

# recompile source code to optimize for user's system
rm ../bin/cli
g++ ../cli/cli.cpp -o ../bin/cli

mkdir ../debug # create debug folder


SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BIN_DIR="$PROJECT_ROOT/bin"

chmod +x "$BIN_DIR"/* 2>/dev/null || true

EXPORT_LINE="export PATH=\"$BIN_DIR:\$PATH\""

if ! grep -Fxq "$EXPORT_LINE" "$HOME/.bashrc"; then
    echo "" >> "$HOME/.bashrc"
    echo "$EXPORT_LINE" >> "$HOME/.bashrc"
    echo "Added $BIN_DIR to ~/.bashrc"
else
    echo "Path already exists in ~/.bashrc"
fi

echo "Run: source ~/.bashrc"
