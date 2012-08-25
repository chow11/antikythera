/*
 * ATKBinaryIntegerMath.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_BINARY_INTEGER_MATH_H_
#define ATK_BINARY_INTEGER_MATH_H_


#include <Stream.h>
#include <ATKIncludes.h>
#include <ATKIOperator.h>


/*
	Operands:
		- operation
		- a value
		- b value
	Results:
		- evaluation of operation
*/

#define MATH_NONE				0
#define MATH_ADDITION			1
#define MATH_SUBTRACTION		2
#define MATH_MULTIPLICATION		3
#define MATH_DIVISION			4

class ATKBinaryIntegerMath : public ATKIOperator {
public:
	ATKBinaryIntegerMath();
	~ATKBinaryIntegerMath();

	virtual String name() { return "ATKBinaryIntegerMath"; }

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
	int32_t *m_constB;
};


#endif

/* ATK_BINARY_INTEGER_MATH_H_ */
