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
	this->_pin1 = pin1;
	this->_pin2 = pin2;
	this->_pin3 = pin3;
	this->_pin4 = pin4;

	this->_motorPin = 4;
	//Set default mode to HalfStep
	this->_currentMode = HalfStep;

	//Setting Output pins
	pinMode(this->_pin1, OUTPUT);
	pinMode(this->_pin2, OUTPUT);
	pinMode(this->_pin3, OUTPUT);
	pinMode(this->_pin4, OUTPUT);

}

//This Function will let you control the stepper motor movment
void Stepper::step(int steps)
{
	//Dicised about direction
	if (steps > 0) {
		this->_direction = Direction::Forward;
	}
	else if (steps < 0) {
		this->_direction = Direction::Backward;
	}
	else {
		return;
	}

	//Record the left steps
	this->_stepLeft = abs(steps);
	while (_stepLeft > 0)
	{
		//current timestamp
		unsigned long now = micros();

		//Check if coil hold time is over
		if (now - this->_lastStateChangeTimeStamp >= this->_coilHoldTime)
		{
			//Record timestamp
			this->_lastStateChangeTimeStamp = micros();

			if (this->_direction == Direction::Forward)
			{
				this->_currentStep++;
				this->_currentPosition++;
				if (this->_currentPosition == 2147483647L) {
					this->_currentPosition = -2147483648L;
				}
				//Reset current position to 0
				if (this->_currentStep == this->_fullRotationSteps) {
					this->_currentStep = 0;
				}
			}
			else if(this->_direction == Direction::Backward)
			{
				//Reset current position to max
				if (this->_currentStep == 0) {
					this->_currentStep = this->_fullRotationSteps;
				}
				this->_currentStep--;
				this->_currentPosition--;
				if (this->_currentPosition == -2147483648L) {
					this->_currentPosition = 2147483647L;
				}
			}
		
			//Decrease the stepLeft
			this->_stepLeft--;

			// Step the motor
			if (this->_motorPin == 4) {
				switch (this->_currentMode)
				{
				case modes::HalfStep:
					this->_changeStep(this->_currentStep % 8);
					break;
				case modes::FullStep:
					this->_changeStep(this->_currentStep % 4);
					break;
				}
			}
		}
	}
}

//This function will move the motor forward
void Stepper::stepForward(unsigned int steps)
{

}

//This function will move the motor backward
void Stepper::stepBackward(unsigned int steps)
{
	
}

void Stepper::setFullRotationSteps(unsigned int FullRotationSteps)
{
	if (this->_currentMode == FullStep) {
		this->_fullRotationSteps = FullRotationSteps;
	}else if(this->_currentMode == HalfStep){
		this->_fullRotationSteps = FullRotationSteps * 2;
	}
}
//This will rotate the stepper to a fixed position
void Stepper::setPosition(unsigned int position)
{
		this->step( position - this->_currentPosition);
	//Else it is in current position
}

int Stepper::getPosition()
{
	return this->_currentPosition;
}

int Stepper::getAngle()
{
	int degree = map(this->_currentStep,  0, this->_fullRotationSteps - 1,0, 359);
	return degree;
}

void Stepper::setAngle(unsigned int angle)
{
	int position = map(angle, 0, 359, 0, this->_fullRotationSteps - 1);
	this->step(position - this->_currentPosition);
}

void Stepper::setSpeed(unsigned int speed)
{

	this->_coilHoldTime = 10000 / speed;
}

void Stepper::setMode(modes mode)
{
	this->_currentMode = mode;
}

//This function will cut of power from motor
void Stepper::MotorUnlock()
{
	//Power of the Motor
	this->_lock = false;
	digitalWrite(_pin1, LOW);
	digitalWrite(_pin2, LOW);
	digitalWrite(_pin3, LOW);
	digitalWrite(_pin4, LOW);
}

//This function will lock the motor at current position
void Stepper::MotorLock()
{
	this->_lock = true;
	if (this->_currentMode == HalfStep) {
		_changeStep(this->_currentStep % 8);
	}
	if (this->_currentMode == FullStep) {
		_changeStep(this->_currentStep % 4);
	}
}

//This function will change current in coil
void Stepper::_changeStep(byte stepNumber) {
	//Switching position
	if(this->_currentMode == HalfStep)
	{
		switch (stepNumber) {
		case 0:
			digitalWrite(this->_pin1, HIGH);
			digitalWrite(this->_pin2, HIGH);
			digitalWrite(this->_pin3, LOW);
			digitalWrite(this->_pin4, LOW);
			break;
		case 1:
			digitalWrite(this->_pin1, LOW);
			digitalWrite(this->_pin2, HIGH);
			digitalWrite(this->_pin3, LOW);
			digitalWrite(this->_pin4, LOW);
			break;
		case 2:
			digitalWrite(this->_pin1, LOW);
			digitalWrite(this->_pin2, HIGH);
			digitalWrite(this->_pin3, HIGH);
			digitalWrite(this->_pin4, LOW);
			break;
		case 3:
			digitalWrite(this->_pin1, LOW);
			digitalWrite(this->_pin2, LOW);
			digitalWrite(this->_pin3, HIGH);
			digitalWrite(this->_pin4, LOW);
			break;
		case 4:
			digitalWrite(this->_pin1, LOW);
			digitalWrite(this->_pin2, LOW);
			digitalWrite(this->_pin3, HIGH);
			digitalWrite(this->_pin4, HIGH);
			break;
		case 5:
			digitalWrite(this->_pin1, LOW);
			digitalWrite(this->_pin2, LOW);
			digitalWrite(this->_pin3, LOW);
			digitalWrite(this->_pin4, HIGH);
			break;
		case 6:
			digitalWrite(this->_pin1, HIGH);
			digitalWrite(this->_pin2, LOW);
			digitalWrite(this->_pin3, LOW);
			digitalWrite(this->_pin4, HIGH);
			break;
		case 7:
			digitalWrite(this->_pin1, HIGH);
			digitalWrite(this->_pin2, LOW);
			digitalWrite(this->_pin3, LOW);
			digitalWrite(this->_pin4, LOW);
			break;
		}
	}
	//FullStep
	if (this->_currentMode == modes::FullStep) {
		switch (stepNumber) {
		case 0:
			digitalWrite(this->_pin1, HIGH);
			digitalWrite(this->_pin2, HIGH);
			digitalWrite(this->_pin3, LOW);
			digitalWrite(this->_pin4, LOW);
			break;
		case 1:
			digitalWrite(this->_pin1, LOW);
			digitalWrite(this->_pin2, HIGH);
			digitalWrite(this->_pin3, HIGH);
			digitalWrite(this->_pin4, LOW);
			break;
		case 2:
			digitalWrite(this->_pin1, LOW);
			digitalWrite(this->_pin2, LOW);
			digitalWrite(this->_pin3, HIGH);
			digitalWrite(this->_pin4, HIGH);
			break;
		case 3:
			digitalWrite(this->_pin1, HIGH);
			digitalWrite(this->_pin2, LOW);
			digitalWrite(this->_pin3, LOW);
			digitalWrite(this->_pin4, HIGH);
			break;
		}
	}
}