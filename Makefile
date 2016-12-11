all: sim

sim: cpu_sim.o op_def.o command.o
	g++ $^ -o $@ -Wall -lm -lreadline
#Macでは -lreadline の代わりに -ledit でもリンクできた


cpu_sim.o: cpu_sim.cpp
	g++ -c $^ -Wall
op_def.o: op_def.cpp
	g++ -c $^ -Wall
command.o: command.cpp
	g++ -c $^ -Wall
command.cpp: command.l
	flex -o $@ $^

clean:
	rm -f command.cpp *.o sim *.gch
