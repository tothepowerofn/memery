#include "exploit.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <exploit.h>
#include <iostream> 
#include <list> 
#include <vector> 
#include <iterator> 
using namespace std; 

int main(){
    list <int> gqlist1;
    for (int i = 0; i < 20; ++i) 
    { 
        gqlist1.push_back(i * 2); 
    }
    exploit_loop();
}

