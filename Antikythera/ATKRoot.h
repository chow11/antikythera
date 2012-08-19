/*
 * ATKATKRoot.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_Root_H_
#define ATK_Root_H_


#include <ATKIOperator.h>


class ATKRoot : public ATKIOperator {
public:
	ATKRoot() { }
	~ATKRoot();

	virtual bool load(Stream *program);

	virtual bool process(unsigned long now);

	virtual uint8_t operandCount();
	virtual ATK_OPERAND operand(uint8_t index);
	virtual uint8_t resultCount();
	virtual void *resultGeneric(uint8_t index);
};


#endif

/* ATK_Root_H_ */
