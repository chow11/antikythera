/*
 * ATKIntegerMath.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <WProgram.h>
#include <ATKIntegerMath.h>
#include <Antikythera.h>

template <typename T>
ATKIntegerMath<T>::ATKIntegerMath() {
	m_dataType = 0;

	m_result = NULL;
	m_resultSize = 0;

	m_constOperation = NULL;
	m_constA = NULL;
	m_constB = NULL;
	m_constC = NULL;
}

template <typename T>
ATKIntegerMath<T>::~ATKIntegerMath() {
	delete[] m_result;

	delete[] m_constOperation;
	delete[] m_constA;
	delete[] m_constB;
	delete[] m_constC;
}

template <typename T>
bool ATKIntegerMath<T>::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (numOperands() != 4) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKIntegerMath::load() - incorrect number(" + String(numOperands()) + ") of operands specified, expected 4.";
#endif
		program->flush();
		return false;
	}

	return true;
}

template <typename T>
bool ATKIntegerMath<T>::loadProperties(Stream *program) {
	return ATKIOperator::loadProperties(program);
}

template <typename T>
bool ATKIntegerMath<T>::initializeConstant(uint8_t operandIndex, uint8_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

	switch (operandIndex) {
	case 0:
		m_constOperation = new uint8_t[constantSize];
		break;

	case 1:
		m_constA = new T[constantSize];
		break;

	case 2:
		m_constB = new T[constantSize];
		break;

	case 3:
		m_constC = new T[constantSize];
		break;

	default:
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKIntegerMath::initializeConstant() - operandIndex out of range.";
#endif
		return false;
	}

	return true;
}

#ifdef ANTIKYTHERA_DEBUG
template <>
bool ATKIntegerMath<int8_t>::evaluate(unsigned long now, Stream *debug) {
#else
template <>
bool ATKIntegerMath<int8_t>::evaluate(unsigned long now) {
#endif
	if (isEvaluated()) {
		return true;
	}
	delete[] m_result;
	m_resultSize = 0;
#ifdef ANTIKYTHERA_DEBUG
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	m_result = new int8_t[numOperations()];
	m_resultSize = numOperations();

	for (uint8_t i; i < numOperations(); i++) {
		OPERAND_ELEMENT(operation, OPERANDTYPE_UINT8, uint8_t, 0, i)
		OPERAND_ELEMENT(a, OPERANDTYPE_INT8, int8_t, 1, i)
		OPERAND_ELEMENT(b, OPERANDTYPE_INT8, int8_t, 2, i)
		OPERAND_ELEMENT(c, OPERANDTYPE_INT8, int8_t, 3, i)

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
			int8_t *temp = new int8_t[m_resultSize + b - 1];
			for (int count = 0; count < m_resultSize; count++) {
				temp[count] = m_result[count];
			}
			m_resultSize += (b - 1);
			delete[] m_result;
			m_result = temp;

			for (int8_t count = 0; count < b; count++) {
				m_result[i + count] = a + count * c;
			}
			break;
		}
#ifdef ANTIKYTHERA_DEBUG
		debug->println("ATKIntegerMath<int8_t>::evaluate(" + String(now) + ", " + String((int)i) + ": " + String((int)operation) + ", " + String((int)a) + ", " + String((int)b)+ ", " + String((int)c) + ") = " + String((int)m_result[i]));
#endif
	}

	setEvaluatedFlag();

	return result;
}

#ifdef ANTIKYTHERA_DEBUG
template <>
bool ATKIntegerMath<int16_t>::evaluate(unsigned long now, Stream *debug) {
#else
template <>
bool ATKIntegerMath<int16_t>::evaluate(unsigned long now) {
#endif
	if (isEvaluated()) {
		return true;
	}
	delete[] m_result;
	m_resultSize = 0;
#ifdef ANTIKYTHERA_DEBUG
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	m_result = new int16_t[numOperations()];
	m_resultSize = numOperations();

	for (uint8_t i; i < numOperations(); i++) {
		OPERAND_ELEMENT(operation, OPERANDTYPE_UINT8, uint8_t, 0, i)
		OPERAND_ELEMENT(a, OPERANDTYPE_INT16, int16_t, 1, i)
		OPERAND_ELEMENT(b, OPERANDTYPE_INT16, int16_t, 2, i)
		OPERAND_ELEMENT(c, OPERANDTYPE_INT16, int16_t, 3, i)

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
			int16_t *temp = new int16_t[m_resultSize + b - 1];
			for (int count = 0; count < m_resultSize; count++) {
				temp[count] = m_result[count];
			}
			m_resultSize += (b - 1);
			delete[] m_result;
			m_result = temp;

			for (int16_t count = 0; count < b; count++) {
				m_result[i + count] = a + count * c;
			}
			break;
		}
#ifdef ANTIKYTHERA_DEBUG
		debug->println("ATKIntegerMath<int16_t>::evaluate(" + String(now) + ", " + String((int)i) + ": " + String((int)operation) + ", " + String((int)a) + ", " + String((int)b)+ ", " + String((int)c) + ") = " + String((int)m_result[i]));
#endif
	}

	setEvaluatedFlag();

	return result;
}

#ifdef ANTIKYTHERA_DEBUG
template <>
bool ATKIntegerMath<int32_t>::evaluate(unsigned long now, Stream *debug) {
#else
template <>
bool ATKIntegerMath<int32_t>::evaluate(unsigned long now) {
#endif
	if (isEvaluated()) {
		return true;
	}
	delete[] m_result;
	m_resultSize = 0;
#ifdef ANTIKYTHERA_DEBUG
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	m_result = new int32_t[numOperations()];
	m_resultSize = numOperations();

	for (uint8_t i; i < numOperations(); i++) {
		OPERAND_ELEMENT(operation, OPERANDTYPE_UINT8, uint8_t, 0, i)
		OPERAND_ELEMENT(a, OPERANDTYPE_INT32, int32_t, 1, i)
		OPERAND_ELEMENT(b, OPERANDTYPE_INT32, int32_t, 2, i)
		OPERAND_ELEMENT(c, OPERANDTYPE_INT32, int32_t, 3, i)

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
			int32_t *temp = new int32_t[m_resultSize + b - 1];
			for (int count = 0; count < m_resultSize; count++) {
				temp[count] = m_result[count];
			}
			m_resultSize += (b - 1);
			delete[] m_result;
			m_result = temp;

			for (int32_t count = 0; count < b; count++) {
				m_result[i + count] = a + count * c;
			}
			break;
		}
#ifdef ANTIKYTHERA_DEBUG
		debug->println("ATKIntegerMath<int32_t>::evaluate(" + String(now) + ", " + String((int)i) + ": " + String((int)operation) + ", " + String((int)a) + ", " + String((int)b)+ ", " + String((int)c) + ") = " + String((int)m_result[i]));
#endif
	}

	setEvaluatedFlag();

	return result;
}

#ifdef ANTIKYTHERA_DEBUG
template <>
bool ATKIntegerMath<uint8_t>::evaluate(unsigned long now, Stream *debug) {
#else
template <>
bool ATKIntegerMath<uint8_t>::evaluate(unsigned long now) {
#endif
	if (isEvaluated()) {
		return true;
	}
	delete[] m_result;
	m_resultSize = 0;
#ifdef ANTIKYTHERA_DEBUG
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	m_result = new uint8_t[numOperations()];
	m_resultSize = numOperations();

	for (uint8_t i; i < numOperations(); i++) {
		OPERAND_ELEMENT(operation, OPERANDTYPE_UINT8, uint8_t, 0, i)
		OPERAND_ELEMENT(a, OPERANDTYPE_UINT8, uint8_t, 1, i)
		OPERAND_ELEMENT(b, OPERANDTYPE_UINT8, uint8_t, 2, i)
		OPERAND_ELEMENT(c, OPERANDTYPE_UINT8, uint8_t, 3, i)

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
			uint8_t *temp = new uint8_t[m_resultSize + b - 1];
			for (int count = 0; count < m_resultSize; count++) {
				temp[count] = m_result[count];
			}
			m_resultSize += (b - 1);
			delete[] m_result;
			m_result = temp;

			for (uint8_t count = 0; count < b; count++) {
				m_result[i + count] = a + count * c;
			}
			break;
		}
#ifdef ANTIKYTHERA_DEBUG
		debug->println("ATKIntegerMath<uint8_t>::evaluate(" + String(now) + ", " + String((int)i) + ": " + String((int)operation) + ", " + String((int)a) + ", " + String((int)b)+ ", " + String((int)c) + ") = " + String((int)m_result[i]));
#endif
	}

	setEvaluatedFlag();

	return result;
}

#ifdef ANTIKYTHERA_DEBUG
template <>
bool ATKIntegerMath<uint16_t>::evaluate(unsigned long now, Stream *debug) {
#else
template <>
bool ATKIntegerMath<uint16_t>::evaluate(unsigned long now) {
#endif
	if (isEvaluated()) {
		return true;
	}
	delete[] m_result;
	m_resultSize = 0;
#ifdef ANTIKYTHERA_DEBUG
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	m_result = new uint16_t[numOperations()];
	m_resultSize = numOperations();

	for (uint8_t i; i < numOperations(); i++) {
		OPERAND_ELEMENT(operation, OPERANDTYPE_UINT8, uint8_t, 0, i)
		OPERAND_ELEMENT(a, OPERANDTYPE_UINT16, uint16_t, 1, i)
		OPERAND_ELEMENT(b, OPERANDTYPE_UINT16, uint16_t, 2, i)
		OPERAND_ELEMENT(c, OPERANDTYPE_UINT16, uint16_t, 3, i)

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
			uint16_t *temp = new uint16_t[m_resultSize + b - 1];
			for (int count = 0; count < m_resultSize; count++) {
				temp[count] = m_result[count];
			}
			m_resultSize += (b - 1);
			delete[] m_result;
			m_result = temp;

			for (uint16_t count = 0; count < b; count++) {
				m_result[i + count] = a + count * c;
			}
			break;
		}
#ifdef ANTIKYTHERA_DEBUG
		debug->println("ATKIntegerMath<uint16_t>::evaluate(" + String(now) + ", " + String((int)i) + ": " + String((int)operation) + ", " + String((int)a) + ", " + String((int)b)+ ", " + String((int)c) + ") = " + String((int)m_result[i]));
#endif
	}

	setEvaluatedFlag();

	return result;
}

#ifdef ANTIKYTHERA_DEBUG
template <>
bool ATKIntegerMath<uint32_t>::evaluate(unsigned long now, Stream *debug) {
#else
template <>
bool ATKIntegerMath<uint32_t>::evaluate(unsigned long now) {
#endif
	if (isEvaluated()) {
		return true;
	}
	delete[] m_result;
	m_resultSize = 0;
#ifdef ANTIKYTHERA_DEBUG
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	m_result = new uint32_t[numOperations()];
	m_resultSize = numOperations();

	for (uint8_t i; i < numOperations(); i++) {
		OPERAND_ELEMENT(operation, OPERANDTYPE_UINT8, uint8_t, 0, i)
		OPERAND_ELEMENT(a, OPERANDTYPE_UINT32, uint32_t, 1, i)
		OPERAND_ELEMENT(b, OPERANDTYPE_UINT32, uint32_t, 2, i)
		OPERAND_ELEMENT(c, OPERANDTYPE_UINT32, uint32_t, 3, i)

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
			uint32_t *temp = new uint32_t[m_resultSize + b - 1];
			for (int count = 0; count < m_resultSize; count++) {
				temp[count] = m_result[count];
			}
			m_resultSize += (b - 1);
			delete[] m_result;
			m_result = temp;

			for (uint32_t count = 0; count < b; count++) {
				m_result[i + count] = a + count * c;
			}
			break;
		}
#ifdef ANTIKYTHERA_DEBUG
		debug->println("ATKIntegerMath<uint32_t>::evaluate(" + String(now) + ", " + String((int)i) + ": " + String((int)operation) + ", " + String((int)a) + ", " + String((int)b)+ ", " + String((int)c) + ") = " + String((int)m_result[i]));
#endif
	}

	setEvaluatedFlag();

	return result;
}

template <typename T>
void *ATKIntegerMath<T>::constantGeneric(uint8_t index) {
	switch (index) {
	case 0:
		return m_constOperation;

	case 1:
		return m_constA;

	case 2:
		return m_constB;

	case 3:
		return m_constC;
	}

	return NULL;
}

template <typename T>
void ATKIntegerMath<T>::result(uint8_t index, uint8_t element, void *value, uint8_t valueType) {
	if (index < numResults()) {
		if (element < m_resultSize) {
			switch (valueType) {
			case OPERANDTYPE_INT8:
				*((int8_t *)value) = m_result[element];
				break;

			case OPERANDTYPE_INT16:
				*((int16_t *)value) = m_result[element];
				break;

			case OPERANDTYPE_INT32:
				*((int32_t *)value) = m_result[element];
				break;

			case OPERANDTYPE_UINT8:
				*((uint8_t *)value) = m_result[element];
				break;

			case OPERANDTYPE_UINT16:
				*((uint16_t *)value) = m_result[element];
				break;

			case OPERANDTYPE_UINT32:
				*((uint32_t *)value) = m_result[element];
				break;

			}
		}
	}
}

template class ATKIntegerMath<int8_t>;
template class ATKIntegerMath<int16_t>;
template class ATKIntegerMath<int32_t>;
template class ATKIntegerMath<uint8_t>;
template class ATKIntegerMath<uint16_t>;
template class ATKIntegerMath<uint32_t>;
