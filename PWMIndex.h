/*
 * FILENAME:	PWMIndex.h
 * AUTHOR:		Josh Trzebiatowski <trzebiatowskj@msoe.edu>
 * DATE:		April 10, 2015
 */

#ifndef PWMINDEX_H
#define PWMINDEX_H

/*
 * PWMIndex enum, determines which half of
 * a PWM controller to use
 */
enum PWMIndex {
	PWMINDEX_A, PWMINDEX_B
};

typedef enum PWMIndex PWMIndex;

#endif
