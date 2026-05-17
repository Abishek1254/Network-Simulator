#ifndef GO_BACK_N_FLOW_CONTROL_H
#define GO_BACK_N_FLOW_CONTROL_H

#include <iostream>
#include <string>
#include <queue>
#include "IFlowControl.h"
using namespace std;

class GoBackNFlowControl : public IFlowControl {
private:
    int windowSize;
    int base; // Sf -> base sequence number
    int nextSequenceNumber; // Snext -> next sequence number to be sent

public:
    GoBackNFlowControl(int windowSize = 3);

    bool sendWithFlowControl(Frame frame);
    void receiveAck(int ackNumber);
    string getProtocolName();

    int getBase() const;
    int getNextSequenceNumber() const;
    int getWindowSize() const;
};

#endif