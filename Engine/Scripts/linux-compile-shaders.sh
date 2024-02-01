#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <input_directory> <output_directory>"
    exit 1
fi

INPUT_DIR="$1"
OUTPUT_DIR="$2"

mkdir -p "$OUTPUT_DIR"

# vertex Shaders
for vert_shader in "$INPUT_DIR"/*.vert; do
    vert_shader_name=$(basename "$vert_shader")
    vert_output="$OUTPUT_DIR/${vert_shader_name%.vert}.vert.spv"
    $VULKAN_SDK/bin/glslc "$vert_shader" -o "$vert_output"

    # check stataus code
    if [ $? -eq 0 ]; then
        echo "$vert_shader --> $vert_output."
    else
        echo "Could not compile $vert_shader".
    fi
done

# fragment Shaders
for frag_shader in "$INPUT_DIR"/*.frag; do
    frag_shader_name=$(basename "$frag_shader")
    frag_output="$OUTPUT_DIR/${frag_shader_name%.frag}.frag.spv"
    $VULKAN_SDK/bin/glslc "$frag_shader" -o "$frag_output"

    # check stataus code
    if [ $? -eq 0 ]; then
        echo "$frag_shader --> $frag_output."
    else
        echo "Could not compile $frag_shader".
    fi
done