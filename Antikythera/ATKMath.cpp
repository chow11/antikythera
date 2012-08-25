/*
 * ATKMath.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <WProgram.h>
#include <ATKMath.h>
#include <Antikythera.h>


ATKMath::ATKMath() {
	m_result = new int32_t[1];
	m_result[0] = 0;
	m_resultSize = 0;

	m_constOperation = NULL;
	m_constDataType = NULL;
	m_constA = NULL;
	m_constB = NULL;
}

ATKMath::~ATKMath() {
	delete[] m_result;

	delete[] m_constOperation;
	delete[] m_constDataType;
	delete[] m_constA;
	delete[] m_constB;
}

bool ATKMath::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (numOperands() != 4) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKMath::load() - incorrect number(" + String(numOperands()) + ") of operands specified, expected 4.";
#endif
		program->flush();
		return false;
	}

	return true;
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKMath::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKMath::evaluate(unsigned long now) {
#endif
	delete[] m_result;
#ifdef ANTIKYTHERA_DEBUG
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif
	m_result = new int32_t[numOperations()];
	m_resultSize = numOperations();

	for (uint8_t i; i < numOperations(); i++) {
		ATK_OPERAND o = operand(0);
		uint8_t operation = OPERAND_ELEMENT(uint8_t, 0, i);
		o = operand(1);
		uint8_t dataType = OPERAND_ELEMENT(uint8_t, 1, 0);		// only the first type applies
		o = operand(2);
		int32_t a = OPERAND_ELEMENT(int32_t, 2, i);				// t2d: modify to support other types
		o = operand(3);
		int32_t b = OPERAND_ELEMENT(int32_t, 3, i);				// t2d: modify to support other types

		switch (operation) {
		case MATH_NONE:
			m_result[i] = 0;
			break;

		case MATH_ADDITION:
			m_result[i] = a + b;
			break;

		case MATH_SUBTRACTION:
			m_result[i] = a - b;
			break;

		case MATH_MULTIPLICATION:
			m_result[i] = a * b;
			break;

		case MATH_DIVISION:
			m_result[i] = a / b;
			break;

		case MATH_MODULO:
			m_result[i] = a % b;
			break;

		case MATH_INCREMENT:
			m_result[i] = a + 1;
			break;

		case MATH_DECREMENT:
			m_result[i] = a - 1;
			break;

		case MATH_ABS:
			m_result[i] = abs(a);
			break;
		}
#ifdef ANTIKYTHERA_DEBUG
		debug->println("ATKMath::evaluate(" + String(now) + ", " + String((int)i) + ": " + String((int)operation) + ", " + String(a) + ", " + String(b) + ") = " + String(m_result[i]));
#endif
	}

	setEvaluatedFlag();

	return result;
}

void *ATKMath::constantGeneric(uint8_t index) {
	switch (index) {
	case 0:
		return m_constOperation;

	case 1:
		return m_constDataType;

	case 2:
		return m_constA;

	case 3:
		return m_constB;
	}

	return NULL;
}

bool ATKMath::initializeConstant(uint8_t operandIndex, uint8_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

	switch (operandIndex) {
	case 0:
		m_constOperation = new uint8_t[constantSize];
		break;

	case 1:
		m_constDataType = new uint8_t[constantSize];
		break;

	case 2:
		m_constA = new int32_t[constantSize];
		break;

	case 3:
		m_constB = new int32_t[constantSize];
		break;

	default:
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKMath::initializeConstant() - operandIndex out of range.";
#endif
		return false;
	}

	return true;
}

bool ATKMath::loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program) {
	return ATKIOperator::loadConstant(operandIndex, flags, program);
}
