# print_function
Using LLVM IR modification


# usage

1. ``gclang <program.c> -g -O0 -o <program.exec>``
2. ``get-bc <program.exec>``
3. ``opt <program.bc> -enable-new-pm=0 -load "print_function/lib/libprintfunc.so" --PrintFunc < <program.bc> -o <tmp.bc>``
4. ``clang++ <tmp.bc> -L "print_function/lib" -lpf-rt -o <program.print>``
5. ``./<program.print>``