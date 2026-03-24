#ifndef HUB_H
#define HUB_H

#include <iostream>
#include <string>
#include "Device.h"
#include "../core/SimulationLogger.h"
using namespace std;

class Hub : public Device {
public:
    Hub();
    Hub(int id, string name);

    void sendFrame(Frame frame, Device* sender = NULL);
    void receiveFrame(Frame frame, Device* sender = NULL);
};

#endif