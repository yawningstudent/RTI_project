#ifndef FEDAMB_H_
#define FEDAMB_H_

#include <string>
#include <RTI/NullFederateAmbassador.h>

using namespace rti1516e;
using namespace std;

class Fedamb : public rti1516e::NullFederateAmbassador
{
    public:
        // variables //

        bool isAnnounced;
        bool isReadyToRun;
        bool isReserved;

        // methods //
        Fedamb();
        virtual ~Fedamb() throw();

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

        // object methods //

        virtual void objectInstanceNameReservationSucceeded (
           std::wstring const & theObjectInstanceName)
           throw (
              FederateInternalError);

        virtual void objectInstanceNameReservationFailed (
           std::wstring const & theObjectInstanceName)
           throw (
              FederateInternalError);

        virtual void discoverObjectInstance( ObjectInstanceHandle theObject,
                                                     ObjectClassHandle theObjectClass,
                                                     const std::wstring& theObjectName )
                    throw( FederateInternalError );

        virtual void discoverObjectInstance( ObjectInstanceHandle theObject,
                                                     ObjectClassHandle theObjectClass,
                                                     const std::wstring& theObjectName,
                                                     FederateHandle producingFederate )
                    throw( FederateInternalError );
};

#endif /*FEDAMB_H_*/
