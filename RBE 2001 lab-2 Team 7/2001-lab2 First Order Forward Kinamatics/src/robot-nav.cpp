/**
 * robot-nav.cpp is where you should put navigation routines.
 */

#include "robot.h"

void Robot::UpdatePose(const Twist& twist) //estimates where robot is
{
    
    const float CONTROL_LOOP_PERIOD_MS = 10;
    float dt = CONTROL_LOOP_PERIOD_MS / 1000.0f;
    

   // Update heading
    currPose.theta += twist.omega * dt;

    // Update position
    currPose.x += twist.u * cos(currPose.theta) * dt;
    currPose.y += twist.u * sin(currPose.theta) * dt;


#ifdef __NAV_DEBUG__
    TeleplotPrint("x", currPose.x);
    TeleplotPrint("y", currPose.y);
    TeleplotPrint("theta", currPose.theta);
#endif

}

/**
 * Sets a destination in the lab frame.
 */
void Robot::SetDestination(const Pose& dest)
{
    /**
     * TODO: Turn on LED, as well.
     */
    Serial.print("Setting dest to: ");
    Serial.print(dest.x);
    Serial.print(", ");
    Serial.print(dest.y);
    Serial.print('\n');

    startPose = currPose; //says that the current pose is where the robot started moving
    destPose = dest;
    robotState = ROBOT_DRIVE_TO_POINT;
}

bool Robot::CheckReachedDestination(void) //checks if distance >= 60cm
{
    //calculate distance between current position and dest pose
    float dx = currPose.x - startPose.x;
    float dy = currPose.y - startPose.y;
    float dist = sqrt(dx*dx + dy*dy);

    //threshold to account for error. ADJUST THIS LATER, IT SHOULD PROBABLY BE SMALLER
    if(dist < 0.5)
    {
        return true;
    }

    return false;
    
}

void Robot::DriveToPoint(void) //tells robot to go straight
{
    if(robotState == ROBOT_DRIVE_TO_POINT)
    {
        float effort = 50;
        chassis.SetMotorEfforts(effort, effort); //same effort for both wheels

#ifdef __NAV_DEBUG__
        float dx = currPose.x - startPose.x;
        float dy = currPose.y - startPose.y;
        float dist = sqrt(dx*dx + dy*dy);

        TeleplotPrint("dist", dist);
        TeleplotPrint("x", currPose.x);
        TeleplotPrint("y", currPose.y);
        TeleplotPrint("theta", currPose.theta);
#endif 
    }
}

void Robot::HandleDestination(void) //stops robot
{
    chassis.SetMotorEfforts(0, 0); //stopped motors
    robotState = ROBOT_IDLE; //changed state to idle
}