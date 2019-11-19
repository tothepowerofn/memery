#include <stdio.h>

int main(){
    int a = 1;
    int b = 2;
    int* heapvar = (int*)malloc(sizeof(int));
    printf("%p\n", heapvar);
    heapvar[3] = 5;
    sleep(100);
}