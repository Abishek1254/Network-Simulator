#ifndef I_ACCESS_CONTROL_H
#define I_ACCESS_CONTROL_H

#include <iostream>
#include <string>
#include "../../core/Frame.h"
using namespace std;

class IAccessControl {
public:
    virtual ~IAccessControl() {}

    virtual bool canTransmit(Frame frame) = 0;
    virtual string getProtocolName() = 0;
};

#endif