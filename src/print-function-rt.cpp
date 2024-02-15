#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <string>
#include <pthread.h>

using namespace std;

extern "C"{

static FILE *seq_fp;
static FILE *rec_fp;
static pthread_mutex_t lock;

static map<pair<string, string>, short> func_set;

void init_out_file(){
    char *record_file = getenv("RECORD_FILE");
    char *out_file = getenv("OUT_FILE");
    if(out_file == NULL){
        fprintf(stderr, "OUT_FILE not set\n");
        exit(1);
    }
    if (record_file == NULL){
        fprintf(stderr, "RECORD_FILE not set\n");
        exit(1);
    }
    seq_fp = fopen(out_file, "w");
    rec_fp = fopen(record_file, "w");
    return;
} 

void print_func(bool is_entry, char * file_name, char * func_name) {
    if (seq_fp == NULL || rec_fp == NULL){
        fprintf(stderr, "Ignoring functions that called before main\n");
        return;
    }
    pthread_mutex_lock(&lock);
    pair<string, string> func_pair = make_pair(string(file_name), string(func_name));
    if (func_set.find(func_pair) == func_set.end()) {
        fprintf(rec_fp, "%lu %s at %s\n", func_set.size(), func_name, file_name);
        func_set[func_pair] = func_set.size();
    }
    short func_id = func_set[func_pair];
    fwrite(&is_entry, sizeof(bool), 1, seq_fp);
    fwrite(&func_id, sizeof(short), 1, seq_fp);
    pthread_mutex_unlock(&lock);
}
}
