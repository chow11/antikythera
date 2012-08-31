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

#define OPERANDTYPE_INT16	0
#define OPERANDTYPE_UINT32	1
#define OPERANDTYPE_FLOAT	2
#define OPERANDTYPE_STRING	3

#define OPERAND_ELEMENT(VNAME,CNAME,OPINDEX,ELEMENT)	if (m_operands[OPINDEX].flags & OPERANDFLAG_LINK) { Antikythera::operators[m_operands[OPINDEX].operatorIndex]->getResult(m_operands[OPINDEX].resultIndex, operandElementIndex(OPINDEX, ELEMENT), &VNAME); } else { VNAME = CNAME[constantElementIndex(OPINDEX, ELEMENT)]; }


struct ATK_OPERAND {
	uint8_t flags;				// lower 2 bits used for data type
	uint16_t operatorIndex;
	uint8_t resultIndex;
};


class ATKIOperator {
protected:
	ATKIOperator();
public:
	virtual ~ATKIOperator();

public:
	virtual bool load(Stream *program);
protected:
	virtual bool loadProperties(Stream *program) { return true; }
	bool loadFlags(Stream *program, uint8_t *flags);
	bool loadOperatorIndex(Stream *program, uint16_t *index);
	bool loadResultIndex(Stream *program, uint8_t *index);
	bool loadConstant(Stream *program, uint8_t operandIndex, uint8_t flags);
	virtual bool initializeConstant(uint8_t operandIndex, uint16_t constantSize);
	virtual void setConstant(uint8_t operandIndex, uint16_t element, void *value) { }

public:
#ifdef ANTIKYTHERA_DEBUG
	virtual bool evaluate(unsigned long now, Stream *debug);
#else
	virtual bool evaluate(unsigned long now);
#endif

	uint16_t operandElementIndex(uint8_t operandIndex, uint16_t iteration);
	uint16_t constantElementIndex(uint8_t operandIndex, uint16_t iteration);
	virtual void getResult(uint8_t resultIndex, uint16_t element, void *value) { }

	// debug
#ifdef ANTIKYTHERA_DEBUG
	String m_lastErrorString;
#endif

public:
	String m_name;
	bool m_isEvaluated;
	uint8_t m_numOperands;
	ATK_OPERAND *m_operands;
	uint16_t *m_constantSize;
	uint16_t m_numOperations;
	uint8_t m_numResults;
	uint16_t *m_resultSize;
};


#endif

/* ATK_IOPERATOR_H_ */
