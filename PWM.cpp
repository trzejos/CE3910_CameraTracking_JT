/*
 * FILENAME:	PWM.cpp
 * AUTHOR:		Josh Trzebiatowski <trzebiatowskj@msoe.edu>
 * DATE:		April 10, 2015
 */

#include "PWM.hpp"
#include "Math.hpp"
#include "PWMIndex.h"

#include <io.h>
#include <stdio.h>

#define MAX_CNT 20000

#define OFFSET_OCRA 0
#define OFFSET_OCRB 2

/*
 * Constructor, base address and which half of the PWM
 * module to control
 */
PWM::PWM(int address, PWMIndex index) : PWM(address, index, 0.0f) { }

/*
 * Constructor, also specify initial duty cycle
 */
PWM::PWM(int address, PWMIndex index, float dc) : address(address), index(index) {
	setDC(dc);
}

/*
 * Sets duty cycle as a value from 0.0 to 1.0
 */
void PWM::setDC(volatile float dc) {
	dc = Math::clamp<float>(dc, 0.0, 1.0);

	this->dc = dc;
	volatile int count = (int)(dc*MAX_CNT);

	//volatile short* ptr;

	switch(index) {
	case PWMINDEX_A:
		IOWR_16DIRECT(address, OFFSET_OCRA, count);
		//ptr = (volatile short*)(address + OFFSET_OCRA);
		break;
	case PWMINDEX_B:
		IOWR_16DIRECT(address, OFFSET_OCRB, count);
		//ptr = (volatile short*)(address + OFFSET_OCRB);
		break;
	}

	//*ptr = count;
}

/*
 * Gets the set duty cycle
 */
float PWM::getDC() {
	return dc;
}
