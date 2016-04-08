#pragma config(Sensor, S1,     touchRight,          sensorEV3_Touch)
#pragma config(Sensor, S4,     touchLeft,          sensorEV3_Touch)
#pragma config(Motor,  motorD,          motorLeft,     tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorA,          motorRight,    tmotorEV3_Large, PIDControl, encoder)


void moving(tMotor left, tMotor right, int velocity_left, int velocity_right) {
	setMotorSpeed(left, velocity_left);
	setMotorSpeed(right, velocity_right);
}

void movingRandom(){
	int random_value = random(5);
	if (random_value == 1) {
		moving(motorLeft, motorRight, 30, 0);
	} else if (random_value == 2) {
		moving(motorLeft, motorRight, 0, 30);
	}
	else {
		moving(motorLeft, motorRight, 50, 50);
	}
}

task main() {

	while (true) {
		// Move forward
		movingRandom();
		// Touch flags
		int rightTouchFlag = SensorValue(touchRight);
		int leftTouchFlag = SensorValue(touchLeft);
		sleep(200);
		if (rightTouchFlag || leftTouchFlag){
			// If only right was touched point turn left
			if (rightTouchFlag && !leftTouchFlag) {
				setMotorSpeed(motorLeft, -20);
				sleep(500);
			// If only lrft was touched point turn right
			} else if (leftTouchFlag && !rightTouchFlag) {
				setMotorSpeed(motorRight, -20);
				sleep(500);
			} else { // Both bumpers
				moving(motorLeft, motorRight, -10, -10);
				sleep(1000);
				moving(motorLeft, motorRight, 50, -50);
				sleep(500);
			}
		}
	}
}
