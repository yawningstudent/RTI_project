#include <iostream>
#include "Fedamb.h"

using namespace std;
using namespace rti1516e;

// constructors //

Fedamb::Fedamb()
{
    // initialize all the variable values

    this->isAnnounced   = false;
    this->isReadyToRun  = false;

}

Fedamb::~Fedamb() throw()
{
}

// methods //

void Fedamb::synchronizationPointRegistrationSucceeded( const std::wstring& label)
    throw( FederateInternalError )
{
    wcout << L"Successfully registered sync point: " << label << endl;
}

void
Fedamb::synchronizationPointRegistrationFailed( const std::wstring& label,
                                                       SynchronizationPointFailureReason reason )
    throw( FederateInternalError )
{
    wcout << L"Failed to register sync point: " << label << endl;
}

void Fedamb::announceSynchronizationPoint( const std::wstring& label,
                                                  const VariableLengthData& tag )
    throw( FederateInternalError )
{
    wcout << L"Synchronization point announced: " << label << endl;
    if( label.compare(L"ReadyToRun") == 0 )
        this->isAnnounced = true;
}

void Fedamb::federationSynchronized( const std::wstring& label, FederateHandleSet const& failedToSyncSet)
    throw( FederateInternalError )
{
    wcout << L"Federation Synchronized: " << label << endl;
    if( label.compare(L"ReadyToRun") == 0 )
        this->isReadyToRun = true;
}

void Fedamb:: objectInstanceNameReservationSucceeded (
        std::wstring const & theObjectInstanceName)
        throw (
        FederateInternalError){
    if( theObjectInstanceName.compare(L"Robot1") == 0 )
        this->isReserved = true;
    wcout << L"Succesfully reserved objectInstanceName:" << theObjectInstanceName << endl;
}

void Fedamb:: objectInstanceNameReservationFailed (
        std::wstring const & theObjectInstanceName)
        throw (
        FederateInternalError){
    wcout << L"Failed to reserve objectInstanceName:" << theObjectInstanceName;
}

void Fedamb::discoverObjectInstance( ObjectInstanceHandle theObject,
                                            ObjectClassHandle theObjectClass,
                                            const std::wstring& theObjectName )
    throw( FederateInternalError )
{
    wcout << L"Discoverd Object: handle =" << theObject
         << L", ObjectClassHandle =" << theObjectClass
         << L", name =" << theObjectName << endl;
}

void Fedamb::discoverObjectInstance( ObjectInstanceHandle theObject,
                                            ObjectClassHandle theObjectClass,
                                            const std::wstring& theObjectName,
                                            FederateHandle producingFederate )
    throw( FederateInternalError )
{
    wcout << L"Discoverd Object: handle =" << theObject
         << L", ObjectClassHandle =" << theObjectClass
         << L", name =" << theObjectName
         << L", createdBy =" << producingFederate << endl;
}
