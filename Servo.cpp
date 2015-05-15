/*
 * FILENAME:	Servo.cpp
 * AUTHOR:		Josh Trzebiatowski <trzebiatowskj@msoe.edu>
 * DATE:		April 10, 2015
 */

#include "Servo.hpp"
#include "system.h"

/*
 * Constructor, which half of the Servo module to control
 */
Servo::Servo(PWMIndex index) : PWM(MYNEWPWMV2_0_BASE, index, 0.0f) { }

/*
 * Constructor, initial position
 */
Servo::Servo(PWMIndex index, float position) : PWM(MYNEWPWMV2_0_BASE, index, position) { }

/*
 * Sets position as a value from 0.0 to 1.0
 */
float Servo::getPosition() {
	return this->getDC();
}

/*
 * Gets the set position
 */
void Servo::setPosition(float position) {
	this->setDC(position);
}

