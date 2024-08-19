#include<stdio.h>

void a(){
}

void b(){
}

void c(){
}

int main(int argc, char *argv[]) {
    for (int i=1; i<argc; i++){
        if (argv[i][0] == 'a'){
            a();
        } else if (argv[i][0] == 'b'){
            b();
        } else if (argv[i][0] == 'c'){
            c();
        }
    }

    return 0;
}