#ifndef I_FLOW_CONTROL_H
#define I_FLOW_CONTROL_H

#include <iostream>
#include <string>
#include "../../core/Frame.h"
using namespace std;

class IFlowControl {
public:
    virtual ~IFlowControl() {}

    virtual bool sendWithFlowControl(Frame frame) = 0;
    virtual string getProtocolName() = 0;
};

#endif