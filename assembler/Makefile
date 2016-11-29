ass: ass_main.o opdef_as.o
	g++ -o ass ass_main.o opdef_as.o

ass_main.o: ass_main.cpp
	g++ -c -o ass_main.o ass_main.cpp -Wall
	
opdef_as.o: opdef_as.cpp
	g++ -std=c++11 -c -o opdef_as.o opdef_as.cpp -Wall

clean:
	rm -f *.o

clear:
	rm -f *.o ass

