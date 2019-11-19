#!/usr/bin/env python3

from sys import argv, exit


def print_usage():
	"""Print the usage string if script was used improperly"""
	print('Usage: \
		\t$ {} <pid>'.format(argv[0]))
	exit(1)


def read_write_heap(pid):
	"""Find @read_str in the heap of @pid and replace it with @write_str"""
	try:
		maps_file = open("/proc/{}/maps".format(pid), 'r')
	except IOError as e:
		print("Can't open file /proc/{}/maps: IOError: {}".format(pid, e))
		exit(1)
	heap_info = None
	for line in maps_file:
		if 'heap' in line:
			heap_info = line.split()
	maps_file.close()
	if 'heap' is None:
		print('No heap found!')
		exit(1)
	addr = heap_info[0].split('-')
	heap_start = addr[0]
	heap_end = addr[1]
	print(heap_start)
	print(heap_end)
	return heap_start, heap_end


if (len(argv) == 2):
	pid = argv[1]
	read_write_heap(pid)
else:
	print_usage()