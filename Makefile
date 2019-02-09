all: main

main: main.o Federate1.o Fed1amb.o
	g++ main.o Federate1.o Fed1amb.o /home/andrey/rti/lib/librti1516e.so /home/andrey/rti/lib/libOpenRTI.so /home/andrey/rti/lib/libFedTime.so /home/andrey/rti/lib/libfedtime1516e.so -o main

main.o: main.cpp Federate1.h
	g++ -c Federate1.h main.cpp

Federate1.o: Federate1.h Fed1amb.h Federate1.cpp
	g++ -c Federate1.h Fed1amb.h Federate1.cpp 
Fed1amb.o: Fed1amb.h Fed1amb.cpp
	g++ -c Fed1amb.h Fed1amb.cpp
