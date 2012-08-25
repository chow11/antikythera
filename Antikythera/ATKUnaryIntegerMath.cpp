/*
 * ATKUnaryIntegerMath.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <math.h>
#include <WProgram.h>
#include <ATKUnaryIntegerMath.h>
#include <Antikythera.h>


ATKUnaryIntegerMath::ATKUnaryIntegerMath() {
	m_result = new int32_t[1];
	m_result[0] = 0;
	m_resultSize = 0;

	m_constOperation = NULL;
	m_constA = NULL;
}

ATKUnaryIntegerMath::~ATKUnaryIntegerMath() {
	delete[] m_result;

	delete[] m_constOperation;
	delete[] m_constA;
}

bool ATKUnaryIntegerMath::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (numOperands() != 2) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKMath::load() - incorrect number(" + String(numOperands()) + ") of operands specified, expected 2.";
#endif
		program->flush();
		return false;
	}

	return true;
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKUnaryIntegerMath::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKUnaryIntegerMath::evaluate(unsigned long now) {
#endif
	delete[] m_result;
#ifdef ANTIKYTHERA_DEBUG
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif
	m_result = new int16_t[numOperations()];
	m_resultSize = numOperations();

	for (uint8_t i; i < numOperations(); i++) {
		ATK_OPERAND o = operand(0);
		uint8_t operation = OPERAND_ELEMENT(uint8_t, 0, i);
		o = operand(1);
		int32_t a = OPERAND_ELEMENT(int32_t, 1, i);
		o = operand(2);
		int32_t b = OPERAND_ELEMENT(int32_t, 2, i);

		switch (operation) {
		case MATH_NONE:
			m_result[i] = 0;
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
		debug->println("ATKUnaryIntegerMath::evaluate(" + String(now) + ", " + String((int)i) + ": " + String((int)operation) + ", " + String(a) + ", " + String(b) + ") = " + String(m_result[i]));
#endif
	}

	setEvaluatedFlag();

	return result;
}

void *ATKUnaryIntegerMath::constantGeneric(uint8_t index) {
	switch (index) {
	case 0:
		return m_constOperation;

	case 1:
		return m_constA;
	}

	return NULL;
}

bool ATKUnaryIntegerMath::initializeConstant(uint8_t operandIndex, uint8_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

	switch (operandIndex) {
	case 0:
		m_constOperation = new uint8_t[constantSize];
		break;

	case 1:
		m_constA = new int32_t[constantSize];
		break;

	default:
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKMath::initializeConstant() - operandIndex out of range.";
#endif
		return false;
	}

	return true;
}

bool ATKUnaryIntegerMath::loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program) {
	return ATKIOperator::loadConstant(operandIndex, flags, program);
}
