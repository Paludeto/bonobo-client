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
    ActuatorClient *actuatorClient = new ActuatorClient("127.0.0.1", 20011);
    WorldMap *wm = new WorldMap(visionClient);
    Coach *coach = new Coach(wm, actuatorClient, ourColor);
    
   
    // Setting actuator and replacer teamColor
    actuatorClient->setTeamColor(ourColor);

    while(1) {

        visionClient->run();
        wm->updateFrame();

        if (wm->updateFrame() == true) {
            coach->runCoach();
        }
    
        // Stop timer
        timer.stop();

        // Since we want the loop to run at a 60Hz frequency, we use T = 10E3 / f to get the remaining time in miliSeconds and subtract the elapsed time
        long remainingTime = (1000 / freq) - timer.getMiliSeconds();
        std::this_thread::sleep_for(std::chrono::milliseconds(remainingTime));  // Pauses current thread until remaining time
    }

    // Closing clients
    visionClient->close();
    actuatorClient->close();

    return a.exec();
}
