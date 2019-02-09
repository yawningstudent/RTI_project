#include <iostream>
#include "Fed1amb.h"

using namespace std;
using namespace rti1516e;

// constructors //

Fed1amb::Fed1amb()
{
    // initialize all the variable values

    this->isAnnounced   = false;
    this->isReadyToRun  = false;

}

Fed1amb::~Fed1amb() throw()
{
}

// methods //

void Fed1amb::synchronizationPointRegistrationSucceeded( const std::wstring& label)
    throw( FederateInternalError )
{
    wcout << L"Successfully registered sync point: " << label << endl;
}

void
Fed1amb::synchronizationPointRegistrationFailed( const std::wstring& label,
                                                       SynchronizationPointFailureReason reason )
    throw( FederateInternalError )
{
    wcout << L"Failed to register sync point: " << label << endl;
}

void Fed1amb::announceSynchronizationPoint( const std::wstring& label,
                                                  const VariableLengthData& tag )
    throw( FederateInternalError )
{
    wcout << L"Synchronization point announced: " << label << endl;
    if( label.compare(L"ReadyToRun") == 0 )
        this->isAnnounced = true;
}

void Fed1amb::federationSynchronized( const std::wstring& label, FederateHandleSet const& failedToSyncSet)
    throw( FederateInternalError )
{
    wcout << L"Federation Synchronized: " << label << endl;
    if( label.compare(L"ReadyToRun") == 0 )
        this->isReadyToRun = true;
}
