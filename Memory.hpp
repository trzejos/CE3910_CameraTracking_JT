/*
 * FILENAME:	Memory.hpp
 * AUTHOR:		Josh Trzebiatowski <trzebiatowskj@msoe.edu>
 * DATE:		April 10, 2015
 */

#ifndef MEMORY_HPP
#define MEMORY_HPP

/*
 * Memory namespace, contains simple read and write functions
 * to interface with memory addresses
 */
namespace Memory {
	/*
	 * Read a single byte from memory
	 */
	char read(int address) {
		volatile char* ptr = (char*)address;
		return *ptr;
	}

	/*
	 * Read a range of bytes from memory
	 */
	char* readRange(int addr1, int addr2) {
		char* rv = new char[addr2 - addr1 + 1];
		char* wr = rv;

		volatile char* ptr = (char*)addr1;
		while(ptr <= (char*)addr2)
			*(wr++) = *(ptr++);

		return rv;
	}

	/*
	 * Write a byte to memory
	 */
	void write(int address, char value) {
		volatile char* ptr = (char*)address;
		*ptr = value;
	}
}

#endif
