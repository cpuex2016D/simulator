all: sim ssim

sim: cpu_sim.o op_def.o
	gcc cpu_sim.o op_def.o -o sim -Wall -lm

ssim: cpu_sim_by_step.o op_def.o
	gcc cpu_sim_by_step.o op_def.o -o ssim -Wall -lm

step: ssim


main.o: cpu_sim.c op_def.h
	gcc -c cpu_sim.c -Wall
op_def.o: op_def.c
	gcc -c op_def.c -Wall

cpu_sim_by_step.o: cpu_sim_by_step.c
	gcc -c cpu_sim_by_step.c -Wall

clean:
	rm -f *.o sim *.gch ssim
