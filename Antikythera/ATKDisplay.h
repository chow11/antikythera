/*
 * ATKDisplay.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_Display_H_
#define ATK_Display_H_

#include "ATKIOperator.h"

/*
	Operands:
		- (multi operand) layer (0 = background)
	Results:
		- none
*/

class ATKDisplay : public ATKIOperator {
public:
	ATKDisplay() { }
	~ATKDisplay() { }

	virtual bool load(Stream *program);

	virtual bool process(long now);

	virtual uint8_t operandCount();
	virtual ATK_OPERAND operand(uint8_t index) { return m_operands[index]; }
	virtual uint8_t resultCount() { return 0; }
	virtual uint8_t resultSize(uint8_t index) { return 0; }

protected:
	virtual void *resultGeneric(uint8_t index) { return NULL; }

private:
	ATK_OPERAND m_operands[];
};

#endif

/* ATK_Display_H_ */
