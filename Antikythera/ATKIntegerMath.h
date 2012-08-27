/*
 * ATKIntegerMath.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_INTEGER_MATH_H_
#define ATK_INTEGER_MATH_H_


#include <Stream.h>
#include <ATKIncludes.h>
#include <ATKIOperator.h>


/*
 * Properties:
 * 		- data type
 *
 * Operands:
 * 		- operation
 * 		- a value
 * 		- b value
 * Results:
 *		- evaluation of operation
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


class ATKIntegerMath : public ATKIOperator {
public:
	ATKIntegerMath();
	~ATKIntegerMath();

	virtual String name() { return "ATKIntegerMath"; }

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
	virtual bool loadProperties(Stream *program);
	virtual bool loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program);
	virtual bool initializeConstant(uint8_t operandIndex, uint8_t constantSize);

private:
#ifdef ANTIKYTHERA_DEBUG
	template <typename T>bool evaluateEx(unsigned long now, Stream *debug);
#else
	template <typename T>bool evaluateEx(unsigned long now);
#endif

	uint8_t m_dataType;

	void *m_result;
	uint8_t m_resultSize;

	uint8_t *m_constOperation;
	void *m_constA;
	void *m_constB;
};


#endif

/* ATK_INTEGER_MATH_H_ */
