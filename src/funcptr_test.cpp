#include "funcptr.h"
using namespace std;

int main(){
    //This is a test hex string of a valid x86-64 function
    char* opstring = "554889E54883EC20897DEC488975E0BF18000000E837FEFFFF488945F8488B45F88B55EC8910488B45F8488B55E04889500848837DE0007439488B45E0488B40104885C07420488B45E0488B4010488B55F848895008488B45E0488B5010488B45F848895010488B45E0488B55F848895010488B45F8C9C3";
    //Convert the string of hex into actual bytes
    char* data = hex2mem(opstring);
    //consider 640 bytes of the data in classification. no reason for this number really...
    size_t sz = 640;
    //outputs 1 if the data was the start of a valid x86 function, 0 if not
    cout << is_func_ptr(data, sz) << endl;
}