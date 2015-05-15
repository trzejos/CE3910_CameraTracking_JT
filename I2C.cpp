/*
 * FILENAME:	I2C.cpp
 * AUTHOR:		Josh Trzebiatowski <trzebiatowskj@msoe.edu>
 * DATE:		April 10, 2015
 */

#include "I2C.hpp"
#include "system.h"
#include <stdio.h>
#include <io.h>

// Command bit definitions
#define CMD_START (1<<7)
#define CMD_STOP (1<<6)
#define CMD_READ (1<<5)
#define CMD_WRITE (1<<4)
#define CMD_NACK (1<<3)
#define CMD_ACK (0)

// Status bit definitions
#define STAT_ACK (1<<7)
#define STAT_BUSY (1<<6)
#define STAT_AL (1<<5)
#define STAT_TIP (1<<1)
#define STAT_IF (1<<0)

// Control bit definitions
#define CTRL_ENABLE (1<<7)

// I2C register offsets
#define OFF_PLO 0
#define OFF_PHI 1
#define OFF_CTR 2
#define OFF_TX 3
#define OFF_RX 3
#define OFF_CMD 4
#define OFF_STAT 4

/*
 * Constructor, create an I2C component with a slave address
 */
I2C::I2C(char slaveAddr) {
	IOWR_8DIRECT(OC_I2C_MASTER_TOP_0_BASE,OFF_PLO,99);
	IOWR_8DIRECT(OC_I2C_MASTER_TOP_0_BASE,OFF_PHI,0);
	IOWR_8DIRECT(OC_I2C_MASTER_TOP_0_BASE,OFF_CTR,CTRL_ENABLE);
	sla = slaveAddr & 0x0FF;
}

/*
 * Perform an I2C data transfer, specify read/write direction,
 * whether or not to send a STOP bit, and whether or not to
 * send the slave address with the operation
 */
bool I2C::transfer(bool read, unsigned char* data, bool stop, bool sla) {
	if(sla) {
		// Set up slave address
		if(read) {
			IOWR_8DIRECT(OC_I2C_MASTER_TOP_0_BASE, OFF_TX, (this->sla)|1);
		} else {
			IOWR_8DIRECT(OC_I2C_MASTER_TOP_0_BASE, OFF_TX, this->sla);
		}

		// send START and WRITE for slave address
		IOWR_8DIRECT(OC_I2C_MASTER_TOP_0_BASE, OFF_CMD, CMD_START|CMD_WRITE);

		// poll TIP
		while((IORD_8DIRECT(OC_I2C_MASTER_TOP_0_BASE, OFF_STAT)&STAT_TIP) != 0);

		// check ACK
		if((IORD_8DIRECT(OC_I2C_MASTER_TOP_0_BASE, OFF_STAT)&STAT_ACK) != 0) {
			// NACK
			IOWR_8DIRECT(OC_I2C_MASTER_TOP_0_BASE, OFF_CMD, CMD_STOP);
			return false;
		}
	}

	if(read) {
		// send READ, NACK
		if(stop) {
			IOWR_8DIRECT(OC_I2C_MASTER_TOP_0_BASE, OFF_CMD, CMD_READ|CMD_NACK|CMD_STOP);
		} else {
			IOWR_8DIRECT(OC_I2C_MASTER_TOP_0_BASE, OFF_CMD, CMD_READ|CMD_NACK);
		}

	} else {
		// prepare to send data
		IOWR_8DIRECT(OC_I2C_MASTER_TOP_0_BASE, OFF_TX, (*data));
		// send WRITE
		if(stop) {
			IOWR_8DIRECT(OC_I2C_MASTER_TOP_0_BASE, OFF_CMD, CMD_WRITE|CMD_STOP);
		} else {
			IOWR_8DIRECT(OC_I2C_MASTER_TOP_0_BASE, OFF_CMD, CMD_WRITE);
		}
	}

	// poll TIP
	while((IORD_8DIRECT(OC_I2C_MASTER_TOP_0_BASE, OFF_STAT)&STAT_TIP) != 0);

	if(read) {
		// Read resulting data
		*data = IORD_8DIRECT(OC_I2C_MASTER_TOP_0_BASE, OFF_RX);
	} else {

		// check ACK
		if((IORD_8DIRECT(OC_I2C_MASTER_TOP_0_BASE, OFF_STAT)&STAT_ACK) != 0) {
			// NACK
			IOWR_8DIRECT(OC_I2C_MASTER_TOP_0_BASE, OFF_CMD, CMD_STOP);
			return false;
		}
	}
	return true;

}



