#include <QCoreApplication>

#include <thread>
#include <timer/timer.h>
#include <vision/vision.h>
#include <referee/referee.h>
#include <actuator/actuator.h>
#include <replacer/replacer.h>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Starting timer
    Timer timer;

    VisionClient *visionClient = new VisionClient("224.0.0.1", 10002);
    RefereeClient *refereeClient = new RefereeClient("224.5.23.2", 10003);
    ReplacerClient *replacerClient = new ReplacerClient("224.5.23.2", 10004);
    ActuatorClient *actuatorClient = new ActuatorClient("127.0.0.1", 20011);

    // Setting our color as BLUE at left side
    VSSRef::Color ourColor = VSSRef::Color::YELLOW;
    bool ourSideIsLeft = false;

    // Desired frequency in hz
    float freq = 60.0;

    // Setting actuator and replacer teamColor
    actuatorClient->setTeamColor(ourColor);
    replacerClient->setTeamColor(ourColor);

    // Question number 1: Why does this loop require a predictable frequency? 

    while(1) {
        // Start timer
        timer.start();

        // Running vision and referee clients
        visionClient->run();
        refereeClient->run();

        // Gets last Environment. An environment has step, frame, field and number of goals. Step is (most likely) the number of frames polled.
        fira_message::sim_to_ref::Environment lastEnv = visionClient->getLastEnvironment();
        if(lastEnv.has_frame()) {
            // Taking last frame
            fira_message::Frame lastFrame = lastEnv.frame();

            // Debugging ball
            std::cout << "\n===== BALL =====\n";
            QString ballDebugStr = QString("Ball x: %1 y: %2")
                                .arg(lastFrame.ball().x())
                                .arg(lastFrame.ball().y());
            std::cout << ballDebugStr.toStdString() + '\n';

            // Debugging blue robots
            std::cout << "\n===== BLUE TEAM =====\n";
            for(int i = 0; i < lastFrame.robots_blue_size(); i++) {
                QString robotDebugStr = QString("Robot %1 -> x: %2 y: %3 ori: %4")
                        .arg(lastFrame.robots_blue(i).robot_id())
                        .arg(lastFrame.robots_blue(i).x())
                        .arg(lastFrame.robots_blue(i).y())
                        .arg(lastFrame.robots_blue(i).orientation());
                std::cout << robotDebugStr.toStdString() + '\n';
            }

            // Debugging yellow robots
            std::cout << "\n===== YELLOW TEAM =====\n";
            for(int i = 0; i < lastFrame.robots_yellow_size(); i++) {
                QString robotDebugStr = QString("Robot %1 -> x: %2 y: %3 ori: %4")
                        .arg(lastFrame.robots_yellow(i).robot_id())
                        .arg(lastFrame.robots_yellow(i).x())
                        .arg(lastFrame.robots_yellow(i).y())
                        .arg(lastFrame.robots_yellow(i).orientation());
                std::cout << robotDebugStr.toStdString() + '\n';
            }
        }

        // Sending robot commands for robot 0, 1 and 2
        actuatorClient->sendCommand(0, 0, 0);
        actuatorClient->sendCommand(1, 0, 0);
        actuatorClient->sendCommand(2, 0, 0);

        // If is kickoff, send this test frame!
        if(refereeClient->getLastFoul() == VSSRef::Foul::KICKOFF) {
            replacerClient->placeRobot(0, ourSideIsLeft ? -0.2 : 0.2, 0, 0);
            replacerClient->placeRobot(1, ourSideIsLeft ? -0.2 : 0.2, 0.2, 0);
            replacerClient->placeRobot(2, ourSideIsLeft ? -0.2 : 0.2, -0.2, 0);
            replacerClient->sendFrame();
        }

        // Stop timer
        timer.stop();

        // Since we want the loop to run at a 60Hz frequency, we use T = 10E3 / f to get the remaining time in miliSeconds and subtract the elapsed time
        long remainingTime = (1000 / freq) - timer.getMiliSeconds();
        std::this_thread::sleep_for(std::chrono::milliseconds(remainingTime));  // Pauses current thread until remaining time
    }

    // Closing clients
    visionClient->close();
    refereeClient->close();
    replacerClient->close();
    actuatorClient->close();

    return a.exec();
}
