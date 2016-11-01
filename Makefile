sim: cpu_sim.o op_def.o
	gcc cpu_sim.o op_def.o -o sim

step: cpu_sim_by_step.o op_def.o
	gcc cpu_sim_by_step.o op_def.o -o ssim

main.o: cpu_sim.c
	gcc -c cpu_sim.c
op_def.o: op_def.c
	gcc -c op_def.c

cpu_sim_by_step.o: cpu_sim_by_step.c
	gcc -c cpu_sim_by_step.c

clean:
	rm -f *.o sim *.gch ssim
