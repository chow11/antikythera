/*
 * ATKSelect.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_SELECT_H_
#define ATK_SELECT_H_


#include <Stream.h>
#include <ATKIncludes.h>
#include <ATKIOperator.h>


/*
	Operands:
		- index of which operator link is evaluated and results passed through. 0 = does nothing.  Implied OPERANDFLAG_SINGLE
		- operator link 1
		- operator link 2
		- ...
		- operator link n
	Results:
		- pass through of results of select operator link
*/

class ATKSelect : public ATKIOperator {
public:
	ATKSelect();
	~ATKSelect();

public:
	virtual bool load(Stream *program);
protected:
	virtual bool loadProperties(Stream *program);
	virtual void initializeOperands(uint8_t numOperands);
	virtual bool initializeConstant(uint8_t operandIndex, uint16_t constantSize);
	virtual void setConstant(uint8_t operandIndex, uint16_t element, void *value);

public:
	#ifdef ANTIKYTHERA_DEBUG
	virtual bool evaluate(unsigned long now, Stream *debug);
#else
	virtual bool evaluate(unsigned long now);
#endif

	virtual void getResult(uint8_t resultIndex, uint16_t element, void *value);

private:
	int16_t *m_evaluatedIndex;
	int16_t **m_consts;
};


#endif

/* ATK_SELECT_H_ */
