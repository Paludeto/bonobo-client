#include <QCoreApplication>
#include <QDebug>
#include <thread>
#include <timer/timer.h>
#include "vision/vision.h"
#include "referee/referee.h"
#include "actuator/actuator.h"
#include "replacer/replacer.h"
#include "worldmap/worldmap.h"
#include "coach/coach.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Setting our color as YELLOW
    VSSRef::Color ourColor = VSSRef::Color::BLUE;

    // Desired frequency in hz
    float freq = 60.0;

    // Starting timer
    Timer timer;

    VisionClient *visionClient = new VisionClient("224.0.0.1", 10002);
    ActuatorClient *actuatorClient = new ActuatorClient("127.0.0.1", 20013);
    WorldMap *wm = new WorldMap(visionClient);
    Coach *coach = new Coach(wm, actuatorClient, ourColor);
    RefereeClient *refereeClient = new RefereeClient("224.5.23.2", 10003);
    ReplacerClient *replacerClient = new ReplacerClient("224.5.23.2", 10004);
    
   
    // Setting actuator and replacer teamColor
    actuatorClient->setTeamColor(ourColor);

    while(1) {

        visionClient->run();
        refereeClient->run();
        bool frameUpdated = wm->updateFrame();

        // Only allow the coach to move players when the game state is GAME_ON
        if (frameUpdated && refereeClient->getLastFoul() == VSSRef::Foul::GAME_ON) {
            coach->runCoach();
        }

        actuatorClient->sendCommand(0, 0, 0);
        actuatorClient->sendCommand(1, 0, 0);
        actuatorClient->sendCommand(2, 0, 0);


        // Stop timer
        timer.stop();

        // Since we want the loop to run at a 60Hz frequency, we use T = 10E3 / f to get the remaining time in miliSeconds and subtract the elapsed time
        long remainingTime = (1000 / freq) - timer.getMiliSeconds();
            // Guard against negative sleep in case processing overruns the timestep
            std::this_thread::sleep_for(std::chrono::milliseconds(std::max<long>(0, remainingTime)));  // Pauses current thread until remaining time
        }

    // Closing clients
    visionClient->close();
    actuatorClient->close();
    

    return a.exec();
}
