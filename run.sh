#!/bin/bash

# Usage: ./script.sh <option> [source_file.cpp]

OPTION=$1
SRC=$2

# Function to get output file name from source
get_out_file() {
    local src=$1
    if [[ "$src" != *.cpp ]]; then
        echo "Error: Source file must have .cpp extension."
        exit 1
    fi
    echo "${src%.cpp}.out"
}

case $OPTION in
    build)
        if [ -z "$SRC" ]; then
            echo "Error: Please provide a source file to build."
            exit 1
        fi
        OUT=$(get_out_file "$SRC")
        g++ -std=c++20 -pthread "$SRC" -o "$OUT"
        if [ $? -ne 0 ]; then
            echo "Compilation failed"
            exit 1
        fi
        echo "Build successful: $OUT"
        ;;

    debug)
        if [ -z "$SRC" ]; then
            echo "Error: Please provide a source file to debug."
            exit 1
        fi
        OUT=$(get_out_file "$SRC")
        g++ -std=c++20 -pthread -g "$SRC" -o "$OUT"
        if [ $? -ne 0 ]; then
            echo "Compilation failed"
            exit 1
        fi
        gdb ./"$OUT"
        ;;

    run)
        if [ -z "$SRC" ]; then
            echo "Error: Please provide the source file corresponding to the executable to run."
            exit 1
        fi
        OUT=$(get_out_file "$SRC")
        if [ ! -f "$OUT" ]; then
            echo "Error: Executable '$OUT' not found. Build first."
            exit 1
        fi
        ./"$OUT"
        ;;

    clean)
        if [ -z "$SRC" ]; then
            echo "Error: Please provide the source file corresponding to the executable to clean."
            exit 1
        fi
        OUT=$(get_out_file "$SRC")
        if [ -f "$OUT" ]; then
            rm "$OUT"
            echo "Cleaned $OUT"
        else
            echo "Nothing to clean."
        fi
        ;;

    *)
        echo "Usage: $0 {build <source.cpp>|run <source.cpp>|clean <source.cpp>|debug <source.cpp>}"
        exit 1
        ;;
esac
