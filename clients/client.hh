#ifndef CLIENT_HH
#define CLIENT_HH

#include <iostream>
#include <QMutex>
#include <QUdpSocket>
#include <QReadWriteLock>
#include <QNetworkDatagram>

class Client
{
public:
    Client(QString serverAddress, quint16 serverPort);

    void run();
    void close();
protected:
    QUdpSocket *clientSocket;
    QString _serverAddress;
    quint16 _serverPort;

    bool _isConnected;

private:
    virtual void connectToNetwork() = 0;
    virtual void disconnectFromNetwork() = 0;
    virtual void runClient() = 0;
};

#endif // CLIENT_HH
