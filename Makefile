all:
	
	gcc main.c queue.c -lm -pthread
	sudo trace-cmd record -e sched_switch taskset 0x01 ./a.out
		

clear:
	rm -rf trace.dat
	rm -rf a.out 
	
