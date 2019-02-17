#ifndef FEDERATE_H_
#define FEDERATE_H_

#include "Fedamb.h"
#include <fstream>
#include "RTI/RTI1516.h"

using namespace rti1516e;
using namespace std;

#define READY_TO_RUN L"ReadyToRun"
#define READY_TO_DELETE L"ReadyToDelete"

class Federate
{

public:
RTIambassador *rtiamb;
Fedamb *fedamb;

//fom handles//

ObjectClassHandle robotClassHandle;
ObjectInstanceHandle robotInstanceHandle;
AttributeHandleSet attrHandleSet;
AttributeHandle nameAttrHandle;
AttributeHandle xAttrHandle;
AttributeHandle yAttrHandle;
//AttributeHandle directionAttrHandle;

//public methods//

Federate(wstring);
virtual ~Federate();
void runFederate(wstring, char*);

private:
        // values //
        int X, Y, Direction;
        wofstream prog_logs;
        wstring Name;
        void updatingAttributesValues(ObjectInstanceHandle);
        void waitForUser();
        void synchronization();
        void DirectionChoose(int, int);
        void movement(int, int);
};

#endif /*FEDERATE_H_*/
