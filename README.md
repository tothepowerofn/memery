# memery
*memey memes*

`memery` is a proof-of-concept tool for extracting recursive data structures
from static memory images. It identifies recursively linked structs in C-like
languages (such as singly or doubly linked lists, trees, etc.) and attempts
to 

This repository contains the code for the `memery` algorithm, as well as
setup code to dump currently running processes' memory using `gdb` and
a suite of small data structure tests.

## Usage

Enter into the `bin/` directory and run the command `sudo bash autodump.sh
<program_name>` where `<program_name>` is the name of a currently running
program (e.g. `linked_lists` from the test suite).

## Output

`memery` attempts to classify unique data structures (recursive, connected
structures with constant in-memory layout) and will identify whether they
are connected circularly, etc.
