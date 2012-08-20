/*
 * ATKDisplay.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_DISPLAY_H_
#define ATK_DISPLAY_H_


#include <stddef.h>
#include <inttypes.h>
#include <ATKIOperator.h>


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

	virtual bool evaluate(unsigned long now);

	virtual uint8_t operandCount();
	virtual ATK_OPERAND operand(uint8_t index) { return m_operands[index]; }
	virtual uint8_t resultCount() { return 0; }
	virtual uint8_t resultSize(uint8_t index) { return 0; }

protected:
	virtual void *resultGeneric(uint8_t index) { return NULL; }

#ifdef ANTIKYTHERA_DEBUG
	virtual String name() { return "ATKDisplay"; }
#else
	virtual String name() { return ""; }
#endif

private:
	ATK_OPERAND m_operands[];
};


#endif

/* ATK_DISPLAY_H_ */
