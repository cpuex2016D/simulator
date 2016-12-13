all: sim

sim: sim.o examine_op.o exec_op.o prnt_op.o
	g++ -Wall sim.o examine_op.o exec_op.o prnt_op.o -o sim -lm

sim.o: sim.cpp
	g++ -Wall -c sim.cpp

examine_op.o: examine_op.cpp prnt_op.h exec_op.h
	g++ -Wall -c examine_op.cpp

exec_op.o: exec_op.cpp
	g++ -Wall -c exec_op.cpp -lm

prnt_op_op.o: prnt_op.cpp
	g++ -Wall -c prnt_op.cpp


clean:
	rm -f *.o

clear:
	rm -f *.o sim
