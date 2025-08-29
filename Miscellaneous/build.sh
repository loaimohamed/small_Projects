#!/bin/bash

# Check if file was provided
if [ $# -eq 0 ]; then
    echo "Error: No file provided"
    echo "Usage: $0 filename"
    exit 1
fi

file="$1"
extension="${file##*.}"
filename="${file%.*}"

echo "Building: $file"
echo ""

if [ ! -f "$file" ]; then
    echo "Error: File '$file' not found!"
    exit 1
fi

# Determine file type and compile
case "$extension" in
    c)
        echo "[C Compilation]"
        gcc -o "$filename" "$file" -Wall -Wextra 2>&1
        compile_error=$?
        ;;
    go)
        echo "[Go Compilation]"
        go build -o "$filename" "$file" 2>&1
        compile_error=$?
        ;;
    rs)
        echo "[Rust Compilation]"
        rustc -o "$filename" "$file" 2>&1
        compile_error=$?
        ;;
    *)
        echo "Error: Unsupported file type '.$extension'"
        echo "Supported extensions: .c, .go, .rs"
        exit 1
        ;;
esac

echo ""
if [ $compile_error -eq 0 ]; then
    echo "✓ Compilation successful!"
    echo "Output: $filename"
else
    echo "✗ Compilation failed with error code: $compile_error"
    echo ""
    echo "=== ERROR OUTPUT ==="
    # Re-run to show errors (since we captured stderr above)
    case "$extension" in
        c) gcc -o "$filename" "$file" -Wall -Wextra ;;
        go) go build -o "$filename" "$file" ;;
        rs) rustc -o "$filename" "$file" ;;
    esac
    exit $compile_error
fi