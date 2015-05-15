/*
 * FILENAME:	Math.hpp
 * AUTHOR:		Josh Trzebiatowski <trzebiatowskj@msoe.edu>
 * DATE:		April 10, 2015
 */

#ifndef MATH_HPP
#define MATH_HPP

/*
 * Math namespace, contains templated functions for simple math operations
 */
namespace Math {
	/*
	 * Return the maximum of two values
	 */
	template <typename T> T max(T a, T b) {
		return a > b ? a : b;
	}

	/*
	 * Return the minimum of two values
	 */
	template <typename T> T min(T a, T b) {
		return a < b ? a : b;
	}

	/*
	 * Clamp a value to a range
	 */
	template <typename T> T clamp(T val, T min, T max) {
		return Math::max(min, Math::min(val, max));
	}

	/*
	 * Check if a value is in a range
	 */
	template <typename T> bool inRange(T val, T min, T max) {
		return (val >= min) && (val <= max);
	}

	/*
	 * Linearly scale a value from an input range to an output range with translation
	 */
	template <typename T> T scale(T value, T in_min, T in_max, T out_min, T out_max) {
		T rin = in_max - in_min;
		T rout = out_max - out_min;
		T slope = rout/rin;
		return (value-in_min) * slope + out_min;
	}
}

#endif
