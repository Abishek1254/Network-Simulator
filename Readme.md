# Network Simulator - Protocol Stack Simulator

## Project Title
Network Simulator Implementing Protocol Stack

## Course
Computer Networks Lab

## Submission
Semester Project Submission 1 + Submission 2

## Overview
This project is a console-based network simulator developed in **C++**.  
The goal of this project is to implement the required parts of the **Physical Layer**, **Data Link Layer**, and **Network Layer** as mentioned in the assignment document.

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
The simulator runs all built-in scenarios automatically from `main.cpp`.  
It does **not** use a "press 1 for physical layer, 2 for data link layer" style menu.

### Output
The simulator prints:
- device creation and transmission events
- frame forwarding and broadcasting logs
- MAC learning logs for switch
- protocol behavior logs
- ARP request/reply logs
- router interface and routing-table logs
- collision domain and broadcast domain reports

The output is textual and intended for simulation and demonstration purposes only.

---

## Scope of Submission 1+ Submission 2

This project currently covers:

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

### 3. Network Layer Functionalities
- Creating and configuring **routers**
- Assigning **well-formatted classless IPv4 addresses**
- Resolving next-hop MAC addresses using **ARP**
- Performing **static routing**
- Performing **longest prefix match** during route lookup
- Performing **dynamic routing using RIP**

The integrated network-layer scenario sends IP traffic over the already implemented lower layers, so ARP, IPv4 forwarding, switching, MAC learning, access control, error control, and flow control are logged together as part of the same simulation flow.

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
  - optional IPv4 / ARP / routing update content

- `SimulationLogger`  
  Prints logs for simulation events

- `IPv4Address`  
  Represents IPv4 addresses and subnet calculations

- `IPPacket` / routing data structures  
  Represent Layer 3 packets, ARP messages, and routing table entries

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

- `Router`  
  Performs IP forwarding across multiple interfaces
  - maintains connected, static, and RIP-learned routes
  - resolves next-hop MAC addresses through ARP
  - applies longest prefix matching for route selection

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
The required assignment test cases are implemented as separate scenario files and are executed automatically in one simulator run.

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

### 5. Network Layer Scenario
- LAN communication using **ARP**
- Remote delivery using **static routing**
- **Longest prefix match** route selection
- **RIP** exchange between routers
- End-to-end IP delivery across multiple routed networks

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
