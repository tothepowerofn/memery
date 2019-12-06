import subprocess
import re

#runs command in the terminal returns output as list of lines
def run_command(command):
    p = subprocess.Popen(command,
                         stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT)
    return iter(p.stdout.readline, b'')

#Gets the assembly op from a line of ndisasm output
def get_op_from_line(line):
    return line.decode(encoding).split()[2] #ops are always the second in the list

#Gets the sequence of ops from the output of ndisasm 
def get_op_seq(lines):
    ops = []
    for line in lines:
        ops.append(get_op_from_line(line))
    return ops

#Checks to see if the ops from ndisasm match the desired convention specified by seq
def matches_op_seq(ops, seq):
    for i in range(len(seq)):
        if ops[i] != seq[i]:
            return 0
    return 1

command = "ndisasm -b64 -o7c00h -s7c3eh ../bin/testfunc" #looks for the function binary named testfunc
encoding = 'utf-8'
start_op_seq = "push mov sub".split() #this is the sequence of ops we're looking for

lines = run_command(command.split()) 
print(matches_op_seq(get_op_seq(lines),start_op_seq)) #print the output of the check so that our C++ can use!