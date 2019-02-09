#include <iostream>
#include "RTI/RTI1516.h"
#include "Fed1amb.h"
#include "Federate1.h"
#include <string>

using namespace rti1516e;

// constructots //

Federate1::Federate1(){
}

Federate1::~Federate1(){
if(this->fedamb)
delete this->fedamb;
}

void Federate1::runFederate( std::wstring federateName )
{

        // create the RTIambassador //

//        vector<wstring> configuration;
//        configuration.push_back( L"<p>=<thread>");
        this->rtiamb = rti1516e::RTIambassadorFactory().createRTIambassador().release();
        wcout << L"Created Fedamb" << endl;

        // connect to the RTI with rti1516e //

        this->fedamb = new Fed1amb();
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

//         resign from the federation //

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
