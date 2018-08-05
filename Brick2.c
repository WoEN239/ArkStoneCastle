#pragma config(Sensor, S1,     Front_Sonar,    sensorSONAR)
#pragma config(Sensor, S2,     Left_Sonar,     sensorSONAR)
#pragma config(Sensor, S3,     Right_Sonar,    sensorSONAR)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop, encoder)

long nXmitSuccesses = 0;

void setupHighSpeedLink(const bool bMaster) {
	nxtEnableHSPort();
	nxtHS_Mode = bMaster ? hsMsgModeMaster : hsMsgModeSlave;
	return;
}

void HS485Transmit() {
	static unsigned char nData[] = {0xFF, 0xFF, 0xFF};
	while (true) {
		nData[0] = SensorValue(Front_Sonar);
		nData[1] = SensorValue(Left_Sonar);
		nData[2] = SensorValue(Right_Sonar);

		if (nxtWriteRawHS(&nData[0], sizeof(nData), 0) == ioRsltSuccess)
			nXmitSuccesses += sizeof(nData);
		wait1Msec(25);
	}
}

task Transmit() {
	eraseDisplay();
	bNxtLCDStatusDisplay = true;
	setupHighSpeedLink(true);
	nxtHS_Mode = hsRawMode;
	HS485Transmit();
}

task main() {
	startTask(Transmit);
	motor[motorA] = 100;
	motor[motorB] = 100;
	motor[motorC] = 100;
	while(true) wait10Msec(1);
}
