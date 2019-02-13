#include "Federate.h"
#include "Fedamb.h"
#include <string>
#include <iostream>

int main (int argc, char* argv[]){

    int X = 0, Y = 0, Direction = 2;

//create and run the federate

Federate *federate;
federate = new Federate(X, Y, Direction, L"Robot2");
federate->runFederate (L"Robot2");

//clean
delete federate;

return 0;

}
