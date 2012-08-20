/*
 * ATKLine.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_LINE_H_
#define ATK_LINE_H_


#include <stddef.h>
#include <inttypes.h>
#include <ATKColor.h>
#include <ATKIOperator.h>


/*
	Operands:
		- x location
		- y location
		- color (HSVA)
		- display number
		- layer (0 = background)
	Results:
		- layer (0 = background)
*/

class ATKLine : public ATKIOperator {
public:
	ATKLine();
	~ATKLine();

	virtual bool load(Stream *program);

	virtual bool evaluate(unsigned long now);

	virtual uint8_t operandCount() { return 10; }
	virtual ATK_OPERAND operand(uint8_t index) { return m_operands[index]; }
	virtual uint8_t resultCount() { return 1; }
	virtual uint8_t resultSize(uint8_t index) { return 1; }

protected:
	virtual void *resultGeneric(uint8_t index) { return m_result; }

#ifdef ANTIKYTHERA_DEBUG
	virtual String name() { return "ATKLine"; }
#else
	virtual String name() { return ""; }
#endif

private:
	ATK_OPERAND m_operands[];
	uint8_t m_result[1];

	uint8_t m_x1;
	uint8_t m_y1;
	uint8_t m_x2;
	uint8_t m_y2;
	ATKColor::HSVA m_color;
	uint8_t m_width;
	uint8_t m_style;
	uint8_t m_mode;
	uint8_t m_displayNumber;
	uint8_t m_layer;
};


#endif

/* ATK_LINE_H_ */
