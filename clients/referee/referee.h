#ifndef REFEREECLIENT_H
#define REFEREECLIENT_H

#include <clients/client.h>
#include <vssref_command.pb.h>

// Class used to retrieve data (fouls) from the referee. Part of VSSRef.
class RefereeClient : public Client 
{

public:
    using Client::Client;

    // Uses VSSRef's namespace to retrieve and store foul data
    VSSRef::Foul getLastFoul();
    VSSRef::Color getLastFoulColor();
    VSSRef::Quadrant getLastFoulQuadrant();

private: 

    // Stores last foul data in a tuple
    std::tuple<VSSRef::Foul, VSSRef::Color, VSSRef::Quadrant> _lastFoulData;

    // Allows foulMutex to be read by multiple sources, but only allows one write
    // Mutex used to lock/unlock read/write foul data
    QReadWriteLock _foulMutex;

    void connectToNetwork();
    void disconnectFromNetwork();

    void runClient();

};

#endif // REFEREECLIENT_H