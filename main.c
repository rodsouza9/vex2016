#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl1,  rampSolenoidA,  sensorDigitalOut)
#pragma config(Sensor, dgtl2,  rampSolenoidB,  sensorDigitalOut)
#pragma config(Sensor, dgtl12, ShooterReadyLED, sensorLEDtoVCC)
#pragma config(Sensor, I2C_1,  testEncoder,    sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           mFrontLeft,    tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           mShooter2,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           mShooter3,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           mShooter4,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           mBackLeft,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           mFrontRight,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           mShooter7,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           mShooter8,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           mShooter9,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          mBackRight,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)
//*!!Code automatically generated by 'ROBOTC' configuration wizard, cleaned up by me!!*//

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)
#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

//--------------------Button Mapping--------------------//
//Drive axis
#define joyDriveA Ch1 //First axis for the drive joysticks
#define joyDriveB Ch4 //Second axis for the drive joysticks
#define joyDriveC Ch3 //Third axis for the drive joysticks

//Shooter buttons
#define joyShooterZero Btn5U //Set the shooter speed to zero (E-STOP)
#define joyShooterIncU Btn6D //Increment shooter speed
#define joyShooterIncD Btn5D //Decrement the shooter speed
#define joyShooterFull Btn6U //Set the shooter speed to max (127)

//Pneumatics Buttons
#define joyPneuToggle  Btn8U //Activates the ramp solenoids

//--------------------Constants--------------------//
float optimalShooterSpd = 36.8; //Optimal speed for firing
float shooterIncrement = 0.2; //How much to increment or decrement speed each tick

//--------------------Variables--------------------//
float shooterMotorRaw = 0; //stores the current set speed for the shooter motors
int lastSpeedValue = 0;    //The previous speed of the shooter
int currentSpeedValue = 0; //The current speed of the shooter
float shooterAverage = 0;  //Average speed of the shooter
float shooterTarget = 0;    //The target for the shooter PID loop
bool isShooterReady = false;

//--------------------Helper Functions-------------//
//Helper function for setting all drive motors in one command
void setDriveMotors(int fL, int fR, int bL, int bR) {
   motor[mFrontLeft] = fL;
   motor[mFrontRight] = fR;
   motor[mBackLeft] = bL;
   motor[mBackRight] = bR;
}

//Helper function for setting all shooter motors in one command, with correct polarity
void setShooterMotors(int power) {
   motor[mShooter2] = power;
   motor[mShooter3] = -power;
   motor[mShooter4] = -power;
   motor[mShooter7] = -power;
   motor[mShooter8] = power;
   motor[mShooter9] = power;
}

void calculateShooter(){
	wait1Msec(50);
  lastSpeedValue = currentSpeedValue;              //Get the prevoius speed of the shooter
  currentSpeedValue = SensorValue[testEncoder];    //Get the current speed of the shooter
  int speed = (currentSpeedValue-lastSpeedValue);  //Calculate the change in position between the last cycle
  if (speed > 50) {speed=50;}                      //Clamp the speed to make sure it doesn't go over 20/s
  else if (speed < -50) {speed=-50;}               // (sometimes it generates erronously high values)

  shooterAverage = (shooterAverage + speed) / 2.0; //Get an average
  float error = shooterTarget-speed;            //Calculate an error based on the target

  shooterMotorRaw=shooterMotorRaw+(error*0.05);           //Add 20% of the error to the motor power output
  if (shooterMotorRaw > 127) {shooterMotorRaw=127;}      //Clamp the motor output so it doesn't go above 127 or below -127
  else if (shooterMotorRaw < -127) {shooterMotorRaw=-127;}

  isShooterReady = (shooterAverage > shooterTarget-1.5 && shooterAverage < shooterTarget+1.5);
  SensorValue[ShooterReadyLED] = isShooterReady;

  //Debug output!
  writeDebugStreamLine("target: %-4f speed: %-4f Motors: %i Battery: %f", shooterTarget, shooterAverage, shooterMotorRaw, nImmediateBatteryLevel/1000.0);

}


//--------------------Initalization Code--------------------//
void pre_auton() {
  bStopTasksBetweenModes = true; //Set false for user tasks to run between mode switches
}

//--------------------Autonomous mode--------------------//
task autonomous() {
	AutonomousCodePlaceholderForTesting();  // Remove this function call once you have "real" code.
}

//--------------------Manual Control Loop--------------------//
task usercontrol() {
	//Main operator control loop
  while(true){

	  int x = vexRT[joyDriveA];
	  int y = vexRT[joyDriveB];
  	int z = vexRT[joyDriveC];

  	//Basic configuration for 4 meccanum wheel drive
  	setDriveMotors(x + z + y,
	                 x - z - y,
	                 x + z - y,
	                 x - z + y);

	  //bring the shooter to a full stop permanently
  	if (vexRT[joyShooterZero] == 1) {
	  	shooterTarget = 0;
	  	shooterMotorRaw = 0;
	  	setShooterMotors(0);

	  //Increment the target
  	}else if (vexRT[joyShooterIncU] == 1) {
	    shooterTarget += shooterIncrement;

	  //Decrement the target
	  }else if (vexRT[joyShooterIncD] == 1) {
	    shooterTarget -= shooterIncrement;

	  //Set the shooter speed to the optimal speed
    }else if (vexRT[joyShooterFull] == 1) {
		  shooterTarget = optimalShooterSpd;

    } //End shooter button if statements

    calculateShooter();                //Calculate the shooter's speed and the motor speed
    setShooterMotors(shooterMotorRaw); //set the shooter motor's speed

    SensorValue[rampSolenoidA] = SensorValue[joyPneuToggle];
    SensorValue[rampSolenoidB] = SensorValue[joyPneuToggle];


  } //End main program loop
}
