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
#define OPERANDTYPE_FLOAT	3
#define OPERANDTYPE_STRING	7
#define OPERANDTYPE_UINT8	8
#define OPERANDTYPE_UINT16	9
#define OPERANDTYPE_UINT32	10
#define OPERANDTYPE_DOUBLE	11
#define OPERANDTYPE_CUSTOM	15

// value, value operandtype, operand type, operand index, element index
#define OPERAND_ELEMENT(V,W,X,Y,Z)	X V; if (operand(Y).flags & OPERANDFLAG_LINK) { Antikythera::operators[operand(Y).operatorIndex]->result(operand(Y).resultIndex, operandElementIndex(Y, operand(Y), Z), &V, W); } else { V = ATKIOperator::constant<X>(Y)[operandElementIndex(Y, operand(Y), Z)]; }


struct ATK_OPERAND {
	uint8_t flags;				// lower 4 bits used for data type
	uint16_t operatorIndex;
	uint8_t resultIndex;		// if OPERANDFLAG_CONST then constant value index
};


class ATKIOperator {
protected:
	ATKIOperator();
public:
	virtual ~ATKIOperator();

	virtual String name();

	// loading
public:
	virtual bool load(Stream *program);
protected:
	virtual bool loadProperties(Stream *program);
	bool loadFlags(Stream *program, uint8_t *flags);
	bool loadOperatorIndex(Stream *program, uint16_t *index);
	bool loadResultIndex(Stream *program, uint8_t *index);
	bool loadConstant(Stream *program, uint8_t operandIndex, uint8_t flags);
	virtual bool initializeConstant(uint8_t operandIndex, uint8_t constantSize);

	// evaluation
public:
	#ifdef ANTIKYTHERA_DEBUG
	virtual bool evaluate(unsigned long now, Stream *debug);
#else
	virtual bool evaluate(unsigned long now);
#endif
	bool isEvaluated() { return m_isEvaluated; }
	void resetEvaluatedFlag() { m_isEvaluated = false; }
	void setEvaluatedFlag() { m_isEvaluated = true; }

	// operands
public:
	uint8_t numOperands() { return m_numOperands; }
	ATK_OPERAND operand(uint8_t index) { return m_operands[index]; }

	// constants
protected:
	uint8_t constantSize(uint8_t index) { return (index < m_numOperands) ? m_constantSize[index] : 0; }
	template<typename T> inline T *constant(uint8_t index) { return (T *)constantGeneric(index); }
	virtual void *constantGeneric(uint8_t index) { return NULL; }

	// operations
protected:
	uint8_t numOperations() { return m_numOperations; };
	uint8_t operandElementIndex(uint8_t operandIndex, ATK_OPERAND o, uint8_t iteration);

	// results
public:
	virtual uint8_t numResults() { return 0; }
	virtual uint8_t resultSize(uint8_t index) { return 0; }
	virtual void result(uint8_t index, uint8_t element, void *value, uint8_t valueType) { }

	// debug
#ifdef ANTIKYTHERA_DEBUG
	String m_lastErrorString;
#endif

protected:
	bool m_isEvaluated;

	uint8_t m_numOperands;
	ATK_OPERAND *m_operands;
	uint8_t *m_constantSize;
	uint8_t m_numOperations;
};


#endif

/* ATK_IOPERATOR_H_ */
