#include <QCoreApplication>
#include <actuator/actuator.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ActuatorClient *actuator = new ActuatorClient("127.0.0.1", 20011);
    
    while (1) {
        actuator->sendCommand(0, -10, 10);
    }
    
    return a.exec();
}
