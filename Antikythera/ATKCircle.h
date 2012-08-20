/*
 * ATKCircle.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_Circle_H_
#define ATK_Circle_H_


#include <ATKColor.h>
#include <ATKIOperator.h>


/*
	Operands:
		- x location
		- y location
		- radius
		- color (HSVA)
		- display number
		- layer (0 = background)
	Results:
		- layer (0 = background)
*/

class ATKCircle : public ATKIOperator {
public:
	ATKCircle();
	~ATKCircle();

	virtual bool load(Stream *program);

	virtual bool evaluate(long now);

	virtual uint8_t operandCount() { return 6; }
	virtual ATK_OPERAND operand(uint8_t index) { return m_operands[index]; }
	virtual uint8_t resultCount() { return 1; }
	virtual uint8_t resultSize(uint8_t index) { return 1; }

protected:
	virtual void *resultGeneric(uint8_t index) { return m_result; }

#ifdef ANTIKYTHERA_DEBUG
	virtual String name() { return "ATKCircle"; }
#else
	virtual String name() { return ""; }
#endif

private:
	ATK_OPERAND m_operands[];
	uint8_t m_result[1];

	uint8_t m_x;
	uint8_t m_y;
	uint8_t m_radius;
	HSVA m_color;
	uint8_t m_displayNumber;
	uint8_t m_layer;
};


#endif

/* ATK_Circle_H_ */
