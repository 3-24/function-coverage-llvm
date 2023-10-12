#include<stdio.h>
#include<stdlib.h>

void print_func_entry(char * func_name) {
    fprintf(stderr, "Function entry: %s\n", func_name);
    return;
}

void print_func_ret(char * func_name) {
    fprintf(stderr, "Function exit: %s\n", func_name);
    return;
}