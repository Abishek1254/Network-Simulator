#ifndef I_ERROR_CONTROL_H
#define I_ERROR_CONTROL_H

#include <iostream>
#include <string>
#include "../../core/Frame.h"
using namespace std;

class IErrorControl {
public:
    virtual ~IErrorControl() {}

    virtual void applyErrorControl(Frame& frame) = 0;
    virtual bool checkError(Frame frame) = 0;
    virtual string getProtocolName() = 0;
};

#endif