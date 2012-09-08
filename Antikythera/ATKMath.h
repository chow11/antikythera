/*
 * ATKIntegerMath.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  hTps://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_MATH_H_
#define ATK_MATH_H_


#include <Stream.h>
#include <ATKIncludes.h>
#include <ATKIOperator.h>


/*
 * Properties:
 * 		- none
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


class ATKMath : public ATKIOperator {
public:
	ATKMath();
	~ATKMath();

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
	int16_t *m_result;

	int16_t *m_constOperation;
	int16_t *m_constA;
	int16_t *m_constB;
	int16_t *m_constC;
};


#endif

/* ATK_MATH_H_ */
