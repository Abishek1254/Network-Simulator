# Network Simulator - Semester Project Submission 1

## Project Title
Network Simulator Implementing Protocol Stack (Submission 1)

## Course
Computer Networks Lab

## Submission
Semester Project Submission 1

## Overview
This project is a console-based network simulator developed in **C++**.  
The goal of this submission is to implement the required parts of the **Physical Layer** and **Data Link Layer** as mentioned in the assignment document.

This simulator is designed in a **modular and extensible object-oriented way** so that future submissions can build on top of the current implementation and add the remaining layers of the protocol stack.

---

## Programming Language and Environment
- **Language Used:** C++
- **Compiler Used:** g++
- **Execution Style:** Console-based simulation
- **Development Style:** Object-Oriented Programming (OOP), modular file structure

---

## Input / Output Representation

### Input
The simulator currently takes input through a **menu-driven console interface**.  
The user can select a scenario to run from the available test cases.

### Output
The simulator prints:
- device creation and transmission events
- frame forwarding and broadcasting logs
- MAC learning logs for switch
- protocol behavior logs
- collision domain and broadcast domain reports

The output is textual and intended for simulation and demonstration purposes only.

---

## Scope of Submission 1

This submission covers:

### 1. Physical Layer Functionalities
- Creating **end devices**
- Creating **hub**
- Creating **connections** to form network topology
- Sending and receiving data

### 2. Data Link Layer Functionalities
- Creating **bridge**
- Creating **switch**
- **MAC address learning** in switch
- One **error control protocol**
- One **access control protocol**
- One **sliding window-based flow control protocol**

---

## Protocols Implemented

### 1. Access Control Protocol
- **CSMA/CD**
- Used to simulate media access checking before transmission

### 2. Error Control Protocol
- **Parity Error Control**
- A parity bit is generated and checked for transmitted frames

### 3. Flow Control Protocol
- **Go-Back-N**
- Used as the sliding window-based flow control protocol

---

## Design Approach

The simulator is implemented using a modular class-based design.

### Core Components
- `Address`  
  Represents MAC address of a device

- `Frame`  
  Represents the data unit being transmitted with:
  - source MAC
  - destination MAC
  - payload
  - sequence number
  - parity bit

- `SimulationLogger`  
  Prints logs for simulation events

### Device Classes
- `Device`  
  Abstract base class for all devices

- `EndDevice`  
  Represents a host/end system that can send and receive frames

- `Hub`  
  Broadcasts incoming frames to all connected devices except sender

- `Bridge`  
  Layer 2 forwarding device included as part of the implementation

- `Switch`  
  Performs MAC learning and selective forwarding

### Protocol Interfaces
To keep the code extensible, protocol functionality is separated through interfaces:

- `IAccessControl`
- `IErrorControl`
- `IFlowControl`

This allows future addition of more protocols without changing the main device architecture.

### Services
- `DomainAnalyzer`  
  Used to calculate:
  - collision domains
  - broadcast domains

### Scenarios
The required assignment test cases are implemented as separate scenario files.

---

## Scenarios Implemented

### 1. Dedicated Link Scenario
- Two end devices connected directly
- Demonstrates direct data transmission

### 2. Hub Star Scenario
- Five end devices connected to one hub
- Demonstrates hub-based broadcasting behavior
- Reports:
  - collision domains
  - broadcast domains

### 3. Switch Scenario
- Five end devices connected to one switch
- Demonstrates:
  - address learning
  - flooding for unknown destination
  - forwarding for known destination
  - access control
  - error control
  - flow control
- Reports:
  - collision domains
  - broadcast domains

### 4. Hybrid Scenario
- Two star topologies
- Each star has five end devices connected to one hub
- Both hubs are connected using a switch
- Demonstrates communication across both stars
- Reports:
  - collision domains
  - broadcast domains

---

## Topology and Domain Assumptions

### Hub Star Topology
- Collision Domains: **1**
- Broadcast Domains: **1**

### Switch Star Topology
- Collision Domains: **number of connected end devices**
- Broadcast Domains: **1**

### Two Hubs Connected Through Switch
- Collision Domains: **2**
- Broadcast Domains: **1**

These values are based on the simplified simulation model used in this submission.

---

## Folder Structure

```text
NetworkSimulator/
│
├── main.cpp
├── README.md
│
├── include/
│   ├── common/
│   │   └── Types.h
│   ├── core/
│   │   ├── Address.h
│   │   ├── Frame.h
│   │   └── SimulationLogger.h
│   ├── devices/
│   │   ├── Device.h
│   │   ├── EndDevice.h
│   │   ├── Hub.h
│   │   ├── Bridge.h
│   │   └── Switch.h
│   ├── protocols/
│   │   ├── access/
│   │   │   ├── IAccessControl.h
│   │   │   └── CSMACD.h
│   │   ├── error/
│   │   │   ├── IErrorControl.h
│   │   │   └── ParityErrorControl.h
│   │   └── flow/
│   │       ├── IFlowControl.h
│   │       └── GoBackNFlowControl.h
│   ├── services/
│   │   └── DomainAnalyzer.h
│   └── scenarios/
│       ├── DedicatedLinkScenario.h
│       ├── HubStarScenario.h
│       ├── SwitchScenario.h
│       └── HybridScenario.h
│
└── src/
    ├── core/
    ├── devices/
    ├── protocols/
    ├── services/
    └── scenarios/