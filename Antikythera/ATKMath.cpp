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
	m_name = "Math";

	m_result = NULL;
	m_resultSize = new uint16_t[1];
	m_resultSize[0] = 0;

	m_constOperation = NULL;
	m_constA = NULL;
	m_constB = NULL;
	m_constC = NULL;
}

ATKMath::~ATKMath() {
	delete[] m_result;

	delete[] m_constOperation;
	delete[] m_constA;
	delete[] m_constB;
	delete[] m_constC;
}

bool ATKMath::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (m_numOperands != 4) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "IMath::load() - incorrect number(" + String(m_numOperands) + ") of operands specified, expected 4.";
#endif
		program->flush();
		return false;
	}

	return true;
}

bool ATKMath::loadProperties(Stream *program) {
	return ATKIOperator::loadProperties(program);
}

void ATKMath::initializeOperands(uint8_t numOperands) {
	ATKIOperator::initializeOperands(numOperands);
}

bool ATKMath::initializeConstant(uint8_t operandIndex, uint16_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

	switch (operandIndex) {
	case 0:
		m_constOperation = new int16_t[constantSize];
		break;

	case 1:
		m_constA = new int16_t[constantSize];
		break;

	case 2:
		m_constB = new int16_t[constantSize];
		break;

	case 3:
		m_constC = new int16_t[constantSize];
		break;

	default:
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "IMath::initializeConstant() - operandIndex out of range.";
#endif
		return false;
	}

	return true;
}

void ATKMath::setConstant(uint8_t operandIndex, uint16_t element, void *value) {
	switch (operandIndex) {
	case 0:
		m_constOperation[element] = *((int16_t *)value);
		break;

	case 1:
		m_constA[element] = *((int16_t *)value);
		break;

	case 2:
		m_constB[element] = *((int16_t *)value);
		break;

	case 3:
		m_constC[element] = *((int16_t *)value);
		break;
	}
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKMath::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKMath::evaluate(unsigned long now) {
#endif
	if (m_isEvaluated) {
		return true;
	}

#ifdef ANTIKYTHERA_DEBUG
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	delete[] m_result;
	m_numResults = 1;
	m_result = new int16_t[m_numOperations];
	m_resultSize[0] = m_numOperations;

	for (int16_t i; i < m_numOperations; i++) {
		int16_t operation;
		OPERAND_ELEMENT(operation, m_constOperation, 0, i)
		int16_t a;
		OPERAND_ELEMENT(a, m_constA, 1, i)
		int16_t b;
		OPERAND_ELEMENT(b, m_constB, 2, i)
		int16_t c;
		OPERAND_ELEMENT(c, m_constC, 3, i)

		switch (operation) {
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

		case MATH_SPLAY:			// splay b values above a with c increment a, a+c, a+2c...a+(b-1)c
			int16_t *temp = new int16_t[m_resultSize[0] + b - 1];
			for (uint16_t count = 0; count < m_resultSize[0]; count++) {
				temp[count] = m_result[count];
			}
			m_resultSize[0] += (b - 1);
			delete[] m_result;
			m_result = temp;

			for (int16_t count = 0; count < b; count++) {
				m_result[i + count] = a + count * c;
			}
			break;
		}
#ifdef ANTIKYTHERA_DEBUG
		debug->println("IMath::evaluate(" + String(now) + ", " + String(i) + ": " + String(operation) + ", " + String(a) + ", " + String(b)+ ", " + String(c) + ") = " + String(m_result[i]));
#endif
	}

	m_isEvaluated = true;

	return result;
}

void ATKMath::getResult(uint8_t resultIndex, uint16_t element, void *value) {
	if (resultIndex < m_numResults) {
		if (element < m_resultSize[0]) {
			*((int16_t *)value) = m_result[element];
		}
	}
}
