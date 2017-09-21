/*
 Name:		NmcStepperController.cpp
 Created:	9/21/2017 9:56:41 PM
 Author:	engrr
 Editor:	http://www.visualmicro.com
*/
#include "arduino.h"
#include "NmcStepperController.h"

Stepper::Stepper(unsigned int pin1, unsigned int pin2, unsigned int pin3, unsigned int pin4)
{
	_pin1 = pin1;
	_pin2 = pin2;
	_pin3 = pin3;
	_pin4 = pin4;
	
	//Setting Output pins
	pinMode(pin1, OUTPUT);
	pinMode(pin2, OUTPUT);
	pinMode(pin3, OUTPUT);
	pinMode(pin4, OUTPUT);

	//Set default mode to HalfStep
	_currentMode = HalfStep;
}

//This Function will let you control the stepper motor movment
void Stepper::step(int steps)
{
	if (steps > 0) {
		stepForward(steps);
	}
	else if (steps < 0) {
		stepBackward(steps);
	}
	else {
		//Do Nothing
	}
}

//This function will move the motor forward
void Stepper::stepForward(unsigned int steps)
{
	for (unsigned int i; i <= steps; i++) {
		if (_currentMode == FullStep) {
			_currentPosition += 2;
		}
		else if (_currentMode == HalfStep) {
			_currentPosition++;
		}
		_changeStep(Forward);
	}
}

//This function will move the motor backward
void Stepper::stepBackward(unsigned int steps)
{
	for (unsigned int i; i <= steps; i++) {
		if (_currentMode == FullStep) {
			_currentPosition -= 2;
		}
		else if (_currentMode == HalfStep) {
			_currentPosition--;
		}
		_changeStep(Backward);
	}
}

//This function will cut of power from motor
void Stepper::_unlock()
{
	//Power of the Motor
	digitalWrite(_pin1, LOW);
	digitalWrite(_pin2, LOW);
	digitalWrite(_pin3, LOW);
	digitalWrite(_pin4, LOW);
}

//This function will lock the motor at current position
void Stepper::_lock()
{
	_changeState();
}

//This function will move the motor 1 step at a time
//Over function will fix the direction
void Stepper::_changeStep(Direction direction) {
	
	if (direction == Forward) {
		if (_currentMode == HalfStep) {
			if (_currentState == 0 || _currentState == 8) {
				_currentState = 1;
			}
			else {
				_currentState++;
			}
		}
		else if(_currentMode == FullStep) {
			if (_currentState == 0 || _currentState == 8) {
				_currentState = 2;
			}
			else {
				_currentState += 2;
			}
		}
	}
	else if (direction == Backward) {
		if (_currentMode == HalfStep) {
			if (_currentState == 0 || _currentState == 1 || _currentState == 2) {
				_currentState = 8;
			}
			else {
				_currentState--;
			}
		}
		else if (_currentMode == FullStep) {
			if (_currentState == 0 || _currentState == 1 || _currentState == 8) {
				_currentState = 8;
			}
			else {
				_currentState -= 2;
			}
		}
	}
	_changeState();
}

//This function will change current in coil
void Stepper::_changeState() {
	//Switching position
	switch (_currentState) {
	case 1:
		digitalWrite(_pin1, HIGH);
		digitalWrite(_pin2, LOW);
		digitalWrite(_pin3, LOW);
		digitalWrite(_pin4, LOW);
		break;
	case 2:
		digitalWrite(_pin1, HIGH);
		digitalWrite(_pin2, HIGH);
		digitalWrite(_pin3, LOW);
		digitalWrite(_pin4, LOW);
		break;
	case 3:
		digitalWrite(_pin1, LOW);
		digitalWrite(_pin2, HIGH);
		digitalWrite(_pin3, LOW);
		digitalWrite(_pin4, LOW);
		break;
	case 4:
		digitalWrite(_pin1, LOW);
		digitalWrite(_pin2, HIGH);
		digitalWrite(_pin3, HIGH);
		digitalWrite(_pin4, LOW);
		break;
	case 5:
		digitalWrite(_pin1, LOW);
		digitalWrite(_pin2, LOW);
		digitalWrite(_pin3, HIGH);
		digitalWrite(_pin4, LOW);
		break;
	case 6:
		digitalWrite(_pin1, LOW);
		digitalWrite(_pin2, LOW);
		digitalWrite(_pin3, HIGH);
		digitalWrite(_pin4, HIGH);
		break;
	case 7:
		digitalWrite(_pin1, LOW);
		digitalWrite(_pin2, LOW);
		digitalWrite(_pin3, LOW);
		digitalWrite(_pin4, HIGH);
		break;
	case 8:
		digitalWrite(_pin1, HIGH);
		digitalWrite(_pin2, LOW);
		digitalWrite(_pin3, LOW);
		digitalWrite(_pin4, HIGH);
		break;
	}
	delay(_coilHoldTime); //Delay time to let the motor shapt move a little bit
}