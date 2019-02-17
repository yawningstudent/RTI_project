#include "Federate.h"
#include "Fedamb.h"
#include <string>
#include <fstream>
#include <iostream>

int main (int argc, char* argv[]){

    //create and run the federate

    Federate *federate;
    federate = new Federate(L"Robot2");
    federate->runFederate (L"Robot2", argv[1]);
    //clean
    delete federate;

    return 0;

}
