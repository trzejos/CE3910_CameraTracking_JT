/*
 * FILENAME:	CameraMount.hpp
 * AUTHOR:		Josh Trzebiatowski <trzebiatowskj@msoe.edu>
 * DATE:		April 10, 2015
 */

#ifndef CAMERAMOUNT_HPP
#define CAMERAMOUNT_HPP

#include "Servo.hpp"
#include "Camera.hpp"

/*
 * CameraMount class, ties pan/tilt servos and camera into
 * one complete package
 */
class CameraMount {
private:
	Servo* servoPan;
	Servo* servoTilt;
	Camera* camera;
	unsigned char threshold;
	float lastPan;
	float lastTilt;

protected:

public:

	/*
	 * Constructor, Initialize servos and camera, set defaults
	 */
	CameraMount();

	/*
	 * Set the pan servo to a position specified by degrees
	 */
	void pan(float data);

	/*
	 * Set the tilt servo to a position specified by degrees
	 */
	void tilt(float data);

	/*
	 * Print a camera frame to the VGA memory
	 */
	void getCameraFrame(bool debug);

	void testFrame();

	void updateThreshold();

	/*
	 * Adjust servos based on last frame captured
	 */
	void adjustServos();

	/*
	 * Read pixel data from the VGA memory
	 */
	char getCameraPixel(int row, int column);

	/*
	 * Write a value to a camera register
	 */
	void write(char subaddr, char data);

	/*
	 * Read a value from a camera register
	 */
	char read(char subaddr);

	/*
	 * Reset servo positions and camera registers
	 */
	void reset();
};

#endif /* CAMERAMOUNT_HPP */
