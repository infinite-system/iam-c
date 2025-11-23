#!/bin/bash

# Name of the script itself
SELF="$(basename "$0")"

# Output file
OUT="output.txt"

# Remove old output
: > "$OUT"

# Loop through files in current directory
for f in *; do
    # Skip directories
    [ -d "$f" ] && continue

    # Skip exclusions
    case "$f" in
        "$SELF" | "Makefile" | "iam_test" | "iam.c" | "$OUT")
            continue
        ;;
    esac

    # Write header + file contents
    echo "Filename: $f" >> "$OUT"
    echo "----------------------------------------" >> "$OUT"
    cat "$f" >> "$OUT"
    echo -e "\n\n" >> "$OUT"
done

echo "Done. Output saved to $OUT."