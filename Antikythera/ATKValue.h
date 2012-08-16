/*
 * ATKValue.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_VALUE_H_
#define ATK_VALUE_H_


#define VALUEFLAG_DIRTY		0x80


union VALUE_TYPE {
	bool b;
	int8_t i8;
	int8_t i16;
	int8_t i32;
};


class ATKValue {
public:
	ATKValue() { m_flags = 0; m_count = 0; }
	~ATKValue() { if (m_values != NULL) delete[] m_values; }

	bool isDirty() { return m_flags & VALUEFLAG_DIRTY; }
	uint8_t count() { return m_count; }
	void *operator[](uint8_t index) { return m_values[index]; }

private:
	uint8_t m_flags;				// lower 4 bits used for data type (future)
	uint8_t m_count;
	void *m_values[];
};


#endif

/* ATK_VALUE_H_ */
