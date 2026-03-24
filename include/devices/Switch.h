#ifndef SWITCH_H
#define SWITCH_H

#include <iostream>
#include <string>
#include <map>
#include "Device.h"
#include "../core/SimulationLogger.h"
using namespace std;

class Switch : public Device {
private:
    map<string, Device*> macTable;

public:
    Switch();
    Switch(int id, string name);

    void sendFrame(Frame frame, Device* sender = NULL);
    void receiveFrame(Frame frame, Device* sender = NULL);

    void printMACTable() const;
};

#endif