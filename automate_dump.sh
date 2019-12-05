#!/bin/bash

# check for command lin args
#

process_to_get=$1
process_id=`pidof $process_to_get`
echo $process_id

addrs=`python3 cd2mem/dump_addrs.py $process_id`
addr_list=( $addrs )
addr_start=${addr_list[0]}
addr_end=${addr_list[1]}
echo $addr_start
echo $addr_end
rm -f gdb_commands
echo -e "dump binary memory /tmp/memery_dump.bin $addr_start $addr_end\nquit" >> /tmp/gdb_commands
gdb -p $process_id --command=/tmp/gdb_commands 
./bin/map_pointers /tmp/memery_dump.bin $addr_start $addr_end
