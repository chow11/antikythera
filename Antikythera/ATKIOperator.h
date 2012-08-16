/*
 * ATKIOperand.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_IOperator_H_
#define ATK_IOperator_H_


#include <Stream.h>


#define OPERANDFLAG_LINK	0x80		// 0=constant 1=link to operator output
#define OPERANDFLAG_LIMIT	0x40		// 0=fill 1=limit
#define OPERANDFLAG_EXTEND	0x20		// 0=cycle 1=extend
#define OPERANDFLAG_SINGLE	0x10		// 0=use all values 1=always use first value


struct ATK_OPERAND {
	uint8_t flags;				// lower 4 bits used for data type (future)
	uint8_t operatorIndex;
	uint8_t resultIndex;		// if OPERANDFLAG_CONST then constant value index
};


class ATKIOperator {
public:
	virtual ~ATKIOperator();

	virtual bool load(Stream *program);

	virtual bool process(long now);

	virtual uint8_t operandCount();
	virtual ATK_OPERAND operand(uint8_t index);
	virtual uint8_t resultCount();
	template<typename T> inline T *result(uint8_t index) { return (T *)resultGeneric(index); }
	virtual uint8_t resultSize(uint8_t index);

	bool isProcessed();
	void resetProcessedFlag();
	void setProcessedFlag();
	
protected:
	ATKIOperator();

	virtual void *resultGeneric(uint8_t index);
	uint8_t operationCount() { return m_operationCount; };
	uint8_t operandElementIndex(ATK_OPERAND o, uint8_t iteration);

private:
	bool m_isProcessed;
	uint8_t m_operationCount;
};


#endif

/* ATK_IOperator_H_ */
