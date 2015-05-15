/*
 * FILENAME:	I2C.hpp
 * AUTHOR:		Josh Trzebiatowski <trzebiatowskj@msoe.edu>
 * DATE:		April 10, 2015
 */

#ifndef I2C_H
#define I2C_H

/*
 * I2C class, handles all I2C read/write operations
 */
class I2C {
private:
	unsigned char sla;

protected:

public:
	/*
	 * Constructor, create an I2C component with a slave address
	 */
	I2C(char slaveAddr);

	/*
	 * Perform an I2C data transfer, specify read/write direction,
	 * whether or not to send a STOP bit, and whether or not to
	 * send the slave address with the operation
	 */
	bool transfer(bool read, unsigned char* data, bool stop, bool sla);
};

#endif /* I2C_H */
