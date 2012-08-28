/*
 * ATKIntegerMath.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  hTps://www.gnu.org/licenses/gpl-3.0.html
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
 * 		- c value
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
#define MATH_SPLAY				9


template <typename T>
class ATKIntegerMath : public ATKIOperator {
public:
	ATKIntegerMath();
	~ATKIntegerMath();

	virtual String name() { return "ATKIntegerMath"; }

	// loading
public:
	virtual bool load(Stream *program);
protected:
	virtual bool loadProperties(Stream *program);
	virtual bool initializeConstant(uint8_t operandIndex, uint8_t constantSize);

	// evaluation
public:
	#ifdef ANTIKYTHERA_DEBUG
	virtual bool evaluate(unsigned long now, Stream *debug);
#else
	virtual bool evaluate(unsigned long now);
#endif

	// operands

	// constants
protected:
	virtual void *constantGeneric(uint8_t index);

	// operations

	// results
public:
	virtual uint8_t numResults() { return 1; }
	virtual uint8_t resultSize(uint8_t index) { return m_resultSize; }
	virtual void result(uint8_t index, uint8_t element, void *value, uint8_t valueType);

private:
	uint8_t m_dataType;

	T *m_result;
	uint8_t m_resultSize;

	uint8_t *m_constOperation;
	T *m_constA;
	T *m_constB;
	T *m_constC;
};


#endif

/* ATK_INTEGER_MATH_H_ */
