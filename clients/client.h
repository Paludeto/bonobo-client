#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <QMutex>
#include <QUdpSocket>
#include <QReadWriteLock>
#include <QNetworkDatagram>

// Abstract class that is supposed to be extended by other clients
class Client
{
public:
    Client(QString serverAddress, quint16 serverPort);

    void run();
    void close();
protected:
    QUdpSocket *_clientSocket;
    QString _serverAddress;
    quint16 _serverPort;

    bool _isConnected;

private:

    /* Run-time polymorphism. Makes it so each class that extends client has to implement these methods. Essentially means that a base pointer can point to
    objects that extend this class and resolve their function at run-time, hence why it's called run-time polymorphism */ 
    
    virtual void connectToNetwork() = 0;
    virtual void disconnectFromNetwork() = 0;
    virtual void runClient() = 0;
};

#endif // CLIENT_H
