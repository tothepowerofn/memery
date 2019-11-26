
process_to_get=$1
process_id=`pidof $process_to_get`
echo $process_id

addrs=`python3 dump_addrs.py $process_id`
addr_list=( $addrs )
addr_start=${addr_list[0]}
addr_end=${addr_list[1]}
echo $addr_start
echo $addr_end
rm gdb_commands
echo "dump binary memory ~/dump.bin $addr_start $addr_end" >> gdb_commands
gdb -p $process_id --command=gdb_commands