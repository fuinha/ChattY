/*
FILNAMN: 		room.h
PROGRAMMERARE:	hanel742, eriek984, jened502, tobgr602, niker917, davha227
SKAPAD DATUM:	2013-11-14
BESKRIVNING:	
Benny
*/

#ifndef room_H
#define room_H

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include "../getTime.h"
#include "../message/message.h"

class Room {
public:
    // Construct
    Room(std::string);
    // Destruct
    virtual ~Room();
    
    virtual void sendMessage(Message);
    void addRoom(Room&);
    void removeRoom(Room&);             // Throws error if room doesn't exist
    void receiveMessage(Message);
    std::string getName();
    
protected:
    void saveToFile(Message);
    void sendMessageAll(Message);
    std::string name;
    std::map<std::string,Room*> rooms;
    std::vector<Message> log;
    Room* parentRoom = nullptr;
};

// --------------------
// User

class User : public Room
{
public:
    User(std::string inName) : Room(inName) {};
    ~User();
    std::string name;
    void sendMessage(Message);
    void receiveMessage(Message);
    void chooseRoom(Room*);
    
private:
    std::vector<Message> log;
    Room* parentRoom;
    
};

#endif