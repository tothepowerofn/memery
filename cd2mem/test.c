#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int a = 1;
    int b = 2;
    int* heapvar = (int*)malloc(100*sizeof(int));
    printf("%p\n", heapvar);
    heapvar[3] = 5;
    sleep(100000000);
}
