#include <iostream>
#include "Fedamb.h"

using namespace std;
using namespace rti1516e;

// constructors //

Fedamb::Fedamb()
{
    // initialize all the variable values
    this->callback_logs.open("/home/andrey/robots/robot1/callback_logs.txt");
    this->SyncisAnnounced   = false;
    this->FedisReadyToRun  = false;
    this->FedisReadyToDel  = false;
    this->NameisReserved  = false;

}

Fedamb::~Fedamb() throw()
{
}

// methods //

void Fedamb::synchronizationPointRegistrationSucceeded( const std::wstring& label)
    throw( FederateInternalError )
{
    callback_logs << L"Successfully registered sync point: " << label << endl;
}

void
Fedamb::synchronizationPointRegistrationFailed( const std::wstring& label,
                                                       SynchronizationPointFailureReason reason )
    throw( FederateInternalError )
{
    callback_logs << L"Failed to register sync point: " << label << endl;
}

void Fedamb::announceSynchronizationPoint( const std::wstring& label,
                                                  const VariableLengthData& tag )
    throw( FederateInternalError )
{
    callback_logs << L"Synchronization point announced: " << label << endl;
    if( label.compare(L"ReadyToRun") == 0 )
        this->SyncisAnnounced = true;
    if( label.compare(L"ReadyToDelete") == 0 )
        this->SyncisAnnounced = true;
}

void Fedamb::federationSynchronized( const std::wstring& label, FederateHandleSet const& failedToSyncSet)
    throw( FederateInternalError )
{
    callback_logs << L"Federation Synchronized: " << label << endl;
    if( label.compare(L"ReadyToRun") == 0 )
        this->FedisReadyToRun = true;
    if( label.compare(L"ReadyToDelete") == 0 )
        this->FedisReadyToDel= true;
        this->SyncisAnnounced = false;
}

void Fedamb:: objectInstanceNameReservationSucceeded (
        std::wstring const & theObjectInstanceName)
        throw (
        FederateInternalError){
    if( theObjectInstanceName.compare(L"Robot2") == 0 )
        this->NameisReserved = true;
    callback_logs << L"Succesfully reserved objectInstanceName:" << theObjectInstanceName << endl;
}

void Fedamb:: objectInstanceNameReservationFailed (
        std::wstring const & theObjectInstanceName)
        throw (
        FederateInternalError){
    callback_logs << L"Failed to reserve objectInstanceName:" << theObjectInstanceName;
    terminate();
}

void Fedamb::discoverObjectInstance( ObjectInstanceHandle theObject,
                                            ObjectClassHandle theObjectClass,
                                            const std::wstring& theObjectName )
    throw( FederateInternalError )
{
    callback_logs << L"Discoverd Object: handle =" << theObject
         << L", ObjectClassHandle =" << theObjectClass
         << L", name =" << theObjectName << endl;
}

void Fedamb::discoverObjectInstance( ObjectInstanceHandle theObject,
                                            ObjectClassHandle theObjectClass,
                                            const std::wstring& theObjectName,
                                            FederateHandle producingFederate )
    throw( FederateInternalError )
{
    callback_logs << L"Discoverd Object: handle =" << theObject
         << L", ObjectClassHandle =" << theObjectClass
         << L", name =" << theObjectName
         << L", createdBy =" << producingFederate << endl;
}

void Fedamb::reflectAttributeValues( ObjectInstanceHandle theObject,
                                            const AttributeHandleValueMap& theAttributes,
                                            const VariableLengthData& tag,
                                            OrderType sentOrder,
                                            TransportationType theType,
                                            SupplementalReflectInfo theReflectInfo )
        throw( FederateInternalError )
{
    callback_logs << L"Reflection Received:";

    // print the handle

    callback_logs << L" object =" << theObject;

    // print the attribute information

    callback_logs << ", attributeCount =" << theAttributes.size() << endl;
    AttributeHandleValueMap::const_iterator iterator;
    for( iterator = theAttributes.begin(); iterator != theAttributes.end(); iterator++ )
    {
        // print the attribute handle

        callback_logs << L"\tattrHandle =" << (*iterator).first
              << L", attrSize =" << (*iterator).second.size() << endl;
    }
}
    void Fedamb::removeObjectInstance( ObjectInstanceHandle theObject,
                                              const VariableLengthData& tag,
                                              OrderType sentOrder,
                                              SupplementalRemoveInfo theRemoveInfo )
            throw( FederateInternalError )
    {
        callback_logs << L"Object Removed: handle=" << theObject << endl;
    }


