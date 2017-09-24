/*
 Name:		NmcStepperController.h
 Created:	9/21/2017 9:56:41 PM
 Author:	Engr. Syed Rowshan Ali
 Editor:	Visual Studio 2015
*/

#ifndef _NmcStepperController_h
#define _NmcStepperController_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	//#include "WProgram.h"
#endif


class Stepper
{
public:
	//4 Pin Configuration
	Stepper(unsigned int pin1, unsigned int pin2, unsigned int pin3, unsigned int pin4);
	
	enum modes{FullStep,DoubleStep,HalfStep};

	void step(int steps);
	void stepForward(unsigned int steps);
	void stepBackward(unsigned int steps);
	void setFullRotationSteps(unsigned int FullRotationSteps);
	void setPosition(unsigned int position);
	int getPosition();
	int getAngle();
	void setAngle(unsigned int angle);
	void setSpeed(unsigned int speed);
	void setMode(modes mode);
	void MotorUnlock();
	void MotorLock();

private:
	//Store Motor Pin Counts
	byte _motorPin = 4;

	//Select motor rotation direction
	enum Direction { Forward, Backward };

	//Pins of motor
	byte _pin1;
	byte _pin2;
	byte _pin3;
	byte _pin4;
	byte _pin5;

	bool _lock = false;
	byte _currentState = 0;
	Direction _direction;

	unsigned int _fullRotationSteps = 4096;

	long _currentPosition;
	unsigned int _currentStep = 0;
	unsigned int _stepLeft = 0;
	
	unsigned int _maxSpeed = 100;
	unsigned int _currentSpeed = 1;
	
	unsigned long _coilHoldTime = 500; //millisecond;

	unsigned long _lastStateChangeTimeStamp = 0; //millisecond;

	modes _currentMode = FullStep;

	void _changeStep(byte stepNumber);
};

#endif

