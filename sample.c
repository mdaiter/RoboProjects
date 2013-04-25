#pragma config(Sensor, dgtl1,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl6,  sonar,          sensorSONAR_cm)
#pragma config(Sensor, dgtl10, led,            sensorLEDtoVCC)
#pragma config(Sensor, dgtl12, led2,           sensorLEDtoVCC)
#pragma config(Motor,  port2,           motorleft,     tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port3,           motorright,    tmotorServoContinuousRotation, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
 * Inits motors to go forwards with certain value.
 */
void initMotors(int x){
	motor[motorleft] = x;
	motor[motorright] = x;
}

/*
 * Stops motors.
 */
void stopMotors(){
	initMotors(0);
}

task autoCorrect(){
   if (SensorValue[leftEncoder] - SensorValue[rightEncoder] > 60){
   motor[motorleft] -= 2;
   motor[motorright] += 2;
  }
 else if (SensorValue[rightEncoder] - SensorValue[leftEncoder] > 60){
    motor[motorleft] += 2;
   motor[motorright] -= 2;
 }
}
 void TillNear(int distance){
   wait1Msec(1000);
motor[motorleft]= 90;
 motor[motorright] = 90;
SensorValue[rightEncoder]=0;
SensorValue[leftEncoder] = 0;
 while (SensorValue[sonar]> distance +10){
 //StartTask(autoCorrect);
wait1Msec(1);
}

motor[motorleft] = 0;
motor[motorright] = 0;
 wait1Msec(1000);
}
void goCentimeters(float cm){
 float cir = PI * 12;
    float rotations = (cm-5) / cir;
 int encoderAmount = rotations * 360;
 SensorValue[leftEncoder] = 0;
 SensorValue[rightEncoder] = 0;
  motor[motorleft] = 90;
  motor[motorright] = 90;
 while (-SensorValue[leftEncoder] < encoderAmount
   && -SensorValue[rightEncoder] < encoderAmount){
   //startTask(autoCorrect);
  wait1Msec(1);
  }
 motor[motorleft] = 0;
 motor[motorright] = 0;
 wait1Msec(1000);
}
void rightTurn90(){
  SensorValue[rightEncoder] = 0;
 SensorValue[leftEncoder] = 0;
  motor[motorleft] = 90;
 motor[motorright] = -90;
 while(SensorValue[leftEncoder] >=-150){
   wait1Msec(1);
   }
    motor[motorleft] = 0;
    motor[motorright] = 0;
  wait1Msec(1000);
}

/*
 * Detects board.
 */
void detectBoard(){
    //Turn robot.
    motor[motorleft] = 80;
    motor[motorright] = -80;
    /*If sonar doesn't have error and if less than hypotenuse of this triangle:
    *     40
    *  _______
    *  \      |
    *   \     |
    *    \    |
    *     \   | 50.
    *  64  \  |
    *       \ |
    *        \|
    *         (pos_{robot})
    * Then stop.
    */
    while (true){
        if (SensorValue[sonar] < 64 &&
                SensorValue[sonar] > 0){
            stopMotors();
            break;
        }
    }
}

/*
 * Calculates slope of new and old position
 */
int calculateSlope(int old, int new){
    return new - old;
}

/*
 * Aligns to pos.
 */
void correctPositionCenter(int pos){
    //Turn motors in other direction
    motor[motorleft] = -60;
    motor[motorright] = 60;
    while(true){
    //If then sonar is at pos, STAHP.
        if (SensorValue[sonar] == pos){
            stopMotors();
            break;
        }
    }
}
/*
 * Attempts to detect center of board after it has detected the board itself
 * Also stores a min number. Then, while repositioning, one only needs to rely on the sonar.
 * No encoders needed.
 *
 * #pragma minNum = number to store minimum value.
 */
void detectCenter(){
    //start turning motors
    motor[motorleft] = 70;
    motor[motorright] = -70;
    //make slope equal to negative number
    int slope = -1;
    //while slope is negative
    while (slope < 0){
        //get initial value of sensor at iteration
        int tempOld = SensorValue[sonar];
        wait10Msec(10);
        //get new value after 100 msec
        int tempNew = SensorValue[sonar];
        //prevent against errors from the sonar...
        if (tempNew != -1 && tempOld != -1){
            slope = calculateSlope(tempOld, tempNew);
        }
        //if x is positive, STAHP (also breaks)
        if (slope >= 0){
            stopMotors();
            //Past value must have been the minimum.
            correctPositionCenter(tempOld);
            break;
        }
    }

}

task main(){
	wait10Msec(10);
    //If aligned towards the board, then turn out of board.
  if (SensorValue[sonar] < 40){
  	rightTurn90();
  }
  //Otherwise, find the board again.
  detectBoard();
  //Now find the center.
  detectCenter();
  //Now approach the board until 10 cm.
  TillNear(10);
}
