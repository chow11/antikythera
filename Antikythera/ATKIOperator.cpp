/*
 * ATKIOperator.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <ATKIOperator.h>
#include <Antikythera.h>


ATKIOperator::ATKIOperator() {
	m_isEvaluated = false;
	m_numOperands = 0;
	m_operands = NULL;
	m_numOperations = 0;
	m_numConstants = NULL;
}

ATKIOperator::~ATKIOperator() {
	delete[] m_operands;
	delete[] m_numConstants;
}

String ATKIOperator::name() {
	return "";
}

// operand count((operand0 flags, operand0 operator index, operand0 result index)...(operand0 flags, [operandN operator index, operandN result index|operandN constant value]))
// alternate operand specification:
// (operand0 flags, constant count(constant0 value,...,constantN value))
bool ATKIOperator::load(Stream *program) {
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
			m_lastErrorString = name() + "::load() - operand count has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = name() + "::load() - operand count contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = name() + "::load() - unexpected end of stream while reading operand count.";
#endif
		program->flush();
		return false;
	}

	m_numOperands = (uint8_t)strtoul(buffer, NULL, 10);
#ifdef ANTIKYTHERA_DEBUG
	program->print("[num operands:");
	program->print(buffer);
	program->println("]");
#endif
	m_operands = new ATK_OPERAND[m_numOperands];
	m_numConstants = new uint8_t[m_numOperands];

	for (int count = 0; count < numOperands(); count++) {
		valid = false;
		while(Antikythera::readProgram(program)) {
			char c = (char)program->read();
#ifdef ANTIKYTHERA_DEBUG
			program->println(c);
#endif
			if (c == '(') {
				valid = true;
				break;
			}
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = name() + "::load() - operand[" + String(count) + "] expected opening parenthesis, read invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = name() + "::load() - operand[" + String(count) + "] unexpected end of stream while reading opening parenthesis.";
#endif
			program->flush();
			return false;
		}

		if (!loadProperties(program)) {
			return false;
		}

		m_operands[count].flags = loadFlags(program);
		if (m_operands[count].flags & OPERANDFLAG_LINK) {
			m_operands[count].operatorIndex = loadOperatorIndex(program);
			m_operands[count].resultIndex = loadResultIndex(program);
		} else {
			if (!loadConstant(count, m_operands[count].flags, program)) {
				return false;
			}

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
#ifdef ANTIKYTHERA_DEBUG
				m_lastErrorString = name() + "::load() - expected closing parenthesis, read invalid character: " + String(c);
#endif
				program->flush();
				return false;
			}
			if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
				m_lastErrorString = name() + "::load() - unexpected end of stream while reading closing parenthesis.";
#endif
				program->flush();
				return false;
			}
		}
	}

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
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = name() + "::load() - expected closing parenthesis, read invalid character: " + String(c);
#endif
		program->flush();
		return false;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = name() + "::load() - unexpected end of stream while reading closing parenthesis.";
#endif
		program->flush();
		return false;
	}

	// connect root to leaf nodes

	return true;
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKIOperator::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKIOperator::evaluate(unsigned long now) {
#endif
	bool result = true;

	for (uint8_t count = 0; count < numOperands(); count++) {
		if ((operand(count).flags & OPERANDFLAG_LINK)) {
#ifdef ANTIKYTHERA_DEBUG
			result &= Antikythera::operators[operand(count).operatorIndex]->evaluate(now, debug);
#else
			result &= Antikythera::operators[operand(count).operatorIndex]->evaluate(now);
#endif
		}
	}

	uint8_t max = 0;
	uint8_t min = 255;
	for (uint8_t i = 0; i < numOperands(); i++) {
		ATK_OPERAND o = operand(i);
		if (o.flags & OPERANDFLAG_LIMIT) {
			uint8_t temp = (o.flags & OPERANDFLAG_LINK) ? Antikythera::operators[o.operatorIndex]->resultSize(o.resultIndex) : numConstants(i);
			if (temp < min) {
				min = temp;
			}
		} else {
			uint8_t temp = (o.flags & OPERANDFLAG_LINK) ? Antikythera::operators[o.operatorIndex]->resultSize(o.resultIndex) : numConstants(i);
			if (temp > max) {
				max = temp;
			}
		}
	}
	m_numOperations = min;
	if (max < min) {
		m_numOperations = max;
	}

	return result;
};

uint8_t ATKIOperator::numResults() {
	return 0;
}

uint8_t ATKIOperator::resultSize(uint8_t index) {
	return 0;
}

bool ATKIOperator::isEvaluated() {
	return m_isEvaluated;
}

void ATKIOperator::resetEvaluatedFlag() {
	m_isEvaluated = false;
}

void ATKIOperator::setEvaluatedFlag() {
	m_isEvaluated = true;
}

void *ATKIOperator::resultGeneric(uint8_t index) {
	return NULL;
}

uint8_t ATKIOperator::operandElementIndex(uint8_t operandIndex, ATK_OPERAND o, uint8_t iteration) {
	uint8_t result = 0;

	uint8_t operandElementSize = (o.flags & OPERANDFLAG_LINK) ? Antikythera::operators[o.operatorIndex]->resultSize(o.resultIndex) : numConstants(operandIndex);
	if (o.flags & OPERANDFLAG_SINGLE) {
		result = 0;
	} else {
		if (m_numOperations < operandElementSize) {
			result = iteration;
		} else {
			if (o.flags & OPERANDFLAG_EXTEND) {
				result = operandElementSize - 1;
			} else {
				result = iteration % operandElementSize;
			}
		}
	}

	return result;
}

void *ATKIOperator::constantGeneric(uint8_t index) {
	return NULL;
}

bool ATKIOperator::loadProperties(Stream *program) {
	return true;
}

uint8_t ATKIOperator::loadFlags(Stream *program) {
	char buffer[21];
	memset(buffer, 0, 21);
	int index = 0;
	bool valid = false;
	while(Antikythera::readProgram(program)) {
		char c = (char)program->read();
#ifdef ANTIKYTHERA_DEBUG
			program->println(c);
#endif
		if (c == ',') {
			valid = true;
			break;
		}
		if (index == 3) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = name() + "::load() - operand flags has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = name() + "::load() - operand flags contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = name() + "::load() - unexpected end of stream while reading flags.";
#endif
		program->flush();
		return false;
	}

#ifdef ANTIKYTHERA_DEBUG
	program->print("[flags:");
	program->print(buffer);
	program->println("]");
#endif
	return (uint8_t)strtoul(buffer, NULL, 10);
}

uint16_t ATKIOperator::loadOperatorIndex(Stream *program) {
	char buffer[21];
	memset(buffer, 0, 21);
	int index = 0;
	bool valid = false;
	while(Antikythera::readProgram(program)) {
		char c = (char)program->read();
#ifdef ANTIKYTHERA_DEBUG
			program->println(c);
#endif
		if (c == ',') {
			valid = true;
			break;
		}
		if (index == 5) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = name() + "::load() - operator index has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = name() + "::load() - operator index contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = name() + "::load() - unexpected end of stream while reading operator index.";
#endif
		program->flush();
		return false;
	}

#ifdef ANTIKYTHERA_DEBUG
	program->print("[operator index:");
	program->print(buffer);
	program->println("]");
#endif
	return (uint16_t)strtoul(buffer, NULL, 10);
}

uint8_t ATKIOperator::loadResultIndex(Stream *program) {
	char buffer[21];
	memset(buffer, 0, 21);
	int index = 0;
	bool valid = false;
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
			m_lastErrorString = name() + "::load() - result index has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = name() + "::load() - result index contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = name() + "::load() - unexpected end of stream while reading result index.";
#endif
		program->flush();
		return false;
	}

#ifdef ANTIKYTHERA_DEBUG
	program->print("[result index:");
	program->print(buffer);
	program->println("]");
#endif
	return (uint8_t)strtoul(buffer, NULL, 10);
}

// update to support more types
bool ATKIOperator::loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program) {
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
			Antikythera::lastErrorString = name() + "::load() - constant count has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			Antikythera::lastErrorString = name() + "::load() - constant count contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		Antikythera::lastErrorString = name() + "::load() - unexpected end of stream while reading constant count.";
#endif
		program->flush();
		return false;
	}

#ifdef ANTIKYTHERA_DEBUG
	program->print("[initialize constant:");
	program->print((int)operandIndex);
	program->print(", count:");
	program->print(buffer);
	program->println("]");
#endif
	initializeConstant(operandIndex, (uint8_t)strtoul(buffer, NULL, 10));
	uint8_t operandType = flags & 0x0F;
	uint8_t maxLength = 0;
	switch (operandType) {
	case OPERANDTYPE_INT8:
		maxLength = 4;
		break;

	case OPERANDTYPE_INT16:
		maxLength = 6;
		break;

	case OPERANDTYPE_INT32:
		maxLength = 11;
		break;

	case OPERANDTYPE_INT64:
		return true;

	case OPERANDTYPE_FLOAT:
		return true;

	case OPERANDTYPE_STRING:
		return true;

	case OPERANDTYPE_UINT8:
		maxLength = 3;
		break;

	case OPERANDTYPE_UINT16:
		maxLength = 5;
		break;

	case OPERANDTYPE_UINT32:
		maxLength = 10;
		break;

	case OPERANDTYPE_UINT64:
		return true;

	case OPERANDTYPE_DOUBLE:
		return true;

	case OPERANDTYPE_CUSTOM:
		return true;
	}

#ifdef ANTIKYTHERA_DEBUG
	program->print("[reading ");
	program->print((int)numConstants(operandIndex));
	program->print(", type:");
	program->print((int)operandType);
	program->println(" constants]");
#endif
	for (int count = 0; count < numConstants(operandIndex); count++) {
		memset(buffer, 0, 21);
		index = 0;
		valid = false;
		if ((operandType >= OPERANDTYPE_INT8) && (operandType <= OPERANDTYPE_INT32)) {
			while(Antikythera::readProgram(program)) {
				char c = (char)program->read();
#ifdef ANTIKYTHERA_DEBUG
			program->println(c);
#endif
				if (c == ',') {
					if (count == (numConstants(operandIndex) - 1)) {
#ifdef ANTIKYTHERA_DEBUG
						m_lastErrorString = name() + "::load() - constant count is less than number of constants.";
#endif
						program->flush();
						return false;
					}
					valid = true;
					break;
				}
				if (c == ')') {
					if (count != (numConstants(operandIndex) - 1)) {
#ifdef ANTIKYTHERA_DEBUG
						m_lastErrorString = name() + "::load() - constant count is greater than number of constants.";
#endif
						program->flush();
						return false;
					}
					valid = true;
					break;
				}
				if (index == maxLength) {
#ifdef ANTIKYTHERA_DEBUG
					m_lastErrorString = name() + "::load() - constant has too many digits.";
#endif
					program->flush();
					return false;
				}
				if (c && (strchr("-0123456789", c) == NULL)) {
#ifdef ANTIKYTHERA_DEBUG
					m_lastErrorString = name() + "::load() - constant contains invalid character: " + String(c);
#endif
					program->flush();
					return false;
				}
				buffer[index++] = c;
			}
			if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
				m_lastErrorString = name() + "::load() - unexpected end of stream while reading constant.";
#endif
				program->flush();
				return false;
			}
		} else if ((operandType >= OPERANDTYPE_UINT8) && (operandType <= OPERANDTYPE_UINT32)) {
			while(Antikythera::readProgram(program)) {
				char c = (char)program->read();
#ifdef ANTIKYTHERA_DEBUG
			program->println(c);
#endif
				if (c == ',') {
					if (count == (numConstants(operandIndex) - 1)) {
#ifdef ANTIKYTHERA_DEBUG
						m_lastErrorString = name() + "::load() - constant count is less than number of constants.";
#endif
						program->flush();
						return false;
					}
					valid = true;
					break;
				}
				if (c == ')') {
					if (count != (numConstants(operandIndex) - 1)) {
#ifdef ANTIKYTHERA_DEBUG
						m_lastErrorString = name() + "::load() - constant count is greater than number of constants.";
#endif
						program->flush();
						return false;
					}
					valid = true;
					break;
				}
				if (index == maxLength) {
#ifdef ANTIKYTHERA_DEBUG
					m_lastErrorString = name() + "::load() - constant has too many digits.";
#endif
					program->flush();
					return false;
				}
				if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
					m_lastErrorString = name() + "::load() - constant contains invalid character: " + String(c);
#endif
					program->flush();
					return false;
				}
				buffer[index++] = c;
			}
			if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
				m_lastErrorString = name() + "::load() - unexpected end of stream while reading constant.";
#endif
				program->flush();
				return false;
			}
		}

#ifdef ANTIKYTHERA_DEBUG
	program->print("[value");
	program->print((int)count);
	program->print(":");
	program->print(buffer);
	program->println("]");
#endif
		switch (operandType) {
		case OPERANDTYPE_INT8:
			constant<int8_t>(operandIndex)[count] = (int8_t)strtol(buffer, NULL, 10);
			break;

		case OPERANDTYPE_INT16:
			constant<int16_t>(operandIndex)[count] = (int16_t)strtol(buffer, NULL, 10);
			break;

		case OPERANDTYPE_INT32:
			constant<int32_t>(operandIndex)[count] = (int32_t)strtol(buffer, NULL, 10);
			break;

		case OPERANDTYPE_INT64:
			return true;

		case OPERANDTYPE_FLOAT:
			return true;

		case OPERANDTYPE_STRING:
			return true;

		case OPERANDTYPE_UINT8:
			constant<uint8_t>(operandIndex)[count] = (uint8_t)strtoul(buffer, NULL, 10);
			break;

		case OPERANDTYPE_UINT16:
			constant<uint16_t>(operandIndex)[count] = (uint16_t)strtoul(buffer, NULL, 10);
			break;

		case OPERANDTYPE_UINT32:
			constant<uint32_t>(operandIndex)[count] = (uint32_t)strtoul(buffer, NULL, 10);
			break;

		case OPERANDTYPE_UINT64:
			return true;

		case OPERANDTYPE_DOUBLE:
			return true;

		case OPERANDTYPE_CUSTOM:
			return true;
		}
	}

	return true;
}

bool ATKIOperator::initializeConstant(uint8_t operandIndex, uint8_t constantSize) {
	m_numConstants[operandIndex] = constantSize;
	return true;
}
