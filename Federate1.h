#ifndef FEDERATE1_H_
#define FEDERATE1_H_

#include "Fed1amb.h"
#include "RTI/RTI1516.h"

using namespace rti1516e;
using namespace std;

#define READY_TO_RUN L"ReadyToRun"

class Federate1
{

public:
rti1516e::RTIambassador *rtiamb;
Fed1amb *fedamb;

//fom handles//

//AttributeHandle x;
//AttributeHandle y;
//AttributeHandle direction;
//InteractionClassHandle blocked;

//public methods//
Federate1();
virtual ~Federate1();
void runFederate(std::wstring federateName);

//private:
//		void initializeHandles();
//		void waitForUser();
//		void publishAndSubscribe();
//		ObjectInstanceHandle registerObject();
//		void updateAttributeValues( ObjectInstanceHandle objectHandle );
//		void sendInteraction();
//		void deleteObject( ObjectInstanceHandle objectHandle );
};

#endif /*FEDERATE1_H_*/
