#ifndef STOP_AND_WAIT_FLOW_CONTROL_H
#define STOP_AND_WAIT_FLOW_CONTROL_H

#include <iostream>
#include <string>
#include "IFlowControl.h"
using namespace std;

class StopAndWaitFlowControl : public IFlowControl {
private:
    bool waitingForAck;

public:
    StopAndWaitFlowControl();

    bool sendWithFlowControl(Frame frame);
    void receiveAck();
    string getProtocolName();
};

#endif