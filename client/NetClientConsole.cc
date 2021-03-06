/*
FILNAMN: 		NetClient.cc
PROGRAMMERARE:	hanel742, eriek984, jened502, tobgr602, niker917, davha227
SKAPAD DATUM:	2013-11-19
BESKRIVNING:	
*/

#include "NetClientConsole.h"
#include <QTest>

using namespace std;

NetClient::NetClient(QString username, QString inAddress, QObject *parent) : QObject(parent){
    
    name=username;
    address=inAddress;
    compare += 0x1F;
    breaker +=0x1E;
}


void NetClient::start(){
    TcpSocket = new QTcpSocket(this);
    
    connect(TcpSocket,SIGNAL(connected()),this,SLOT(connected()));
    connect(TcpSocket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(TcpSocket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    
    
    QHostInfo info = QHostInfo::fromName(address);
    
    if (info.addresses().size() == 0){
        qDebug() << "Unable to find hostname.";
    }
    else{
        TcpSocket->connectToHost(info.addresses().at(0),quint16(40001));
        
        if(!TcpSocket->waitForConnected(1000)){
            qDebug() << "Connection time out.";
        }
    }

}


//------Slots---------

void NetClient::connected(){
    QByteArray array = "/initiate";
    array += 0x1F; //unit separator
    array += name;
    array += breaker;
    
    TcpSocket->write(array);
    TcpSocket->waitForBytesWritten(1000);
}

// ---------------------------------------

void NetClient::disconnected(){
    qDebug() << "disconnected";
    
}

// ---------------------------------------

void NetClient::bytesWritten(qint64 bytes){
    qDebug() << "we wrote: " << bytes;
    
}

// ---------------------------------------

void NetClient::readyRead(){
    
    QString inData = "";
    
    if( not(incompleteCommand.isEmpty())){
        inData = incompleteCommand;
        incompleteCommand = "";
    }
    QByteArray Data = TcpSocket->readAll();
    
    QString commandName = "";
    inData += Data;
    QString rest = "";
    int n = inData.indexOf(breaker);
    int i;
    
    
    if(inData.indexOf(breaker) == -1 ){
        incompleteCommand = inData;
        return;
    }
    
    
    do {
        rest = inData.mid(n+1);
        inData = inData.left(n);
        i = inData.indexOf(compare);
        
        commandName = inData.left(i);
        
        inData = inData.mid(i+1);
        
        QString temp = inData;
        string stdInData = temp.toStdString();
        
        // Check which command that's supposed to run
        if (commandName == "/reinitiate") {
            qDebug() << "Username taken.";
            break;
        }
        
        else if ( commandName == "/userAccepted") {
            qDebug() << "Connected!";
        }
        
        else if (commandName == "/history") {
            handleHistory(inData);
        }
        
        else if (commandName == "/oldHistory") {
            handleOldHistory(inData);
        }
        
        else if (commandName == "/message") {
            handleMessage(inData);
        }
        
        else if ( commandName == "/requestStruct") {
            handleRequestStruct();
        }
        
        else if ( commandName == "/structure" ) {
            handleStructure(inData);
        }
        
        else {
            
            //throw logic_error("Unknown command: " + commandName.toStdString());
            //release version
            qDebug() << "Unknown command: ";
            inData = "";
            commandName = "";
            incompleteCommand = "";
            return;
        }
        
        inData = rest;
        n = inData.indexOf(breaker);
        commandName = "";
        
    }while (n != -1 );

}

// ---------------------------------------

void NetClient::sendMessage(QString from, QString to, QString message){
    QByteArray array = "/message";
    array += 0x1F; //unit separator
    array += from;
    array += 0x1F;
    array += to;
    array += 0x1F;
    array += message;
    array += breaker;
    
    TcpSocket->write(array);
    TcpSocket->waitForBytesWritten(1000);
}

// ---------------------------------------------

void NetClient::handleMessage(QString inData){
    
    int i;
    // Get from
    i = inData.indexOf(compare);
    QString from = inData.left(i);
    inData = inData.mid(i+1);
    
    // Get to
    i = inData.indexOf(compare);
    QString to = inData.left(i);
    inData = inData.mid(i+1);
    
    // Get message
    i = inData.indexOf(compare);
    QString contents = inData.left(i);
    inData = inData.mid(i+1);
    
    // Get time
    QString dateTime = inData;
    
    // Output data
    qDebug() << dateTime + "   " + from + ": " + contents;
    
}

// ---------------------------------------------

void NetClient::handleHistory(QString inData){
    QVector<QString> history;
    int i = inData.indexOf(compare);
    while(i != -1 ){
        
        // Get from
        i = inData.indexOf(compare);
        QString from = inData.left(i);
        inData = inData.mid(i+1);
        history.push_back(from);
        
        // Get to
        i = inData.indexOf(compare);
        QString to = inData.left(i);
        inData = inData.mid(i+1);
        history.push_back(to);
        
        // Get message
        i = inData.indexOf(compare);
        QString contents = inData.left(i);
        inData = inData.mid(i+1);
        history.push_back(contents);
        
        
        //Get time
        i = inData.indexOf(compare);
        QString time = inData.left(i);
        inData = inData.mid(i+1);
        history.push_back(time);
    }
    
    // PRint out
    for (i = 0; i < history.size(); i++) {
        QString temp;
        temp = history.at(i+3); // time
        temp += "   ";
        
        temp += history.at(i); // From
        temp += ": ";
        
        i++;    // Skip to
        i++;    // Skip time
        temp += history.at(i); // Message
        i++;
        qDebug() << temp;
    }
}

// ---------------------------------------------

void NetClient::handleOldHistory(QString inData){
    QVector<QString> history;
    int i = inData.indexOf(compare);
    while(i != -1 ){
        
        // Get from
        i = inData.indexOf(compare);
        QString from = inData.left(i);
        inData = inData.mid(i+1);
        history.push_back(from);
        
        // Get to
        i = inData.indexOf(compare);
        QString to = inData.left(i);
        inData = inData.mid(i+1);
        history.push_back(to);
        
        // Get message
        i = inData.indexOf(compare);
        QString contents = inData.left(i);
        inData = inData.mid(i+1);
        history.push_back(contents);
        
        
        //Get time
        i = inData.indexOf(compare);
        QString time = inData.left(i);
        inData = inData.mid(i+1);
        history.push_back(time);
    }
    //guiPointer->receiveOldHistory(history);
}

// ---------------------------------------------

void NetClient::handleStructure(QString inData){
    QVector<QString> output;
    int i = inData.indexOf(compare);
    while(i  != -1){
        QString data = inData.left(i);
        inData = inData.mid(i+1);
        output.push_back(data);
        
        i = inData.indexOf(compare);
    }
    
    output.push_back(inData);
    
    //guiPointer->updateStruct(output);
    
}

// ---------------------------------------------

void NetClient::getHistory(unsigned int daysBack) {
    QString temp;
    string daysBackString = to_string(daysBack);
    
    temp = QString::fromStdString(daysBackString);
    
    QByteArray array = "/oldHistory";
    array += compare; //unit separator
    array += temp;
    array += breaker;
    
    TcpSocket->write(array);
    TcpSocket->waitForBytesWritten(1000);
}

// ---------------------------------------------

void NetClient::handleRequestStruct(){
    QByteArray array = "/structure";
    array += compare;
    array += breaker;
    
    TcpSocket->write(array);
    TcpSocket->waitForBytesWritten(1000);
}
