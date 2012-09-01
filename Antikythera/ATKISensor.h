/*
 * ATKISensor.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_ISENSOR_H_
#define ATK_ISENSOR_H_


#include <ATKIncludes.h>


class ATKISensor {
public:
	virtual ~ATKISensor();

	virtual void initialize();

	virtual int16_t readInt16(uint8_t index);
	virtual uint32_t readUInt32(uint8_t index);
	virtual float readFloat(uint8_t index);
	virtual String readString(uint8_t index);

	virtual bool pop();			// remove top set of values in the queue

protected:
	ATKISensor() { }
};


#endif

/* ATK_ISENSOR_H_ */
