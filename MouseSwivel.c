#pragma config(Sensor, dgtl1,  bump,           sensorTouch)
#pragma config(Sensor, dgtl2,  sonar,          sensorSONAR_cm)
#pragma config(Sensor, dgtl4,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl6,  leftEncoder,    sensorQuadEncoder)
#pragma config(Motor,  port2,           turnServo,     tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port3,           flagServo,     tmotorServoStandard, openLoop)
#pragma config(Motor,  port4,           driveMotor,    tmotorServoContinuousRotation, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


int nForward = -86; // turnServo value for going straight
int nTurn = 86; // turnServo value for turning
int driveSpeed = 50;
int turnSpeed = 50;

//---------------------------------------------------------
// moveTillNear(distance) - Moves robot until a given distance
// (in CM) from whatever the sonar is sensing.  The function
// returns the distance the robot has moved (in CM)

void moveTillNear(int distance)
{
  int left;
  int right;
  SensorValue(leftEncoder) = 0;  // reset encoders
  SensorValue(rightEncoder) = 0;

  motor[turnServo] = nForward; // swivel wheel to face forward
  wait1Msec(500);

  motor[driveMotor] = driveSpeed; // start going forward

  while(SensorValue(sonar) > distance+4 || SensorValue(sonar) < 0)
  {
    // Correct if not going straight
    left = SensorValue(leftEncoder);
    right = SensorValue(rightEncoder);
    if(left > right)
      {motor[turnServo] = nForward - 6;}
    else if(left < right)
      {motor[turnServo] = nForward + 6;}
    else
      {motor[turnServo] = nForward;}
  }

  motor[driveMotor] = 0; // stop
  wait1Msec(1000);

  //return SensorValue(leftEncoder)*7/286; // returns distance traveled

} // end function


//---------------------------------------------------------
// turn90Right() - Turns robot 90 degrees to the right
void turn90Right()
{
  SensorValue(leftEncoder) = 0;  // reset encoders
  SensorValue(rightEncoder) = 0;

  motor[turnServo] = nTurn; // swivel wheel to face forward
  wait1Msec(500);

  // while loops uses the average of the encoder values to judge whether
  // 90 degree turn is complete
  while((SensorValue[leftEncoder] - SensorValue[rightEncoder])/2 < 446)
  {
    if(SensorValue(leftEncoder) < 350)
    {motor[driveMotor] = 60;} // drive forward
    else
    {motor[driveMotor] = 40;} // drive forward slowly
  }

  // stop
  motor[driveMotor] = 0;
  wait1Msec(100);

  // turn the front wheel so that it is forward
  motor[turnServo] = nForward; // swivel wheel to face forward
  wait1Msec(500);
}

//---------------------------------------------------------
// turn90Left() - Turns robot 90 degrees to the left
void turn90Left()
{
  SensorValue(leftEncoder) = 0;  // reset encoders
  SensorValue(rightEncoder) = 0;

  motor[turnServo] = nTurn; // swivel wheel to face forward
  wait1Msec(500);

  // while loops uses the average of the encoder values to judge whether
  // 90 degree turn is complete
  while((SensorValue[rightEncoder] - SensorValue[leftEncoder])/2 < 436)
  {
    if(SensorValue(rightEncoder) < 350)
    {motor[driveMotor] = -60;} // drive
    else
    {motor[driveMotor] = -40;} // drive slowly
  }

  // stop
  motor[driveMotor] = 0;
  wait1Msec(100);

  // turn the front wheel so that it is forward
  motor[turnServo] = nForward; // swivel wheel to face forward
  wait1Msec(500);
}

void cheese()
{
  int i;
  for(i = 0; i < 10; i++)
  {
    motor[flagServo] = 30;
    wait1Msec(200);
    motor[flagServo] = -30;
    wait1Msec(200);
  }
  motor[flagServo] = 0;
}

//*****************************************************
// NO CHANGES ABOVE THIS LINE
//*****************************************************
/*
 * Checks state of the robot. If the robot is at cheese,
 * returns 2. If the robot can turn, returns 1. If the robot
 * can still go forward, returns 0.
 */
int checkState(){
    //If the sonar has more to go, then return 0.
    if (SensorValue[sonar] <= 14){
        turn90Left();
        //If the first turn can be taken, return 1 to make sure
        //the robot takes it.
        if (SensorValue[sonar] <= 14){
            //If the robot takes a 180 degree turn in the other
            //direction, then check distance. If distance is less
            //than or equal to 14, the the robot is fine.
            //Otherwise, the robot can take another path.
            for (int i = 0; i < 2; i++){
                turn90Right();
            }
            if (SensorValue[sonar] <= 14){
                return 2;
            }
            else{
                //If didn't return 2 by now, must return 1.
                return 1;
            }
        }
        else{
            return 1;
        }
    }
    else{
        return 0;
    }
}

int reactToState(int x){
    switch(x){
        case 0:
            //This should never be thrown, because the sensor
            //is supposed to stop after the robot has moved
            //to proximity. But, if there's a mechanical, then
            //the robot moves closer.
            moveTillNear(14);
            break;
        case 1:
            //The code before already sets up robot to move in
            //clear direction. So, all you need to do is move
            //until you need to make another decision.
            moveTillNear(14);
            break;
        case 2:
            //If there is no way to go but back, then the robot
            //does cheese and exits.
            cheese();
            break;
        //This would be awkward...
        default:
            break;
    }
    return x;
}

//**********************************************************
task main()
{

  //moveTillNear(14) centers the robot in a corner
  // of the path

  while(SensorValue[bump] == 0) {}
  //Based off of a reactionary model. Simply put, always run
  //until cheese is hit.

  //Got to give it the baby step first...
  moveTillNear(14);
    while(true){
        //Repeatedly check for the sonar to be less than or equal
        //to 14. If so, check state and react to it.
        if (SensorValue[sonar] <= 14){
            if (reactToState(checkState()) == 2){
                break;
            }
        }
    }


  /*Sample calls to functions
	moveTillNear(14);
	turn90Right();
	turn90Left();
	moveTillNear(14);
	cheese();
	*/


}
