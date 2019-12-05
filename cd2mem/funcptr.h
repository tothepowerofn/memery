#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include <fstream>
#include <string>

std::string exec_command(const char* cmd);
bool mem2file (std::string const & filename, char const * data, size_t const bytes);
char* hex2mem(char* string);
bool is_func_ptr(char * mem, size_t sz);