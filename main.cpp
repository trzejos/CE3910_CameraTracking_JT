/*
 * FILENAME:	main.cpp
 * AUTHOR:		Josh Trzebiatowski <trzebiatowskj@msoe.edu>
 * DATE:		April 10, 2015
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Write and read bytes directly to memory
#include "Memory.hpp"

// A few simple math functions
#include "Math.hpp"

// A CameraMount object, which controls servos,
// I2C camera communication, and camera image data
#include "CameraMount.hpp"

// Input constants
#define BUFFER_SIZE 80
#define MAX_ARGS 20
#define MAX_ARG_LENGTH 20

// Used to determine the parsed type of data input
typedef enum {
	SUCCESS_INTEGER=3,
	SUCCESS_FLOAT=2,
	SUCCESS_STRING=1,
	SUCCESS_NONE=0
} SuccessType;

/*
 * Main function, requests user input and executes appropriate command
 */
int main() {

	// initialization
	char* rxbuffer = new char[BUFFER_SIZE];
	char* cmd = new char[MAX_ARG_LENGTH];
	char* sargs[MAX_ARGS];
	int* iargs[MAX_ARGS];
	float* fargs[MAX_ARGS];
	SuccessType successes[MAX_ARGS];

	for(int i=0; i < MAX_ARGS; i++){
		sargs[i] = new char[MAX_ARG_LENGTH];
		iargs[i] = new int;
		fargs[i] = new float;
		successes[i] = SUCCESS_NONE;
	}

	CameraMount* cm = new CameraMount();

	printf("Enter \"HELP\" for a list of commands.\n\n");

	while(true) {
		// Retrieve input into rxbuffer
		fgets(rxbuffer, BUFFER_SIZE, stdin);

		// decode input into strings
		int fields = sscanf(rxbuffer, "%s%s%s", cmd, sargs[0], sargs[1]);
		bool success = (fields != 0);



		// decode arguments into numerical values if possible
		for(int n=0;fields >= n+2; n++) {
			successes[n] = SUCCESS_NONE;
			if(sargs[n][0] != '\0')
				successes[n] = SUCCESS_STRING;
			if(sscanf(sargs[n], "%f", fargs[n]) == 1)
				successes[n] = SUCCESS_FLOAT;
			if(sscanf(sargs[n], "%i", iargs[n]) == 1)
				successes[n] = SUCCESS_INTEGER;
		}

		// perform input command
		if(success) {
			switch(fields) {
			case 3:
				// Read a range of memory and output results to console
				if(strcmp(cmd, "RD") == 0 && successes[0] >= SUCCESS_INTEGER && successes[1] >= SUCCESS_INTEGER) {
					char* read = Memory::readRange(*iargs[0], *iargs[1]);

					printf("Memory Read:");
					for(int i=0; i<= (*iargs[1]-*iargs[0]); i++) {
						if((i % 16) == 0)
							printf("\n0x%08X: ", *iargs[0] + i);

							printf("%02X ", (char)read[i]);
					}
					printf("\n");

					delete read;

				// Write a byte value to a memory address
				} else if(strcmp(cmd, "WR") == 0 && successes[0] >= SUCCESS_INTEGER && successes[1] >= SUCCESS_INTEGER) {
					Memory::write(*iargs[0], *iargs[1]);
					printf("Memory written: %02X @ 0x%08X\n\n", *iargs[1], *iargs[0]);

				// Write a byte to a camera subaddress
				} else if(strcmp(cmd, "CW") == 0 && successes[0] >= SUCCESS_INTEGER && successes[1] >= SUCCESS_INTEGER) {
					cm->write(*iargs[0], *iargs[1]);
					printf("Cam register %X: %X", *iargs[0], *iargs[1]);

				// Invalid input
				} else {
					printf("ERROR: Invalid command\n");
				}
				break;
			case 2:
				// Change pan servo position
				if(strcmp(cmd, "PAN") == 0 && successes[0] >= SUCCESS_FLOAT) {
					printf("Pan camera: %f\n", *fargs[0]);
					cm->pan(*fargs[0]);

				// Change tilt servo position
				} else if(strcmp(cmd, "TILT") == 0 && successes[0] >= SUCCESS_FLOAT) {
					printf("Tilt camera: %f\n", *fargs[0]);
					cm->tilt(*fargs[0]);

				// Read from a camera subaddress
				} else if(strcmp(cmd, "CR") == 0 && successes[0] >= SUCCESS_INTEGER) {
					printf("Cam register %X: %X", *iargs[0], cm->read(*iargs[0]));

				// Invalid input
				} else {
					printf("ERROR: Invalid command\n");
				}
				break;
			case 1:
				// Reset CameraMount
				if(strcmp(cmd, "RESET") == 0) {
					printf("Resetting...\n");
					cm->reset();

				} else if(strcmp(cmd, "TEST") == 0) {
					cm->getCameraFrame(false);
					cm->updateThreshold();
					cm->testFrame();
					cm->adjustServos();

				// Take one image and display it to the VGA
				} else if (strcmp(cmd, "SNAPSHOT") == 0) {
					printf("Taking a snapshot\n");
					cm->getCameraFrame(false);

				// Same as SNAPSHOT, but also print debug info
				} else if (strcmp(cmd, "TRACK") == 0) {
					while(true) {
						cm->getCameraFrame(false);
						cm->updateThreshold();
						cm->adjustServos();
					}
				// Start a continuous feed of camera data
				} else if (strcmp(cmd, "CAMFEED") == 0) {
					while(true) cm->getCameraFrame(false);

				// Print command information
				} else if (strcmp(cmd, "HELP") == 0) {
					printf("Commands:\n");

					printf("RD addr1 addr2\n");
					printf("  Read a range of bytes in memory to console\n");
					printf("\n");

					printf("WR addr value\n");
					printf("  Write a byte to memory\n");
					printf("\n");

					printf("CW subaddr value\n");
					printf("  Write to a camera subaddress\n");
					printf("\n");

					printf("PAN deg\n");
					printf("  Pan the camera to a certain position (degrees)\n");
					printf("\n");

					printf("TILT deg\n");
					printf(" Tilt the camera to a certain position (degrees)\n");
					printf("\n");

					printf("CR subaddr\n");
					printf("  Read from a camera subaddress\n");
					printf("\n");

					printf("SNAPSHOT\n");
					printf("  Get a new frame from the camera\n");
					printf("\n");

					printf("TRACK\n");
					printf("  Start camera tracking\n");
					printf("  (Prevents further input)\n");
					printf("\n");

					printf("CAMFEED\n");
					printf("  Start a continuous camera feed\n");
					printf("  (Prevents further input)\n");
					printf("\n");

					printf("RESET\n");
					printf("  Reset the camera\n");
					printf("\n");

					printf("HELP\n");
					printf("  Show these commands\n");
					printf("\n");

				// Invalid input
				} else {
					printf("ERROR: Invalid command\n");
				}
				break;

			// Invalid input
			default:
				printf("ERROR: Invalid command\n");
				break;
			}

		// Invalid input
		} else {
			printf("ERROR: Invalid command\n");
		}
	}

	return 0;
}
