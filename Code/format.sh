#!/bin/sh

current_dir=$(pwd)

# Change to the script's directory
cd "$(dirname "$0")" || exit 1

# Return to the original directory on script exit
trap 'cd "$current_dir"' EXIT

echo "Running code formatter..."

update_in_place=false
if [ "$1" = "--in-place" ]; then
    update_in_place=true
fi


echo "Using clang-format version:"
clang-format --version

if [ "$update_in_place" = true ]; then
    echo "Updating files in place."
    
    find ./ -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i
else
    echo "Checking formatting without updating files."

    find ./ -iname '*.h' -o -iname '*.cpp' | xargs clang-format --dry-run --Werror
    if [ $? -ne 0 ]; then
        echo "Formatting issues found. Please run './Code/format.sh --in-place' to fix them."
        exit 1
    else
        echo "All files are properly formatted."
    fi
fi

cd "$current_dir"
