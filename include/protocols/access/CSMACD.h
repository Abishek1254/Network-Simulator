#ifndef CSMACD_H
#define CSMACD_H

#include <iostream>
#include <string>
#include "IAccessControl.h"
using namespace std;

class CSMACD : public IAccessControl {
private:
    bool mediumBusy;

public:
    CSMACD();

    void setMediumBusy(bool busy);

    bool canTransmit(Frame frame);
    string getProtocolName();
};

#endif