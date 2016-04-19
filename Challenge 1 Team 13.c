/**
* Can Huynh
* Minh Vu
* TCSS 437
* Spring 2016
**/

#pragma config(Sensor, S1,     touchRight,          sensorEV3_Touch)
#pragma config(Sensor, S4,     touchLeft,          sensorEV3_Touch)
#pragma config(Motor,  motorD,          motorLeft,     tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorA,          motorRight,    tmotorEV3_Large, PIDControl, encoder)

#define RANDOM_TIME_IN_MS 3

#define FORWARD_TURNING_FAST 75
#define FORWARD_TURNING_SLOW 25

#define BACKWARD -50
#define FORWARD 50

#define REACT_TURNING_FAST 10
#define REACT_TURNING_SLOW -20

#define RANDOM_FORWARD_PROBABILITY 8

// Counters for the number of times a direction was turned.
typedef struct {
	int leftCounter;
	int rightCounter;
} turnCounter;


// returns a random number from 100 to (RANDOM_TIME_IN_MS + 1) * 100;
int randomTime() {
	return (random(RANDOM_TIME_IN_MS) + 1) * 100 ;
}

// Moves the robot at specified speed
void moving(tMotor left, tMotor right, int velocity_left, int velocity_right) {
	setMotorSpeed(left, velocity_left);
	setMotorSpeed(right, velocity_right);
}

// Controls the turn of robot with random left/right turns and random duration for turns.
// For each turn a counter is incremented in the direction it turned 
// and decrements the opposite direction counter. This bias allows for the robot to head
// towards a certain direction and prevents it from turning around by limiting the amount of 
// successive turns in a single direction.
void wanderTurn(turnCounter movingRobot){
	int random_value = random(3); // Random value in the interval [0, 3]

	if (random_value >= 1 - movingRobot.leftCounter) { // Turn Right
		moving(motorLeft, motorRight, FORWARD_TURNING_FAST, FORWARD_TURNING_SLOW);
		movingRobot.leftCounter--;
		movingRobot.rightCounter++;
	} else if (random_value < 1 + movingRobot.rightCounter) { // Turn Left
		moving(motorLeft, motorRight, FORWARD_TURNING_SLOW, FORWARD_TURNING_FAST);
		movingRobot.rightCounter--;
		movingRobot.leftCounter++;
	}
	sleep(randomTime()); // Random amount of turning.
}

// Reaction to touch sensor being bumped
void bumpReact(int leftTouchFlag, int rightTouchFlag, turnCounter movingRobot) {
	while (rightTouchFlag || leftTouchFlag){
		// Resets counters making robot have bias towards a new direction.
		movingRobot.leftCounter = 0;
		movingRobot.rightCounter = 0;
		// If only right was touched point turn left
		if (rightTouchFlag && !leftTouchFlag) {
			moving(motorLeft, motorRight, REACT_TURNING_SLOW, REACT_TURNING_FAST);
			// If only left was touched point turn right
			} else if (leftTouchFlag && !rightTouchFlag) {
				moving(motorLeft, motorRight, REACT_TURNING_FAST, REACT_TURNING_SLOW);
			} else { // Both bumpers
			// the robot makes a sound
			playTone(600, 75);
			// Back up a short distance
			moving(motorLeft, motorRight, BACKWARD, BACKWARD);
			sleep(1500);
			// pause for 2 seconds without moving
			moving(motorLeft, motorRight, 0, 0);
			sleep(1500);
			// then turn in a random direction
			// moving(motorLeft, motorRight, 50, -25);
			wanderTurn(movingRobot);
		}
		// detect whether the sensor is hitting again
		sleep(500);
		rightTouchFlag = SensorValue(touchRight);
		leftTouchFlag = SensorValue(touchLeft);
		sleep(250);
	}
}

// Detects bumps for a certain period of time so to allow for 
// reaction rather than continue moving into an obstacle.
void detectBump(turnCounter movingRobot, int time) {
	clearTimer(T1);
	while (time1[T1] < time) {
		// Touch flags
		int rightTouchFlag = SensorValue(touchRight);
		int leftTouchFlag = SensorValue(touchLeft);
		sleep(10); // To give time for both sensor to touch.
		bumpReact(leftTouchFlag, rightTouchFlag, movingRobot);
	}
}

task main() {
	// Initializes turn counters
	turnCounter movingRobot;
	movingRobot.leftCounter = 0;
	movingRobot.rightCounter = 0;
	
	while (true) {
		// Move forward for a random period of time while detecting for bumps.
		moving(motorLeft, motorRight, FORWARD , FORWARD);

		detectBump(movingRobot, randomTime() - 100);
		// 20% chance to do a random turn and 80% to move continue forward
		int random_value = random(10);
		if (random_value > RANDOM_TURN_PROBABILITY) {
			wanderTurn(movingRobot);
		}
	}
}
