/*
 * FILENAME:	Servo.hpp
 * AUTHOR:		Josh Trzebiatowski <trzebiatowskj@msoe.edu>
 * DATE:		April 10, 2015
 */

#ifndef SERVO_HPP
#define SERVO_HPP

#include "PWM.hpp"

/*
 * Servo class, extension of PWM class, provides access
 * to PWM methods
 */
class Servo : public PWM {
private:
	int pos;

protected:

public:
	/*
	 * Constructor, which half of the Servo module to control
	 */
	Servo(PWMIndex index);

	/*
	 * Constructor, initial position
	 */
	Servo(PWMIndex index, float position);

	/*
	 * Sets position as a value from 0.0 to 1.0
	 */
	void setPosition(float position);

	/*
	 * Gets the set position
	 */
	float getPosition();
};

#endif
