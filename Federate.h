#ifndef FEDERATE_H_
#define FEDERATE_H_

#include "Fedamb.h"
#include "RTI/RTI1516.h"

using namespace rti1516e;
using namespace std;

#define READY_TO_RUN L"ReadyToRun"

class Federate
{

public:
rti1516e::RTIambassador *rtiamb;
Fedamb *fedamb;

//fom handles//

ObjectClassHandle robotClassHandle;
ObjectInstanceHandle robotInstanceHandle;
AttributeHandleSet attrHandleSet;
AttributeHandle nameAttrHandle;
AttributeHandle xAttrHandle;
AttributeHandle yAttrHandle;
AttributeHandle directionAttrHandle;

//public methods//

Federate();
virtual ~Federate();
void runFederate(std::wstring federateName);

private:

        void waitForUser();
};

#endif /*FEDERATE_H_*/
