#include "Federate1.h"
#include "Fed1amb.h"
#include <string>
#include <iostream>

int main (int argc, char* argv[]){

//create and run the federate

Federate1 *federate;
federate = new Federate1();
federate->runFederate (L"Robot1");

//clean
delete federate;

return 0;

}
