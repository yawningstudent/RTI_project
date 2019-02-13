all: main

main: robot1_main.o Federate.o Fedamb.o
	g++ robot1_main.o Federate.o Fedamb.o /home/andrey/rti/lib/librti1516e.so /home/andrey/rti/lib/libOpenRTI.so /home/andrey/rti/lib/libFedTime.so /home/andrey/rti/lib/libfedtime1516e.so -o robot1

robot1_main.o: robot1_main.cpp Federate.h
	g++ -c -w Federate.h robot1_main.cpp

Federate.o: Federate.h Fedamb.h Federate.cpp
	g++ -c -w Federate.h Fedamb.h Federate.cpp 
Fedamb.o: Fedamb.h Fedamb.cpp
	g++ -c -w Fedamb.h Fedamb.cpp
clear:
	rm -f robot1_main.o Fedamb.o Federate.o Federate.h.gch Fedamb.h.gch
