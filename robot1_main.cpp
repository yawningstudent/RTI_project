#include "Federate.h"
#include "Fedamb.h"
#include <string>
#include <iostream>

int main (int argc, char* argv[]){

//create and run the federate

Federate *federate;
federate = new Federate();
federate->runFederate (L"Robot1");

//clean
delete federate;

return 0;

}
