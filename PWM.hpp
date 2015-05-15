/*
 * FILENAME:	PWM.hpp
 * AUTHOR:		Josh Trzebiatowski <trzebiatowskj@msoe.edu>
 * DATE:		April 10, 2015
 */

#ifndef PWM_HPP
#define PWM_HPP

#include "PWMIndex.h"

/*
 * PWM class, controls one half of a PWM component
 */
class PWM {
private:
	float dc;
	int address;
	PWMIndex index;

protected:

	/*
	 * Constructor, base address and which half of the PWM
	 * module to control
	 */
	PWM(int address, PWMIndex index);

	/*
	 * Constructor, also specify initial duty cycle
	 */
	PWM(int address, PWMIndex index, float dc);

public:

	/*
	 * Sets duty cycle as a value from 0.0 to 1.0
	 */
	void setDC(float dc);

	/*
	 * Gets the set duty cycle
	 */
	float getDC();

};

#endif
