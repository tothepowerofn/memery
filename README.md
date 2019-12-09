# memery
*memey memes*


`memery` is a proof-of-concept tool for extracting information about the high-level program constructs of an executable with a memory read vulnerability by following chains of pointers. The current implememntation reliably detects both singly- and doubly-linked data structures, and detects data types of interest within those structures, such as function pointers and character string pointers.

This repository contains the code for the `memery` algorithm, as well as multiple test programs that use common types of linked data structures that can be used to evaluate `memery`.

## Usage

First, ensure that you have all the tools and packages to run `memery`. You will need `Python3` (aliased as `python3` in your shell) and `NDISASM` (The Netwide Disassembler, often distributed under package name `nasm`).

Next, enter the root directory of the project and compile `memery` and its tests by typing `make`. Once compilation is complete, 

Then, enter into the `bin/tests` directory and run the executable corresponding to the data structure on which you wish to test `memery`. While allowing the test executable to continue running, start `memery`, located in `bin` (by simply typing `./memery` when in the folder `bin`). `memery` will begin siphoning data from the victimized test program, and output information and analysis about the linked data structures it detects.