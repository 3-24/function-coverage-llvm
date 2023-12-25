#include<stdio.h>
#include<stdlib.h>

static FILE *fp;

void init_out_file(){
    char *out_file = getenv("OUT_FILE");
    if(out_file == NULL){
        fprintf(stderr, "OUT_FILE not set\n");
        exit(1);
    }
    fp = fopen(out_file, "w");
    return;
} 

void print_func_entry(char * filename, char * func_name) {
    fprintf(fp, "CALL %s %s\n", filename, func_name);
    return;
}

void print_func_ret(char * filename, char * func_name) {
    fprintf(fp, "RET %s %s\n", filename, func_name);
    return;
}