#!/bin/bash
# Name: convertPng.sh
# Author: Daydeus
# Description: Converts the example.png in the graphics folder to a C source file and creates a
#              header file so the image can be included in the program.
###################################################################################################

# Define the image file path.
IMAGE_PATH="graphics/example.png"

# Define the output C source file name.
OUTPUT_FILE="src/example.cpp"

# Run gdk-pixbuf-csource to generate C code from the image.
# --name: Specifies the identifier name for the generated variables/macros.
# --raw: Disables run-length encoding for the pixel data.
# --extern: Allows the data array to be included in other files.
gdk-pixbuf-csource --name=example --raw --extern "$IMAGE_PATH" > "$OUTPUT_FILE"

echo "C source code generated successfully for $IMAGE_PATH"

# Add header includes to generated C source file.
sed -i "3i\
#include <glib-2.0/glib.h>\n\
#include \"example.h\"\n" "$OUTPUT_FILE"

# Create header file if it doesn't already exist.
if [ ! -f "include/example.h" ]; then
echo -e "// Header file for c-source converted example.png\n
#ifndef EXAMPLE_H
#define EXAMPLE_H\n
#include <glib-2.0/glib.h>\n
extern const guint8 example[];\n
#endif // EXAMPLE_H" > include/example.h
echo "C header file generated successfully for $OUTPUT_FILE"
fi
