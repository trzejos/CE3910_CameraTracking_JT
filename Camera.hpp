/*
 * FILENAME:	Camera.hpp
 * AUTHOR:		Josh Trzebiatowski <trzebiatowskj@msoe.edu>
 * DATE:		April 10, 2015
 */

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "I2C.hpp"

/*
 * Camera class definition
 *   Reads camera information and communicates via I2C
 */
class Camera {
private:
	I2C* i2c;
protected:

public:
	/*
	 * Constructor, initializes pointers and I2C component
	 */
	Camera();

	/*
	 * Get one frame and print it to the VGA memory
	 * Parameter debug toggles printing of I2C debug information
	 */
	unsigned char getFrame(bool debug);

	/*
	 * Get a pointer to the pixel data at a specified location
	 */
	volatile unsigned char* pixel(int row, int column);

	/*
	 * write data to the camera register specified by subaddr
	 */
	void camWrite(unsigned char subaddr,unsigned char data);

	/*
	 * read data from the camera register specified by subaddr
	 */
	unsigned char camRead(unsigned char subaddr);
};


#endif /* CAMERA_HPP_ */
