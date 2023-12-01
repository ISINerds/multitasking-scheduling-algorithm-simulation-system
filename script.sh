
for file in ./src/scheduling_algorithms/*.c; do gcc -shared -fPIC $file -o ./build/algorithms/`basename $file .c`; done 