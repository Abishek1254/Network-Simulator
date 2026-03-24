#ifndef PARITY_ERROR_CONTROL_H
#define PARITY_ERROR_CONTROL_H

#include <iostream>
#include <string>
#include "IErrorControl.h"
using namespace std;

class ParityErrorControl : public IErrorControl {
public:
    void applyErrorControl(Frame& frame);
    bool checkError(Frame frame);
    string getProtocolName();
};

#endif