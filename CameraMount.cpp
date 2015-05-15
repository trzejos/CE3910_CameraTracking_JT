/*
 * FILENAME:	CameraMount.cpp
 * AUTHOR:		Josh Trzebiatowski <trzebiatowskj@msoe.edu>
 * DATE:		April 10, 2015
 */

#include "CameraMount.hpp"
#include "Servo.hpp"
#include "PWMIndex.h"
#include "Math.hpp"

#include <stdio.h>
#include <unistd.h>

// Tilt servo boundaries
#define TILT_MIN 0.02f
#define TILT_MAX 0.10f

// Pan servo boundaries
#define PAN_MIN 0.025f
#define PAN_MAX 0.12f

// Servo input boundaries
#define INPUT_MIN 0.0f
#define INPUT_MAX 1.0f

// Initial servo positions
#define PAN_INIT 0.5f
#define TILT_INIT 0.6f

#define ADJ_FACTOR_PAN (-0.5f)
#define ADJ_FACTOR_TILT (0.6f)
#define PXROW_TH_COLS 5
#define PXROW_TH_ROWS 3

#define ROW_START 18
#define ROW_END 55
#define COL_START 5
#define COL_END 75

/*
 * Constructor, Initialize servos and camera, set defaults
 */
CameraMount::CameraMount() {
	// init components
	servoPan = new Servo(PWMINDEX_A, PAN_INIT * (PAN_MIN + PAN_MAX));
	servoTilt = new Servo(PWMINDEX_B, TILT_INIT * (TILT_MIN + TILT_MAX));
	camera = new Camera();
	threshold = 128;

	// set defaults
	reset();
}

/*
 * Set the pan servo to a position specified by degrees
 */
void CameraMount::pan(float degrees) {
	float value = Math::scale<float>(degrees, -90.0f, 90.0f, PAN_MIN, PAN_MAX);
	servoPan->setDC(Math::clamp<float>(value, PAN_MIN, PAN_MAX));
	lastPan = Math::scale<float>(servoPan->getDC(), PAN_MIN, PAN_MAX, -90.0f, 90.0f);
}

/*
 * Set the tilt servo to a position specified by degrees
 */
void CameraMount::tilt(float degrees) {
	float value = Math::scale<float>(degrees, 0.0f, 90.0f, 0.0f, 0.65f);
	servoTilt->setDC(Math::clamp<float>(Math::scale<float>(value, INPUT_MIN, INPUT_MAX, TILT_MIN, TILT_MAX), TILT_MIN, TILT_MAX));
	lastTilt = Math::scale<float>(servoTilt->getDC(), TILT_MIN, TILT_MAX, INPUT_MIN, INPUT_MAX);
	lastTilt = Math::scale<float>(lastTilt, 0.0f, 0.65f, 0.0f, 90.0f);
}

/*
 * Write a value to a camera register
 */
void CameraMount::write(char subaddr, char data) {
	camera->camWrite(subaddr, data);
}

/*
 * Read a value from a camera register
 */
char CameraMount::read(char subaddr) {
	return camera->camRead(subaddr);
}

/*
 * Print a camera frame to the VGA memory
 */
void CameraMount::getCameraFrame(bool debug) {
	threshold = camera->getFrame(debug);
}

void CameraMount::updateThreshold() {
	unsigned char max = 0;
	unsigned char min = 255;

	for(int r = ROW_START; r < ROW_END; r++) {
		for(int c = COL_START; c < COL_END; c++) {
			unsigned char px = *(camera->pixel(r,c));
			max = px > max ? px : max;
			min = px < min ? px : min;
		}
	}

	threshold = (max>>1) + (min>>1);
}

void CameraMount::testFrame() {
	*(camera->pixel(0,0)) = threshold;
	for(int r = ROW_START; r < ROW_END; r++) {
		for(int c = COL_START; c < COL_END; c++) {
			volatile unsigned char* px = camera->pixel(r,c);
			if(*px < threshold)
				*px = 0;
			else
				*px = 255;
		}
	}
}

void CameraMount::adjustServos() {
	const int ROW_MID = (ROW_END+ROW_START)/2;
	const int COL_MID = (COL_END+COL_START)/2;
	float adjPan = 0.0f;
	float adjTilt = 0.0f;
	int pxInARow = 0;

	int maxInARow = 0;

	int ulr = ROW_START;
	int ulc = COL_START;

	int lrr = ROW_END;
	int lrc = COL_END;

	for(int r = ROW_START; r < ROW_END; r++) {
		for(int c = COL_START; c < COL_END; c++) {
			unsigned char px = *(camera->pixel(r,c));
			if(px > threshold) {
				pxInARow++;
			} else {
				if(pxInARow > PXROW_TH_COLS && pxInARow > maxInARow) {
					maxInARow = pxInARow;
					ulc = c-pxInARow;
					lrc = c-1;
				}
				pxInARow = 0;
			}
		}
	}

	pxInARow=0;
	maxInARow = 0;
	for(int c = COL_START; c < COL_END; c++) {
		for(int r = ROW_START; r < ROW_END; r++) {
			unsigned char px = *(camera->pixel(r,c));
			if(px > threshold) {
				pxInARow++;
			} else {
				if(pxInARow > PXROW_TH_ROWS && pxInARow > maxInARow) {
					maxInARow = pxInARow;
					ulr = r-pxInARow;
					lrr = r-1;
				}
				pxInARow = 0;
			}
		}
	}

	*(camera->pixel(ulr, ulc)) = 64;
	*(camera->pixel(lrr, lrc)) = 196;
	*(camera->pixel(ROW_MID, COL_MID)) = 128;

	adjPan = (float)COL_MID - (((float)ulc) + ((float)lrc)) / 2.0f;
	adjTilt = (float)ROW_MID - (((float)ulr) + ((float)lrr)) / 2.0f;

	tilt(lastTilt + adjTilt * ADJ_FACTOR_TILT);
	pan(lastPan + adjPan * ADJ_FACTOR_PAN);
}

/*
 * Read pixel data from the VGA memory
 */
char CameraMount::getCameraPixel(int row, int column) {
	return *(camera->pixel(row, column));
}

/*
 * Reset servo positions and camera registers
 */
void CameraMount::reset() {
	pan(0.0f);
	tilt(90.0f);

	write(0x11, 0x08);
	usleep(100);
	write(0x14, 1<<5);
	usleep(100);
	write(0x39, 1<<6);
	usleep(100);
}

