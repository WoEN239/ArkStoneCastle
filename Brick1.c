#pragma config(Sensor, S1,     Field_Color_Sensor, sensorI2CCustom)
#pragma config(Sensor, S2,     Left_Color_Sensor, sensorI2CCustom)
#pragma config(Sensor, S3,     Right_Color_Sensor, sensorI2CCustom)
#pragma config(Hubs,   S4, HTServo,  none,     none,     none)
#pragma config(Motor,  motorB,          Left_Sep_Motor, tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          Right_Sep_Motor, tmotorNXT, PIDControl, encoder)
#pragma config(Servo,  srvo_S4_C1_1,    topor,                tServoStandard)
#pragma config(Servo,  srvo_S4_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S4_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S4_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S4_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S4_C1_6,    servo6,               tServoNone)

#include "drivers\hitechnic-colour-v2.h"

short teamcolor, enemycolor;
long targetL, targetR, errL, errR;

#define white 0
#define red   1
#define blue  2
short Color(int value)
{
	switch (value)
	{
		//blue
	case 12:
		return blue;
		break;
	case 3:
		return blue;
		break;
		//red
	case 7:
		return red;
		break;
	case 8:
		return red;
		break;
	case 10:
		//white
	case 14:
		return white;
		break;
	default:
		return white;
		break;
	}
}

int FieldSensorTeamColor;
task rightSeparator() {
	while(true) {

		if (Color(HTCS2readColor(Field_Color_Sensor))!=FieldSensorTeamColor)
		{
			if (HTCS2readColor(Right_Color_Sensor) == teamcolor) {
				targetR -= 120;
				wait10Msec(15);
				motor[Right_Sep_Motor] = -60;
			}
			else if (HTCS2readColor(Right_Color_Sensor) == enemycolor) {
				targetR += 120;
				wait10Msec(15);
				motor[Right_Sep_Motor] = 60;
			}
			clearTimer(T2);
			while(nMotorEncoder[Right_Sep_Motor] != targetR) {
				if(time100[T2]>10) {
					targetR-=120*sgn(targetR);
					clearTimer(T2);
				}
				errR = targetR - nMotorEncoder[Right_Sep_Motor];
				motor[Right_Sep_Motor] = 2 * sgn(errR) + errR / 2;
			}
		}
	}
}

task leftSeparator() {
	while(true) {
		if (Color(HTCS2readColor(Field_Color_Sensor))!=FieldSensorTeamColor)
		{
			if (HTCS2readColor(Left_Color_Sensor) == teamcolor) {
				targetL -= 120;
				wait10Msec(15);
				motor[Left_Sep_Motor] = -60;
			}
			else if (HTCS2readColor(Left_Color_Sensor) == enemycolor) {
				targetL += 120;
				wait10Msec(15);
				motor[Left_Sep_Motor] = 60;
			}
			ClearTimer(T1);
			while(nMotorEncoder[Left_Sep_Motor] != targetL) {
				if(time100[T1]>10) {
					targetL -= 120 * sgn(targetL);
					ClearTimer(T1);
				}
				errL = targetL - nMotorEncoder[Left_Sep_Motor];
				motor[Left_Sep_Motor] = 2 * sgn(errL) + errL / 2;
			}
		}
	}
}

task Topor() {
	while(true){
		if (Color(HTCS2readColor(Field_Color_Sensor)) != FieldSensorTeamColor)
		{
			wait1Msec(666);
			if (Color(HTCS2readColor(Field_Color_Sensor)) != FieldSensorTeamColor)
				servo[topor] = 0;
		}
		else servo[topor] = 100;
	}
}
task main() {
	switch(Color(HTCS2readColor(Field_Color_Sensor)))
	{
	case red:
		teamcolor = 8; //red
		enemycolor = 2; //blue
		FieldSensorTeamColor = red;
		break;
	case blue:
		teamcolor = 2; //blue
		enemycolor = 8; //red
		FieldSensorTeamColor = blue;
		break;
	default:
		nxtDisplayCenteredBigTextLine(3,"ERR");
		PlaySound(soundException);
		break;
	}
	StartTask(Topor);

	StartTask(leftSeparator);
	StartTask(rightSeparator);
	while (true) wait10Msec(1);
}
