#include "robot.h"
#include <Romi32U4Buttons.h>

Romi32U4ButtonA buttonC;
Romi32U4ButtonB buttonB;

void Robot::InitializeRobot(void)
{
    chassis.InititalizeChassis();
    pinMode(13, OUTPUT);
}

void Robot::EnterIdleState(void)
{
    chassis.Stop();

    Serial.println("-> IDLE");
    robotState = ROBOT_IDLE;
}

/**
 * The main loop for your robot. Process both synchronous events (motor control),
 * and asynchronous events (distance readings, etc.).
*/
void Robot::RobotLoop(void) 
{
     /**
     * Run the chassis loop, which handles low-level control.
     */
    Twist velocity;
    if(chassis.ChassisLoop(velocity))
    {

        // We alwayse update pose
        UpdatePose(velocity);
    
        /**
         * Here, we break with tradition and only call these functions if we're in the 
         * DRIVE_TO_POINT state. CheckReachedDestination() is expensive, so we don't want
         * to do all the maths when we don't need to.
         * 
         * While we're at it, we'll toss DriveToPoint() in, as well.
         */ 

        
        // Check if Button C is pressed 
        if (buttonC.getSingleDebouncedPress()) 
        {
            // Example: Set a destination 60cm ahead when button is pressed
            Pose target;
            target.x = currPose.x + 60.0 * cos(currPose.theta);
            target.y = currPose.y + 60.0 * sin(currPose.theta);
            
            SetDestination(target);
            digitalWrite(13, HIGH); // Turn on LED to show we are moving
        }
       

        // logic fron state macine
        if(robotState == ROBOT_DRIVE_TO_POINT)
        {
            DriveToPoint();

            //check if robot is there
            if(CheckReachedDestination())
            {
                HandleDestination();
                digitalWrite(13, LOW); //this turns off the LED when done
            }
        }
        else if (robotState == ROBOT_IDLE)
        {
            chassis.Stop();
        }
    }
}
