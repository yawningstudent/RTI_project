#include <iostream>
#include "RTI/RTI1516.h"
#include "Fedamb.h"
#include "Federate.h"
#include <string>

using namespace rti1516e;

// constructots //

Federate::Federate(){
}

Federate::~Federate(){
if(this->fedamb)
delete this->fedamb;
}

void Federate::runFederate( std::wstring federateName )
{

        // create the RTIambassador //

//        vector<wstring> configuration;
//        configuration.push_back( L"<p>=<thread>");
        this->rtiamb = rti1516e::RTIambassadorFactory().createRTIambassador().release();
        wcout << L"Created Fedamb" << endl;

        // connect to the RTI with rti1516e //

        this->fedamb = new Fedamb();
        try
        {
            rtiamb->connect(*this->fedamb, HLA_EVOKED);
        }
        catch( ConnectionFailed& connectionFailed )
        {
            wcout << L"Connection failed: " << connectionFailed.what() << endl;

        }
        catch( InvalidLocalSettingsDesignator& settings )
        {
            wcout << L"Connection failed, InvalidLocalSettingsDesignator: " << settings.what() << endl;
        }
        catch( UnsupportedCallbackModel& callbackModel )
        {
            wcout << L"Connection failed, UnsupportedCallbackModel: " << callbackModel.what() << endl;
        }
        catch( AlreadyConnected& connected )
        {
            wcout << L"Connection failed, AlreadyConnected: " << connected.what() << endl;
        }
        catch( RTIinternalError& error )
        {
            wcout << L"Connection failed, Generic Error: " << error.what() << endl;
        }

        // create and join to the federation //

        // create
        //some other federate may have already created the federation,
        //so we'll just try and join it

        try
        {
            wstring fom = L"/home/andrey/RTI_project/Federate1/fom.xml";
            rtiamb->createFederationExecution( L"Federation", fom );
            wcout << L"Created Federation" << endl;
        }
        catch ( FederationExecutionAlreadyExists& exists )
        {
            wcout << L"Didn't create federation, it already existed" << endl;
        }
        catch ( Exception& e )
        {
            wcout << L"Something else happened: " << e.what() << endl;
        }

        // join the federation //

        rtiamb->joinFederationExecution( federateName, L"Federate", L"Federation");
        wcout << L"Joined Federation as " << federateName << endl;

        // synchronization //
        VariableLengthData tag ((void*)"", 1 );
        rtiamb->registerFederationSynchronizationPoint( READY_TO_RUN, tag );
        while( fedamb->isAnnounced == false )
            {
                rtiamb->evokeMultipleCallbacks( 0.1, 1.0 );
            }

        waitForUser();

        rtiamb->synchronizationPointAchieved( READY_TO_RUN );
            wcout << L"Achieved sync point: " << READY_TO_RUN << L", waiting for federation..." << endl;
            while( fedamb->isReadyToRun == false )
            {
                rtiamb->evokeMultipleCallbacks( 0.1, 1.0 );
            }

            // preparations for discovering objects //

            try {
                 robotClassHandle = rtiamb->getObjectClassHandle(L"Robot");
                 wcout << L"Got robotClassHandle" << endl;
            } catch (Exception& e){
                wcout << L"Didn't get robotClassHandle: " << e.what() << endl;
            }

            try {
            nameAttrHandle = rtiamb->getAttributeHandle(robotClassHandle, L"Name");
            wcout << L"Got nameAttrHandle" << endl;
            } catch (Exception& e){
                wcout << L"Didn't get nameAttrHandle: " << e.what() << endl;
            }

            try {
            xAttrHandle = rtiamb->getAttributeHandle(robotClassHandle, L"X");
            wcout << L"Got xAttrHandle" << endl;
            } catch (Exception& e){
                wcout << L"Didn't get xAttrHandle: " << e.what() << endl;
            }

            try {
            yAttrHandle = rtiamb->getAttributeHandle(robotClassHandle, L"Y");
            wcout << L"Got yAttrHandle" << endl;
            } catch (Exception& e){
                wcout << L"Didn't get yAttrHandle: " << e.what() << endl;
            }

            try {
            directionAttrHandle = rtiamb->getAttributeHandle(robotClassHandle, L"Direction");
            wcout << L"Got directionAttrHandle" << endl;
            } catch (Exception& e){
                wcout << L"Didn't get directionAttrHandle: " << e.what() << endl;
            }

            attrHandleSet.insert(nameAttrHandle);
            attrHandleSet.insert(xAttrHandle);
            attrHandleSet.insert(yAttrHandle);
            attrHandleSet.insert(directionAttrHandle);

            try {
            rtiamb->publishObjectClassAttributes(robotClassHandle, attrHandleSet);
            wcout << L"Published" << endl;
            } catch (Exception& e){
                wcout << L"Can't publish attributes: " << e.what() << endl;
            }
            try {
            rtiamb->subscribeObjectClassAttributes(robotClassHandle, attrHandleSet);
            wcout << L"Subscribed" << endl;
            } catch (Exception& e){
                wcout << L"Can't subscribe attributes: " << e.what() << endl;
            }

            wstring NameToReserve = L"Robot1";


                rtiamb->reserveObjectInstanceName(NameToReserve);
                wcout << L"Reserved ObjectInstanceName: " << NameToReserve << endl;
                while( fedamb->isReserved == false )
                    {
                        rtiamb->evokeMultipleCallbacks( 0.1, 1.0 );
                    }

            try {
            robotInstanceHandle = rtiamb->registerObjectInstance(robotClassHandle, NameToReserve);
            wcout << L"Registered robotInstanceHandle" << endl;
            } catch (Exception& e){
                wcout << L"Didn't registeredObjectInstance: " << e.what() << endl;
            }

        

        // resign from the federation //

        rtiamb->resignFederationExecution( NO_ACTION );
        wcout << L"Resigned from Federation" << endl;

       // try and destroy the federation //

        try
        {
            rtiamb->destroyFederationExecution( L"Federation" );
            wcout << L"Destroyed Federation" << endl;
        }
        catch( FederationExecutionDoesNotExist& dne )
        {
            wcout << L"Federation doesn't exist" << endl;
        }
        catch( FederatesCurrentlyJoined& fcj )
        {
            wcout << L"Didn't destroy federation, federates still joined" << endl;
        }


       // disconnect from the RTI //

        this->rtiamb->disconnect();

       // clean up //

        delete this->rtiamb;

}

// other methods //

void Federate::waitForUser()
{
    wcout << L">>> Press Enter to Continue <<<" << endl;
    string line;
    getline( cin, line );
}
