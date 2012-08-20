/*
 * ATKATKRoot.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_ROOT_H_
#define ATK_ROOT_H_


#include <stddef.h>
#include <inttypes.h>
#include <ATKIOperator.h>


class ATKRoot : public ATKIOperator {
public:
	ATKRoot() { }
	~ATKRoot();

	virtual bool load(Stream *program);

	virtual bool evaluate(unsigned long now);

	virtual uint8_t operandCount();
	virtual ATK_OPERAND operand(uint8_t index);
	virtual uint8_t resultCount();
	virtual void *resultGeneric(uint8_t index);

#ifdef ANTIKYTHERA_DEBUG
	virtual String name() { return "ATKRoot"; }
#else
	virtual String name() { return ""; }
#endif

};


#endif

/* ATK_ROOT_H_ */
