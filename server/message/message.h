/*
 FILNAMN: 		message.h
 LABORATION:
 PROGRAMMERARE:	hanel742, eriek984, jened502, tobgr602, niker917, davha227
 DATUM:			2013-11-14
 BESKRIVNING:
 */

#ifndef MESSAGE
#define MESSAGE
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include "../getTime.h"

/*
 Class holding contents and other info regarding messages. 
 */

class Message
{
public:
    Message(const std::string message, const std::string from, const std::string to);
    Message(const std::string message, const std::string from, const std::string to, const std::string time);
    ~Message()=default;
    
    
    
    std::string getMessage() const;
    std::string getFrom() const;
    std::string getTo() const;
    std::string getServerTime() const;
    void print() const;
    
private:
    std::string message;
    std::string from;
    std::string to;
    std::string localTime;
    std::string serverTime;
    
    
};



#endif