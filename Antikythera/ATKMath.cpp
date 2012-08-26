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
	m_dataType = 0;

	m_result = new int32_t[1];
	m_result[0] = 0;
	m_resultSize = 0;

	m_constOperation = NULL;
	m_constA = NULL;
	m_constB = NULL;
}

ATKMath::~ATKMath() {
	delete[] m_result;

	delete[] m_constOperation;
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

	switch (m_dataType) {
	case OPERANDTYPE_INT8:
#ifdef ANTIKYTHERA_DEBUG
		return evaluateEx<int8_t>(now, debug);
#else
		return evaluateEx<int8_t>(now);
#endif

	case OPERANDTYPE_INT16:
#ifdef ANTIKYTHERA_DEBUG
		return evaluateEx<int16_t>(now, debug);
#else
		return evaluateEx<int16_t>(now);
#endif

	case OPERANDTYPE_INT32:
#ifdef ANTIKYTHERA_DEBUG
		return evaluateEx<int32_t>(now, debug);
#else
		return evaluateEx<int32_t>(now);
#endif

	case OPERANDTYPE_INT64:
#ifdef ANTIKYTHERA_DEBUG
		return evaluateEx<int64_t>(now, debug);
#else
		return evaluateEx<int64_t>(now);
#endif

	case OPERANDTYPE_FLOAT:
#ifdef ANTIKYTHERA_DEBUG
		return evaluateEx<float>(now, debug);
#else
		return evaluateEx<float>(now);
#endif

	case OPERANDTYPE_STRING:
#ifdef ANTIKYTHERA_DEBUG
		return evaluateEx<String>(now, debug);
#else
		return evaluateEx<String>(now);
#endif

	case OPERANDTYPE_UINT8:
#ifdef ANTIKYTHERA_DEBUG
		return evaluateEx<uint8_t>(now, debug);
#else
		return evaluateEx<uint8_t>(now);
#endif

	case OPERANDTYPE_UINT16:
#ifdef ANTIKYTHERA_DEBUG
		return evaluateEx<uint16_t>(now, debug);
#else
		return evaluateEx<uint16_t>(now);
#endif

	case OPERANDTYPE_UINT32:
#ifdef ANTIKYTHERA_DEBUG
		return evaluateEx<uint32_t>(now, debug);
#else
		return evaluateEx<uint32_t>(now);
#endif

	case OPERANDTYPE_UINT64:
#ifdef ANTIKYTHERA_DEBUG
		return evaluateEx<uint64_t>(now, debug);
#else
		return evaluateEx<uint64_t>(now);
#endif

	case OPERANDTYPE_DOUBLE:
#ifdef ANTIKYTHERA_DEBUG
		return evaluateEx<double>(now, debug);
#else
		return evaluateEx<double>(now);
#endif
	}

	return false;
}

