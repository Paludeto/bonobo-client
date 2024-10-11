#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <client.h>
#include <vssref_common.pb.h>
#include <packet.pb.h>

class ActuatorClient : public Client
{
public:
    using Client::Client; // Uses same constructor as Client
    void setTeamColor(VSSRef::Color teamColor);
    void sendCommand(quint8 robotId, float leftWheel, float rightWheel);

private:
    VSSRef::Color _teamColor;

    void connectToNetwork();
    void disconnectFromNetwork();

    void runClient();
};

#endif // ACTUATOR_H
