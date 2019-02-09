#ifndef FED1AMB_H_
#define FED1AMB_H_

#include <string>
#include <RTI/NullFederateAmbassador.h>

using namespace rti1516e;
using namespace std;

class Fed1amb : public rti1516e::NullFederateAmbassador
{
    public:
        // variables //

        bool isAnnounced;
        bool isReadyToRun;

        // methods //
        Fed1amb();
        virtual ~Fed1amb() throw();

      // synchronization point methods //

        virtual void synchronizationPointRegistrationSucceeded( const std::wstring& label )
            throw( FederateInternalError ) ;

        virtual void synchronizationPointRegistrationFailed( const std::wstring& label,
                                                SynchronizationPointFailureReason reason )
            throw( FederateInternalError );

        virtual void announceSynchronizationPoint( const std::wstring& label,
                                                   const VariableLengthData& theUserSuppliedTag )
            throw( FederateInternalError );

        virtual void federationSynchronized( const std::wstring& label, FederateHandleSet const& failedToSyncSet)
            throw( FederateInternalError );

};

#endif /*FED1AMB_H_*/
