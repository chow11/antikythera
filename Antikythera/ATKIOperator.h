/*
 * ATKIOperand.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_IOPERATOR_H_
#define ATK_IOPERATOR_H_


#include <Stream.h>
#include <ATKIncludes.h>


#define OPERANDFLAG_LINK	0x80		// 0=constant 1=link to operator output
#define OPERANDFLAG_LIMIT	0x40		// 0=fill 1=limit
#define OPERANDFLAG_EXTEND	0x20		// 0=cycle 1=extend
#define OPERANDFLAG_SINGLE	0x10		// 0=use all values 1=always use first value

#define OPERANDTYPE_INT8	0
#define OPERANDTYPE_INT16	1
#define OPERANDTYPE_INT32	2
#define OPERANDTYPE_INT64	3
#define OPERANDTYPE_FLOAT	4
#define OPERANDTYPE_STRING	7
#define OPERANDTYPE_UINT8	8
#define OPERANDTYPE_UINT16	9
#define OPERANDTYPE_UINT32	10
#define OPERANDTYPE_UINT64	11
#define OPERANDTYPE_DOUBLE	12
#define OPERANDTYPE_CUSTOM	15


struct ATK_OPERAND {
	uint8_t flags;				// lower 4 bits used for data type
	uint16_t operatorIndex;
	uint8_t resultIndex;		// if OPERANDFLAG_CONST then constant value index
};


class ATKIOperator {
public:
	virtual ~ATKIOperator();

	virtual String name();

	virtual bool load(Stream *program);
	virtual bool evaluate(unsigned long now);
	virtual uint8_t numResults();
	virtual uint8_t resultSize(uint8_t index);

	uint8_t numOperands() { return m_numOperands; }
	ATK_OPERAND operand(uint8_t index) { return m_operands[index]; }
	template<typename T> inline T *constant(uint8_t index) { return (T *)constantGeneric(index); }
	template<typename T> inline T *result(uint8_t index) { return (T *)resultGeneric(index); }

	bool isEvaluated();
	void resetEvaluatedFlag();
	void setEvaluatedFlag();

	String lastErrorString() { return m_lastErrorString; }

protected:
	ATKIOperator();

	virtual void *resultGeneric(uint8_t index);
	uint8_t numOperations() { return m_numOperations; };
	uint8_t operandElementIndex(ATK_OPERAND o, uint8_t iteration);
	virtual void *constantGeneric(uint8_t index);

	uint8_t loadFlags(Stream *program);
	uint16_t loadOperatorIndex(Stream *program);
	uint8_t loadResultIndex(Stream *program);
	uint8_t constantSize(uint8_t index) { return (index < m_numOperands) ? m_constantSize[index] : 0; }
	virtual bool initializeConstant(uint8_t operandIndex, uint8_t constantSize);
	virtual bool loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program);

	uint8_t *m_constantSize;
	String m_lastErrorString;

private:
	bool m_isEvaluated;

	uint8_t m_numOperands;
	ATK_OPERAND *m_operands;

	uint8_t m_numOperations;
};


#endif

/* ATK_IOPERATOR_H_ */
