/*
 * ATKUnaryIntegerMath.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_UNARY_INTEGER_MATH_H_
#define ATK_UNARY_INTEGER_MATH_H_


#include <Stream.h>
#include <ATKIncludes.h>
#include <ATKIOperator.h>


/*
	Operands:
		- operation
		- a value
	Results:
		- evaluation of operation
*/

#define MATH_NONE				0
#define MATH_INCREMENT			1
#define MATH_DECREMENT			2
#define MATH_ABS				3

class ATKUnaryIntegerMath : public ATKIOperator {
public:
	ATKUnaryIntegerMath();
	~ATKUnaryIntegerMath();

	virtual String name() { return "ATKUnaryIntegerMath"; }

	virtual bool load(Stream *program);
#ifdef ANTIKYTHERA_DEBUG
	virtual bool evaluate(unsigned long now, Stream *debug);
#else
	virtual bool evaluate(unsigned long now);
#endif
	virtual uint8_t numResults() { return 1; }
	virtual uint8_t resultSize(uint8_t index) { return m_resultSize; }

protected:
	virtual void *resultGeneric(uint8_t index) { return m_result; }
	virtual void *constantGeneric(uint8_t index);
	virtual bool initializeConstant(uint8_t operandIndex, uint8_t constantSize);
	virtual bool loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program);

private:
	int32_t *m_result;
	uint8_t m_resultSize;

	uint8_t *m_constOperation;
	int32_t *m_constA;
};


#endif

/* ATK_UNARY_INTEGER_MATH_H_ */
