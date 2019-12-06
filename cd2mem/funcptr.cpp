#include "funcptr.h"
using namespace std;

//https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
//Executes the cmd specified in cmd and stores the result in a string
std::string exec_command(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}


//https://stackoverflow.com/questions/3408706/hexadecimal-string-to-byte-array-in-c
//Turns a string of hex characters into a char* array. This function is for debugging purposes
char* hex2mem(char* string) {

    if(string == NULL) 
       return NULL;

    size_t slength = strlen(string);
    if((slength % 2) != 0) // must be even
       return NULL;

    size_t dlength = slength / 2;

    uint8_t* data = (uint8_t*)malloc(dlength);
    memset(data, 0, dlength);

    size_t index = 0;
    while (index < slength) {
        char c = string[index];
        int value = 0;
        if(c >= '0' && c <= '9')
          value = (c - '0');
        else if (c >= 'A' && c <= 'F') 
          value = (10 + (c - 'A'));
        else if (c >= 'a' && c <= 'f')
          value = (10 + (c - 'a'));
        else {
          free(data);
          return NULL;
        }

        data[(index/2)] += value << (((index + 1) % 2) * 4);

        index++;
    }

    return (char*)data;
}

//https://stackoverflow.com/questions/22530206/write-raw-memory-to-file
//Writes raw memory specified by *data to filename in the current working directory
//Used to pass the memory to python script for classification (func/nonfunc)
bool mem2file (std::string const & filename, char const * data, size_t const bytes)
{
  std::ofstream b_stream(filename.c_str(), 
    std::fstream::out | std::fstream::binary);
  if (b_stream)
  {
    b_stream.write(data, bytes);
    return (b_stream.good());
  }
  return false;
}

//Determines if a specified region of memory is a function by exporting the memory
//to a file, calling a python script that uses ndisasm to look at the assembly to
//see if the first instructions follow conventions, and then outputting that result
int is_func_ptr(char* mem, size_t sz){
    mem2file("testfunc", mem, sz); //NOTE: we always save the memory as file named testfunc in the bin folder
    return stoi(exec_command("python3 ../cd2mem/funcptrclassify.py"));
}
