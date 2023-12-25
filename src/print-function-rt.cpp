#include<stdio.h>
#include<stdlib.h>

#include <set>
#include <string>

using namespace std;

extern "C"{

static FILE *fp;
static set<pair<string, string>> func_set;

void init_out_file(){
    char *out_file = getenv("OUT_FILE");
    if(out_file == NULL){
        fprintf(stderr, "OUT_FILE not set\n");
        exit(1);
    }
    fp = fopen(out_file, "w");
    return;
} 

void print_func_entry(char * file_name, char * func_name) {
    pair<string, string> func_pair = make_pair(string(file_name), string(func_name));
    if (func_set.find(func_pair) != func_set.end()) {
        return;
    } else {
        func_set.insert(func_pair);
        fprintf(fp, "CALL %s at %s\n", func_name, file_name);
        return;
    }
}
}