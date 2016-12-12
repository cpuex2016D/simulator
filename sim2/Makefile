all: sim

sim: sim.o examine_op.o exec_op.o prnt_op.o
	gcc -Wall sim.o examine_op.o exec_op.o prnt_op.o -o sim -lm

sim.o: sim.c
	gcc -Wall -c sim.c

examine_op.o: examine_op.c prnt_op.h exec_op.h
	gcc -Wall -c examine_op.c

exec_op.o: exec_op.c
	gcc -Wall -c exec_op.c -lm

prnt_op_op.o: prnt_op.c
	gcc -Wall -c prnt_op.c


clean:
	rm -f *.o

clear:
	rm -f *.o sim
