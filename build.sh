#! /bin/bash

BIN=batcry

mkdir -p bin/obj

for source_file in $(find src -type f -printf '%f\n')
do
  echo "compiling src/$source_file"

  # ${source_file/.c/.o} replaces .c with .o; -c tells gcc to not link; 
  gcc "src/$source_file" -o "bin/obj/${source_file/.c/.o}" -O3 -c -Iinclude
done

echo 'linking'

# Link all the .o files
gcc -o "bin/$BIN" $(find bin/obj -type f -name '*.o' -printf '%p ')

echo 'done'
