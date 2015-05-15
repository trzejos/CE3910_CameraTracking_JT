/*
 * FILENAME:	Camera.cpp
 * AUTHOR:		Josh Trzebiatowski <trzebiatowskj@msoe.edu>
 * DATE:		April 10, 2015
 */

#include "Camera.hpp"
#include "I2C.hpp"
#include "system.h"
#include "Math.hpp"

#include <stdio.h>

// CAM_CONTROL bit definitions
#define CAMCONTROL_PCLK (1<<0)
#define CAMCONTROL_HREF (1<<1)
#define CAMCONTROL_VSYNC (1<<2)

// OmniVision camera I2C slave address
#define CAM_SLA 0xC0

// Camera image dimensions
#define CAM_ROWS 144
#define CAM_COLUMNS 176

// VGA memory address
#define VGA_BASE 0x80800000

// VGA memory dimensions
#define VGA_ROWS 60
#define VGA_COLUMNS 80

/*
* Constructor, initializes pointers and I2C component
*/
Camera::Camera() {
	i2c = new I2C((char)CAM_SLA);
}

/*
 * Get one frame and print it to the VGA memory
 * Parameter debug toggles printing of I2C debug information
 */
unsigned char Camera::getFrame(bool debug) {
	volatile register char* pxlPort = (volatile char*)(0x80000000 | PIXEL_PORT_BASE);
	volatile register char* control = (volatile char*)(0x80000000 | CAM_CONTROL_BASE);

	register unsigned char min = 255;
	register unsigned char max = 0;
	register unsigned char px;

	// initialize VGA counters
	register bool rowvalid = false;
	register unsigned char* vga = (unsigned char*)((0x80000000|VGA_BASE) | (59*(1<<7)) | (79) + 48);


	// wait for VSYNC falling edge
	while((*control & CAMCONTROL_VSYNC) == 0);
	//while((*control & CAMCONTROL_VSYNC) != 0);

	for(register int r=0; r<CAM_ROWS; r++) {
		// poll for HREF falling edge
		while((*control & CAMCONTROL_HREF) != 0);
		// wait for HREF rising edge
		while((*control & CAMCONTROL_HREF) == 0);

		for(register int c=0; c<CAM_COLUMNS;c++) {
			// wait for pclk rising edge
			while((*control & CAMCONTROL_PCLK) != 0);
			while((*control & CAMCONTROL_PCLK) == 0);



			// if valid column and row, sample data and write to VGA memory
			if((c >= 8) && ((c&1) == 0) && (c <= 167 ) && rowvalid) {
				px = *pxlPort;
				min = px<min ? px:min;
				max = px>max ? px:max;

				*vga = px;
				vga--;
			}
		}
		// reset VGA column counter
		rowvalid = false;
		// if valid row, flag a boolean as such
		if((r >= 12) && ((r&1) == 0) && (r <= 131)) {
			rowvalid = true;
			vga -= 48;
		}
	}

	return (min>>1) + (max>>1);
}

/*
 * Get a pointer to the pixel data at a specified location
 */
volatile unsigned char* Camera::pixel(int row, int column) {
	row = Math::clamp(row, 0, VGA_ROWS-1);
	column = Math::clamp(column, 0, VGA_COLUMNS-1);
	volatile unsigned char* rv = (volatile unsigned char*)(VGA_BASE|row<<7|column);
	return rv;
}

/*
 * read data from the camera register specified by subaddr
 */
unsigned char Camera::camRead(unsigned char subaddr) {
	unsigned char rv = '\0';
	if(i2c->transfer(false, &subaddr, true, true))
		i2c->transfer(true, &rv, true, true);
	return rv;
}

/*
 * write data to the camera register specified by subaddr
 */
void Camera::camWrite(unsigned char subaddr, unsigned char data) {
	if(i2c->transfer(false, &subaddr, false, true))
		i2c->transfer(false, &data, true, false);
}
