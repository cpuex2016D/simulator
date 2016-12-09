all: sim

sim: cpu_sim.o op_def.o
	gcc cpu_sim.o op_def.o -o sim -Wall -lm


main.o: cpu_sim.c op_def.h
	gcc -c cpu_sim.c -Wall
op_def.o: op_def.c
	gcc -c op_def.c -Wall

clean:
	rm -f *.o sim *.gch
