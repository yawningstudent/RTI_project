#include <iostream>
#include <fstream>
#include "RTI/RTI1516.h"
#include "RTI/encoding/BasicDataElements.h"
#include "RTI/encoding/HLAvariantRecord.h"
#include "RTI/encoding/DataElement.h"
#include "Fedamb.h"
#include "Federate.h"
#include <string>
#include <unistd.h>
#include <chrono>

using namespace rti1516e;
using namespace std::chrono;

// constructots //

Federate::Federate(wstring name){
    Name = name;
}

Federate::~Federate(){
if(this->fedamb)
delete this->fedamb;
}

void Federate::runFederate( std::wstring federateName, char* route_file_name)
{
    int destXa, destYa, destXb, destYb;
    char first_coordinate;
    ifstream route;
    ofstream movement_logs;
    movement_logs.open("/home/andrey/robots/robot1/movement_logs.txt");
    prog_logs.open("/home/andrey/robots/robot1/prog_logs.txt");
    route.open(route_file_name);
    if (route.is_open())
     prog_logs << "input file opened" << endl;
     else
     {
     prog_logs << "input file didn't open" << endl;
     terminate();
     }
    route >> X >> Y;
    route >> first_coordinate;
    if(first_coordinate == 'X')
    {
        route >> destXa >> destYb;
        destYa = Y;
        destXb = destXa;
    }
    if(first_coordinate == 'Y'){
        route >> destYa >> destXb;
        destXa = X;
        destYb = destYa;
    }

        prog_logs << L"Initial values:" << endl;
        prog_logs << "Name:" << Name << endl;
        prog_logs << "X:" << X << endl;
        prog_logs << "Y:" << Y << endl;
        prog_logs << first_coordinate << endl;
        prog_logs << destXa << endl;
        prog_logs << destYa << endl;
        prog_logs << destXb << endl;
        prog_logs << destYb << endl;

        // create the RTIambassador //

        this->rtiamb = rti1516e::RTIambassadorFactory().createRTIambassador().release();
        prog_logs << L"Created Fedamb" << endl;

        // connect to the RTI with rti1516e //

        this->fedamb = new Fedamb();
        try
        {
            rtiamb->connect(*this->fedamb, HLA_EVOKED);
        }
        catch( ConnectionFailed& connectionFailed )
        {
            prog_logs << L"Connection failed: " << connectionFailed.what() << endl;

        }
        catch( InvalidLocalSettingsDesignator& settings )
        {
            prog_logs << L"Connection failed, InvalidLocalSettingsDesignator: " << settings.what() << endl;
        }
        catch( UnsupportedCallbackModel& callbackModel )
        {
            prog_logs << L"Connection failed, UnsupportedCallbackModel: " << callbackModel.what() << endl;
        }
        catch( AlreadyConnected& connected )
        {
            prog_logs << L"Connection failed, AlreadyConnected: " << connected.what() << endl;
        }
        catch( RTIinternalError& error )
        {
            prog_logs << L"Connection failed, Generic Error: " << error.what() << endl;
        }

        // create and join to the federation //

        // create
        //some other federate may have already created the federation,
        //so we'll just try and join it

        try
        {
            wstring fom = L"/home/andrey/RTI_project/Federate1/fom.xml";
            rtiamb->createFederationExecution( L"Federation", fom );
            prog_logs << L"Created Federation" << endl;
        }
        catch ( FederationExecutionAlreadyExists& exists )
        {
            prog_logs << L"Didn't create federation, it already existed" << endl;
        }
        catch ( Exception& e )
        {
            prog_logs << L"Something else happened: " << e.what() << endl;
        }

        // join the federation //

        rtiamb->joinFederationExecution( federateName, L"Federate", L"Federation");
        prog_logs << L"Joined Federation as " << federateName << endl;

        // synchronization //

        VariableLengthData tag ((void*)"", 1 );
        rtiamb->registerFederationSynchronizationPoint(READY_TO_RUN , tag );
        while( fedamb->SyncisAnnounced == false )
            {
                rtiamb->evokeMultipleCallbacks( 0.1, 1.0 );
            }

        waitForUser();

        rtiamb->synchronizationPointAchieved( READY_TO_RUN );
            prog_logs << L"Achieved sync point: " << READY_TO_RUN << L", waiting for federation..." << endl;
            while( fedamb->FedisReadyToRun == false )
            {
                rtiamb->evokeMultipleCallbacks( 0.1, 1.0 );
            }

            // preparations for discovering objects //

            try {
                 robotClassHandle = rtiamb->getObjectClassHandle(L"HLAobjectRoot.Robot");
                 prog_logs << L"Got robotClassHandle" << endl;
            } catch (Exception& e){
                prog_logs << L"Didn't get robotClassHandle: " << e.what() << endl;
            }

            try {
            nameAttrHandle = rtiamb->getAttributeHandle(robotClassHandle, L"Name");
            prog_logs << L"Got nameAttrHandle" << endl;
            } catch (Exception& e){
                prog_logs << L"Didn't get nameAttrHandle: " << e.what() << endl;
            }

            try {
            xAttrHandle = rtiamb->getAttributeHandle(robotClassHandle, L"X");
            prog_logs << L"Got xAttrHandle" << endl;
            } catch (Exception& e){
                prog_logs << L"Didn't get xAttrHandle: " << e.what() << endl;
            }

            try {
            yAttrHandle = rtiamb->getAttributeHandle(robotClassHandle, L"Y");
            prog_logs << L"Got yAttrHandle" << endl;
            } catch (Exception& e){
                prog_logs << L"Didn't get yAttrHandle: " << e.what() << endl;
            }

//            try {
//            directionAttrHandle = rtiamb->getAttributeHandle(robotClassHandle, L"Direction");
//            prog_logs << L"Got directionAttrHandle" << endl;
//            } catch (Exception& e){
//                prog_logs << L"Didn't get directionAttrHandle: " << e.what() << endl;
//            }

            attrHandleSet.insert(nameAttrHandle);
            attrHandleSet.insert(xAttrHandle);
            attrHandleSet.insert(yAttrHandle);
//            attrHandleSet.insert(directionAttrHandle);

            try {
            rtiamb->publishObjectClassAttributes(robotClassHandle, attrHandleSet);
            prog_logs << L"Published" << endl;
            } catch (Exception& e){
                prog_logs << L"Can't publish attributes: " << e.what() << endl;
            }
            try {
            rtiamb->subscribeObjectClassAttributes(robotClassHandle, attrHandleSet);
            prog_logs << L"Subscribed" << endl;
            } catch (Exception& e){
                prog_logs << L"Can't subscribe attributes: " << e.what() << endl;
            }

            // discovering objects //

            wstring NameToReserve = L"Robot2";


            rtiamb->reserveObjectInstanceName(NameToReserve);
            prog_logs << L"Reserved ObjectInstanceName: " << NameToReserve << endl;
            while( fedamb->NameisReserved == false )
                {
                    rtiamb->evokeMultipleCallbacks( 0.1, 1.0 );
                }

            try {
            robotInstanceHandle = rtiamb->registerObjectInstance(robotClassHandle, NameToReserve);
            prog_logs << L"Registered robotInstanceHandle" << endl;
            } catch (Exception& e){
                prog_logs << L"Didn't registeredObjectInstance: " << e.what() << endl;
            }

            // release initial values //
            updatingAttributesValues(robotInstanceHandle);
//            auto start = steady_clock::now();
//            seconds s {1};
//            while (steady_clock::now() < start + s){}
//            start = steady_clock::now();
            sleep(1);
            // main simulation //

            movement(destXa, destYa);
            movement_logs << "Current position: " << X << "," << Y << ";" << endl;
            movement(destXb, destYb);
            movement_logs << "Current position: " << X << "," << Y << ";" << endl;
            movement_logs.close();
            route.close();

        // delete object instance //
        // Note: we need to syncronize federation, because some federates could still reflect our attr values //

            rtiamb->registerFederationSynchronizationPoint(READY_TO_DELETE , tag );
            while( fedamb->SyncisAnnounced == false )
                {
                    rtiamb->evokeMultipleCallbacks( 0.1, 1.0 );
                }

            rtiamb->synchronizationPointAchieved( READY_TO_DELETE );
                prog_logs << L"Achieved sync point: " << READY_TO_DELETE << L", waiting for federation..." << endl;
                while( fedamb->FedisReadyToDel == false )
                {
                    rtiamb->evokeMultipleCallbacks( 0.1, 1.0 );
                }

        VariableLengthData del_tag ((void*)"Del",  4);
        rtiamb->deleteObjectInstance( robotInstanceHandle, del_tag );
        prog_logs << L"Deleted object instance, handle:"<< robotInstanceHandle << endl;

        // resign from the federation //

        rtiamb->resignFederationExecution( NO_ACTION );
        prog_logs << L"Resigned from Federation" << endl;

       // try and destroy the federation //

        try
        {
            rtiamb->destroyFederationExecution( L"Federation" );
            prog_logs << L"Destroyed Federation" << endl;
        }
        catch( FederationExecutionDoesNotExist& dne )
        {
            prog_logs << L"Federation doesn't exist" << endl;
        }
        catch( FederatesCurrentlyJoined& fcj )
        {
            prog_logs << L"Didn't destroy federation, federates still joined" << endl;
        }


       // disconnect from the RTI //

        this->rtiamb->disconnect();

       // clean up //

        delete this->rtiamb;
        prog_logs.close();
}

