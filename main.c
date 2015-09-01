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

//This is version 2!!!

//Constants
#define targetShooterPos = 127;

float shooterSpeed = 0;

//Helper function for setting all drive motors in one command
void setAllMotors(float fL, float fR, float bL, float bR) {
   motor[mFrontLeft] = fL;
   motor[mFrontRight] = fR;
   motor[mBackLeft] = bL;
   motor[mBackRight] = bR;
}

void setShooterMotors(float power) {
   motor[mShooter2] = power;
   motor[mShooter3] = -power;
   motor[mShooter4] = -power;
   motor[mShooter7] = -power;
   motor[mShooter8] = power;
   motor[mShooter9] = power;
}


task main()
{

while(true){
	float x = vexRT[Ch1];
	float y = vexRT[Ch4];
	float z = vexRT[Ch3];
	setAllMotors(x + z + y,
	             x - z - y,
	             x + z - y,
	             x - z + y);

	float newSpeed = shooterSpeed;

	if (vexRT[Btn5U] == 1) {
		shooterSpeed = 0;
		newSpeed = 0;
	}else if (vexRT[Btn5D] == 1) {
	  shooterSpeed += 10;
	  newSpeed = shooterSpeed;
   }

	if (vexRT[Btn5U] == 1) {
		newSpeed = 127;
	}else if (vexRT[Btn5D] == 1) {
	  shooterSpeed -= 10;
	  newSpeed = shooterSpeed;
	 }

	setShooterMotors(newSpeed);

}

}
