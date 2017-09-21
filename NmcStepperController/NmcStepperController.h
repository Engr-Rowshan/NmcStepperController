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
	Stepper(unsigned int pin1, unsigned int pin2, unsigned int pin3, unsigned int pin4);
	enum modes{FullStep,DoubleStep,HalfStep};

	void step(int steps);
	void stepForward(unsigned int steps);
	void stepBackward(unsigned int steps);
	void setPosition(unsigned int position);
	int getPosition();
	int getAngle();
	void setAngle(unsigned int angle);
	void setSpeed(unsigned int speed);
	void setMode(modes mode);

private:
	enum Direction { Forward, Backward };
	unsigned int _pin1;
	unsigned int _pin2;
	unsigned int _pin3;
	unsigned int _pin4;
	unsigned int _fullRotationSteps = 4096;
	unsigned int _currentPosition = 0;
	unsigned int _maxSpeed = 100;
	unsigned int _speed = 1;
	unsigned int _currentState = 0;
	unsigned int _coilHoldTime = 5; //millisecond;

	modes _currentMode = FullStep;
	bool _lock = false;
	void _unlock();
	void _lock();
	void _changeStep(Direction direction);
	void _changeState();
};

#endif

