sim: sim.o simprepare.o parent_core.o child_core.o examine_op.o exec_op.o prnt_op.o prnt_state_stats_memory.o
	g++ -Wall $^ -o $@ -lm -lpthread 

sim.o: sim.cpp sim.h simprepare.h parent_core.h
	g++ -Wall -c sim.cpp

simprepare.o: simprepare.cpp sim.h
	g++ -Wall -c $<

parent_core.o: parent_core.cpp sim.h examine_op.h prnt_state_stats_memory.h
	g++ -Wall -c $<

child_core.o: child_core.cpp sim.h examine_op.h prnt_state_stats_memory.h types.h
	g++ -Wall -c $<

examine_op.o: examine_op.cpp sim.h exec_op.h prnt_op.h
	g++ -Wall -c $<

exec_op.o: exec_op.cpp sim.h examine_op.h types.h child_core.h
	g++ -Wall -c $<

prnt_op.o: prnt_op.cpp sim.h
	g++ -Wall -c $<

prnt_state_stats_memory.o: prnt_state_stats_memory.cpp sim.h
	g++ -Wall -c $<

clean:
	rm -f *.o

clear:
	rm -f *.o sim
