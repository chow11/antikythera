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


#include <Stream.h>
#include <ATKIncludes.h>
#include <ATKIOperator.h>


/*
	Operands:
		- (multi operand) layer (0 = background)
	Results:
		- none
*/

class ATKDisplay : public ATKIOperator {
public:
	ATKDisplay();
	~ATKDisplay();

#ifdef ANTIKYTHERA_DEBUG
	virtual String name() { return "ATKDisplay"; }
#else
	virtual String name() { return ""; }
#endif

	virtual bool load(Stream *program);
	virtual bool evaluate(unsigned long now);
	virtual uint8_t numResults() { return 0; }
	virtual uint8_t resultSize(uint8_t index) { return 0; }

protected:
	virtual void *resultGeneric(uint8_t index) { return NULL; }
	virtual void *constantGeneric(uint8_t index);
	virtual bool initializeConstant(uint8_t operandIndex, uint8_t constantSize);
	virtual bool loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program);
};


#endif

/* ATK_DISPLAY_H_ */
