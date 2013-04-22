#pragma config(Sensor, dgtl1,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl6,  sonar,          sensorSONAR_cm)
#pragma config(Sensor, dgtl10, led,            sensorLEDtoVCC)
#pragma config(Sensor, dgtl12, led2,           sensorLEDtoVCC)
#pragma config(Motor,  port2,           motorleft,     tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port3,           motorright,    tmotorServoContinuousRotation, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void initMotors(int x){
	motor[motorleft] = x;
	motor[motorright] = x;
}

void stopMotors(){
	initMotors(0);
}

void sonarMeasure(){
 wait10Msec(100);
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
   startTask(autoCorrect);
  wait1Msec(1);
  }
 motor[motorleft] = 0;
 motor[motorright] = 0;
 wait1Msec(1000);
}
void leftTurn90(){
  SensorValue[rightEncoder] = 0;
 SensorValue[leftEncoder] = 0;
   motor[motorleft] = -90;
 motor[motorright] = 90;
  while(SensorValue[leftEncoder] <=180){
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
void rightTurn180(){
  for (int i = 0; i < 2; i++){
  rightTurn90();
 }
}
void leftTurn180(){
 SensorValue[rightEncoder] = 0;
SensorValue[leftEncoder] = 0;
  motor[motorleft] = -90;
motor[motorright] = 90;
while(SensorValue[leftEncoder] <=370){
  wait1Msec(1);
}
 motor[motorleft] = 0;
  motor[motorright] = 0;
   wait1Msec(1000);
}
void goStraight(){
   motor[motorleft] = 127;
 motor[motorright] = 127;
SensorValue[rightEncoder] = 0;
SensorValue[leftEncoder] = 0;
 while (SensorValue[leftEncoder] <600){
 if (SensorValue[leftEncoder]-SensorValue[rightEncoder]<-10){
  motor[motorleft]-=2;
motor[motorright]+=2;
}else if (SensorValue[leftEncoder]-SensorValue[rightEncoder]>10){
 motor[motorleft]+=2;
motor[motorright]-=2;
 }
wait1Msec(150);
}
}

void Act1(){
   motor[motorleft]= 100;
motor[motorright] = 100;

while(SensorValue[leftEncoder] >=-800){
wait1Msec(1);
 }
motor[motorleft]= 100;
motor[motorright] = 100;
 wait1Msec(100);
}
void Act2(){
  motor[motorleft]= -100;
    motor[motorright] = -100;
while(SensorValue[leftEncoder] >= 600){
  wait1Msec(1);
 }
}
void Act3(){
motor[motorleft] = -90;
 motor[motorright] = 90;
while(SensorValue[leftEncoder] <=180){
   wait1Msec(1);
}
}

void Act4(){
 motor[motorleft] = 60;
 wait1Msec(4000);
  motor[motorleft] = 0;
 wait10Msec(100);
 SensorValue[leftEncoder] = 0;
  motor[motorleft] = 40;
 wait10Msec(300);
}

void calculateSlope(){
	SensorValue[leftEncoder] = 0;
	motor[motorleft] = 50;
	motor[motorright] = -90;
	SensorValue[led] = 1;
	while(true){
	int oldVal = SensorValue[sonar];
	wait10Msec(1);
	int newVal = SensorValue[sonar];
	if (newVal - oldVal > 0){
		//stopMotors();

		/*motor[motorleft] = -50;
		motor[motorright] = 60;
		int x = 3;
		SensorValue[leftEncoder] = 0;
		while (SensorValue[leftEncoder]<x){
				SensorValue[led2] = 1;
		wait1Msec(1);
		}*/
		break;
	}
}
}
void detectBoard(){
 wait10Msec(100);
 motor[motorleft] = 80;
 motor[motorright] = -80;
 while (true){
   wait1Msec(1);
   if(SensorValue[sonar] < 50 &&
     SensorValue[sonar] > 0){
       break;
     }
 }
 motor[motorleft] = 0;
 motor[motorright] = 0;
}


task main(){
	wait10Msec(10);
  if (SensorValue[sonar] < 40){
  	rightTurn90();
  }
  detectBoard();
  calculateSlope();
  TillNear(10);
}
