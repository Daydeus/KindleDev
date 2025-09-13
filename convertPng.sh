#!/bin/bash
# Name: convertPng.sh
# Author: Daydeus
# Description: Converts a png file in the graphics folder into a C source code file and creates a
#              header file so the image can be included in the program.
###################################################################################################

# Define the image file path.
PATH_TO_IMAGE="$1"

# Only begin process if the image file exists.
if [ -f "$PATH_TO_IMAGE" ]; then
  # Extract the file name from the path.
  FILE_NAME=$(basename "$PATH_TO_IMAGE" .png)

  # Define the output C source code file name.
  CODE_FILE="src/$FILE_NAME.cpp"

  # Run gdk-pixbuf-csource to generate C code from the image.
  # --name: Specifies the identifier name for the generated variables/macros.
  # --raw: Disables run-length encoding for the pixel data.
  # --extern: Allows the data array to be included in other files.
  gdk-pixbuf-csource --name=$FILE_NAME --raw --extern "$PATH_TO_IMAGE" > "$CODE_FILE"
  echo "C source code file generated successfully for $PATH_TO_IMAGE"

  # Add header includes to generated C source file.
  sed -i "3i\
  #include <glib-2.0/glib.h>\n\
  #include \"$FILE_NAME.h\"\n" "$CODE_FILE"

  # Create header file if it doesn't already exist.
  if [ ! -f "include/$FILE_NAME.h" ]; then
    # Define the output C header file name.
    HEADER_FILE="include/$FILE_NAME.h"

    # Define content of the header file.
    HEADER_CONTENT="// Header file for c-source converted $FILE_NAME.png
    #ifndef ${FILE_NAME^^}_H
    #define ${FILE_NAME^^}_H
    #include <glib-2.0/glib.h>
    extern const guint8 $FILE_NAME[];
    #endif // ${FILE_NAME^^}_H"

    # Create the header file and add the content.
    echo "$HEADER_CONTENT" | sed 's/^[[:space:]]*//' > "$HEADER_FILE"
    echo "C header file generated successfully for $CODE_FILE"
  fi
else
  echo "$PATH_TO_IMAGE does not exist. Unable to generate C source code file."
fi
