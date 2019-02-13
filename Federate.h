#ifndef FEDERATE_H_
#define FEDERATE_H_

#include "Fedamb.h"
#include "RTI/RTI1516.h"

using namespace rti1516e;
using namespace std;

#define READY_TO_RUN L"ReadyToRun"
#define READY_TO_DELETE L"ReadyToDelete"

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

Federate(int, int, int, wstring);
virtual ~Federate();
void runFederate(std::wstring federateName);

private:
        // values //
        int X, Y, Direction;
        wstring Name;
        void updatingAttributesValues(ObjectInstanceHandle, wstring, int, int, int);
        void waitForUser();
        void synchronization();
};

#endif /*FEDERATE_H_*/