// other methods //
void Federate::updatingAttributesValues(ObjectInstanceHandle objectHandle)
{
    HLAunicodeString nameEncoder;
    HLAinteger32BE XEncoder, YEncoder;

    AttributeHandleValueMap attributes;
    VariableLengthData userSuppliedTag;

    nameEncoder.set(Name);
    XEncoder.set(X);
    YEncoder.set(Y);
    attributes[nameAttrHandle] = nameEncoder.encode();
    attributes[xAttrHandle] = XEncoder.encode();
    attributes[yAttrHandle] = YEncoder.encode();

    try {
        rtiamb->updateAttributeValues(objectHandle, attributes, userSuppliedTag);
        prog_logs << L"Attributes successfuly updated" << endl;
    } catch (Exception& e){
        prog_logs << L"Can't update attributes: " << e.what() << endl;
    }
}


void Federate::waitForUser()
{
    wcout << L">>> Run other federates, when press Enter to Continue <<<" << endl;
    string line;
    getline( cin, line );
}

void Federate::synchronization(){
}

void Federate::DirectionChoose(int destX, int destY){
    if(Y == destY){
        if(destX - X > 0) Direction = 4;
        else Direction = 3;
    }
    else if (X == destX){
        if(destY - Y > 0) Direction = 1;
        else Direction = 2;
    }
    else {
        prog_logs << "Incorrect route, failed at point:" << X << "; " << Y << endl;
    }
}

void Federate::movement(int destX, int destY){
    DirectionChoose(destX, destY);
    if (Direction == 1){
        while(Y!=destY){
            Y++;
            updatingAttributesValues(robotInstanceHandle);
        }
    }
    else if (Direction == 2){
        while(Y!=destY){
            Y--;
            updatingAttributesValues(robotInstanceHandle);
        }
    }
    else if (Direction == 3){
        while(X!=destX){
            X--;
            updatingAttributesValues(robotInstanceHandle);
        }
    }
    else if (Direction == 4){
        while(X!=destX){
            X++;
            updatingAttributesValues(robotInstanceHandle);
        }
    }
    else {
        prog_logs << "Incorrect Direction" << endl;
    }
}

