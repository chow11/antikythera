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

	virtual bool readBool(uint8_t index);
	virtual double readDouble(uint8_t index);
	virtual float readFloat(uint8_t index);
	virtual int8_t readInt8(uint8_t index);
	virtual int16_t readInt16(uint8_t index);
	virtual int32_t readInt32(uint8_t index);
	virtual int64_t readInt64(uint8_t index);
	virtual uint8_t readUInt8(uint8_t index);
	virtual uint16_t readUInt16(uint8_t index);
	virtual uint32_t readUInt32(uint8_t index);
	virtual uint64_t readUInt64(uint8_t index);

	virtual void block();		// protect top set of value(s) in the queue from being overwritten while reading multiple values
	virtual void unblock();
	virtual bool pop();			// remove top set of values in the queue

protected:
	ATKISensor() { }
};


#endif

/* ATK_ISENSOR_H_ */