#ifdef ANTIKYTHERA_DEBUG
template <class T>bool ATKMath::evaluateEx(unsigned long now, Stream *debug) {
#else
	template <class T>bool ATKMath::evaulateEx(unsigned long now) {
#endif
	delete[] m_result;		// might be a leak
#ifdef ANTIKYTHERA_DEBUG
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif
	m_result = new T[numOperations()];
	m_resultSize = numOperations();

	for (uint8_t i; i < numOperations(); i++) {
		ATK_OPERAND o = operand(0);
		uint8_t operation = OPERAND_ELEMENT(uint8_t, 0, i);
		o = operand(1);
		T a = OPERAND_ELEMENT(T, 1, i);				// t2d: modify to support other types
		o = operand(2);
		T b = OPERAND_ELEMENT(T, 2, i);				// t2d: modify to support other types
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
		break;
	}

	setEvaluatedFlag();

	return result;
}

void *ATKMath::constantGeneric(uint8_t index) {
	switch (index) {
	case 0:
		return m_constOperation;

	case 2:
		return m_constA;

	case 3:
		return m_constB;
	}

	return NULL;
}

bool ATKMath::loadProperties(Stream *program) {
	ATKIOperator::loadProperties(program);

	char buffer[21];
	memset(buffer, 0, 21);
	int index = 0;
	bool valid = false;
	while(Antikythera::readProgram(program)) {
		char c = (char)program->read();
#ifdef ANTIKYTHERA_DEBUG
			program->println(c);
#endif
		if (c == '(') {
			valid = true;
			break;
		}
		if (index == 3) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = "ATKMath::loadProperties() - property count has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = "ATKMath::loadProperties() - property count contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKMath::loadProperties() - unexpected end of stream while reading property count.";
#endif
		program->flush();
		return false;
	}

	uint8_t numProperties = (uint8_t)strtoul(buffer, NULL, 10);

	if (numProperties != 1) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKMath::load() - incorrect number(" + String(numProperties) + ") of properties specified, expected 1.";
#endif
		program->flush();
		return false;
	}

	memset(buffer, 0, 21);
	index = 0;
	valid = false;
	while(Antikythera::readProgram(program)) {
		char c = (char)program->read();
#ifdef ANTIKYTHERA_DEBUG
			program->println(c);
#endif
		if (c == ')') {
			valid = true;
			break;
		}
		if (index == 3) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = "ATKMath::loadProperties() - data type has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = "ATKMath::loadProperties() - data type contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKMath::loadProperties() - unexpected end of stream while reading data type.";
#endif
		program->flush();
		return false;
	}

	uint8_t numProperties = (uint8_t)strtoul(buffer, NULL, 10);

	return true;
}

bool ATKMath::loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program) {
	return ATKIOperator::loadConstant(operandIndex, flags, program);
}

bool ATKMath::initializeConstant(uint8_t operandIndex, uint8_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

	switch (operandIndex) {
	case 0:
		m_constOperation = new uint8_t[constantSize];
		break;

	case 1:
		switch (m_dataType) {
		case OPERANDTYPE_INT8:
			m_constA = new int8_t[constantSize];
			break;

		case OPERANDTYPE_INT16:
			m_constA = new int16_t[constantSize];
			break;

		case OPERANDTYPE_INT32:
			m_constA = new int32_t[constantSize];
			break;

		case OPERANDTYPE_INT64:
			m_constA = new int64_t[constantSize];
			return true;

		case OPERANDTYPE_FLOAT:
			m_constA = new float[constantSize];
			return true;

		case OPERANDTYPE_STRING:
			m_constA = new String[constantSize];
			return true;

		case OPERANDTYPE_UINT8:
			m_constA = new uint8_t[constantSize];
			break;

		case OPERANDTYPE_UINT16:
			m_constA = new uint16_t[constantSize];
			break;

		case OPERANDTYPE_UINT32:
			m_constA = new uint32_t[constantSize];
			break;

		case OPERANDTYPE_UINT64:
			m_constA = new uint64_t[constantSize];
			return true;

		case OPERANDTYPE_DOUBLE:
			m_constA = new double[constantSize];
			return true;
		}
		break;

	case 2:
		switch (m_dataType) {
		case OPERANDTYPE_INT8:
			m_constB = new int8_t[constantSize];
			break;

		case OPERANDTYPE_INT16:
			m_constB = new int16_t[constantSize];
			break;

		case OPERANDTYPE_INT32:
			m_constB = new int32_t[constantSize];
			break;

		case OPERANDTYPE_INT64:
			m_constB = new int64_t[constantSize];
			return true;

		case OPERANDTYPE_FLOAT:
			m_constB = new float[constantSize];
			return true;

		case OPERANDTYPE_STRING:
			m_constB = new String[constantSize];
			return true;

		case OPERANDTYPE_UINT8:
			m_constB = new uint8_t[constantSize];
			break;

		case OPERANDTYPE_UINT16:
			m_constB = new uint16_t[constantSize];
			break;

		case OPERANDTYPE_UINT32:
			m_constB = new uint32_t[constantSize];
			break;

		case OPERANDTYPE_UINT64:
			m_constB = new uint64_t[constantSize];
			return true;

		case OPERANDTYPE_DOUBLE:
			m_constB = new double[constantSize];
			return true;
		}
		break;

	default:
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKMath::initializeConstant() - operandIndex out of range.";
#endif
		return false;
	}

	return true;
}
