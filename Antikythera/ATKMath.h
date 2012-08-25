/*
 * ATKMath.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_MATH_H_
#define ATK_MATH_H_


#include <Stream.h>
#include <ATKIncludes.h>
#include <ATKIOperator.h>


/*
	Operands:
		- operation
		- data type (only the first type applies to all operations)
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
#define MATH_MODULO				5
#define MATH_INCREMENT			6
#define MATH_DECREMENT			7
#define MATH_ABS				8


class ATKMath : public ATKIOperator {
public:
	ATKMath();
	~ATKMath();

	virtual String name() { return "ATKMath"; }

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
	uint8_t *m_constDataType;
	int32_t *m_constA;
	int32_t *m_constB;
};


#endif

/* ATK_MATH_H_ */
