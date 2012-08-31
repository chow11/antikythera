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
	m_name = "";
	m_isEvaluated = false;
	m_numOperands = 0;
	m_operands = NULL;
	m_constantSize = NULL;
	m_numOperations = 0;
	m_numResults = 0;
	m_resultSize = NULL;
}

ATKIOperator::~ATKIOperator() {
	delete[] m_operands;
	delete[] m_constantSize;
	delete[] m_resultSize;
}

// operand count((operand0 flags, operand0 operator index, operand0 result index)...(operand0 flags, [operandN operator index, operandN result index|operandN constant value]))
// alternate operand specification:
// (operand0 flags, constant count(constant0 value,...,constantN value))
bool ATKIOperator::load(Stream *program) {
	if (!loadProperties(program)) {
		program->flush();
		return false;
	}

	char buffer[21];
	memset(buffer, 0, 21);
	int index = 0;
	bool valid = false;
	while(Antikythera::readProgram(program)) {
		char c = (char)program->read();
		program->print(c);

		if (c == '(') {
			valid = true;
			break;
		}
		if (index == 3) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = m_name + "::load() - operand count has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = m_name + "::load() - operand count contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = m_name + "::load() - unexpected end of stream while reading operand count.";
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
	m_constantSize = new uint16_t[m_numOperands];

	for (int count = 0; count < m_numOperands; count++) {
		valid = false;
		while(Antikythera::readProgram(program)) {
			char c = (char)program->read();
			program->print(c);

			if (c == '(') {
				valid = true;
				break;
			}
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = m_name + "::load() - operand[" + String(count) + "] expected opening parenthesis, read invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = m_name + "::load() - operand[" + String(count) + "] unexpected end of stream while reading opening parenthesis.";
#endif
			program->flush();
			return false;
		}

		if (!loadFlags(program, &m_operands[count].flags)) {
			return false;
		}
		if (m_operands[count].flags & OPERANDFLAG_LINK) {
			if (!loadOperatorIndex(program, &m_operands[count].operatorIndex)) {
				return false;
			}
			if (!loadResultIndex(program, &m_operands[count].resultIndex)) {
				return false;
			}
		} else {
			if (!loadConstant(program, count, m_operands[count].flags)) {
				return false;
			}

			valid = false;
			while(Antikythera::readProgram(program)) {
				char c = (char)program->read();
				program->print(c);

				if (c == ')') {
					valid = true;
					break;
				}
#ifdef ANTIKYTHERA_DEBUG
				m_lastErrorString = m_name + "::load() - expected closing parenthesis, read invalid character: " + String(c);
#endif
				program->flush();
				return false;
			}
			if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
				m_lastErrorString = m_name + "::load() - unexpected end of stream while reading closing parenthesis.";
#endif
				program->flush();
				return false;
			}
		}
	}

	valid = false;
	while(Antikythera::readProgram(program)) {
		char c = (char)program->read();
		program->print(c);

		if (c == ')') {
			valid = true;
			break;
		}
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = m_name + "::load() - expected closing parenthesis, read invalid character: " + String(c);
#endif
		program->flush();
		return false;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = m_name + "::load() - unexpected end of stream while reading closing parenthesis.";
#endif
		program->flush();
		return false;
	}

	// connect root to leaf nodes

	return true;
}

bool ATKIOperator::loadFlags(Stream *program, uint8_t *flags) {
	char buffer[21];
	memset(buffer, 0, 21);
	int index = 0;
	bool valid = false;
	while(Antikythera::readProgram(program)) {
		char c = (char)program->read();
		program->print(c);

		if (c == ',') {
			valid = true;
			break;
		}
		if (index == 3) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = m_name + "::load() - operand flags has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = m_name + "::load() - operand flags contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = m_name + "::load() - unexpected end of stream while reading flags.";
#endif
		program->flush();
		return false;
	}

#ifdef ANTIKYTHERA_DEBUG
	program->print("[flags:");
	program->print(buffer);
	program->println("]");
#endif
	*flags = (uint8_t)strtoul(buffer, NULL, 10);
	return true;
}

bool ATKIOperator::loadOperatorIndex(Stream *program, uint16_t *operatorIndex) {
	char buffer[21];
	memset(buffer, 0, 21);
	int index = 0;
	bool valid = false;
	while(Antikythera::readProgram(program)) {
		char c = (char)program->read();
		program->print(c);

		if (c == ',') {
			valid = true;
			break;
		}
		if (index == 5) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = m_name + "::load() - operator index has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = m_name + "::load() - operator index contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = m_name + "::load() - unexpected end of stream while reading operator index.";
#endif
		program->flush();
		return false;
	}

#ifdef ANTIKYTHERA_DEBUG
	program->print("[operator index:");
	program->print(buffer);
	program->println("]");
#endif
	*operatorIndex = (uint16_t)strtoul(buffer, NULL, 10);
	return true;
}

bool ATKIOperator::loadResultIndex(Stream *program, uint8_t *resultIndex) {
	char buffer[21];
	memset(buffer, 0, 21);
	int index = 0;
	bool valid = false;
	while(Antikythera::readProgram(program)) {
		char c = (char)program->read();
		program->print(c);

		if (c == ')') {
			valid = true;
			break;
		}
		if (index == 3) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = m_name + "::load() - result index has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			m_lastErrorString = m_name + "::load() - result index contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = m_name + "::load() - unexpected end of stream while reading result index.";
#endif
		program->flush();
		return false;
	}

#ifdef ANTIKYTHERA_DEBUG
	program->print("[result index:");
	program->print(buffer);
	program->println("]");
#endif
	*resultIndex = (uint8_t)strtoul(buffer, NULL, 10);
	return true;
}

// update to support more types
bool ATKIOperator::loadConstant(Stream *program, uint8_t operandIndex, uint8_t flags) {
	char buffer[21];
	memset(buffer, 0, 21);
	int index = 0;
	bool valid = false;
	while(Antikythera::readProgram(program)) {
		char c = (char)program->read();
		program->print(c);

		if (c == '(') {
			valid = true;
			break;
		}
		if (index == 5) {
#ifdef ANTIKYTHERA_DEBUG
			Antikythera::lastErrorString = m_name + "::load() - constant count has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			Antikythera::lastErrorString = m_name + "::load() - constant count contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		Antikythera::lastErrorString = m_name + "::load() - unexpected end of stream while reading constant count.";
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
	initializeConstant(operandIndex, (uint16_t)strtoul(buffer, NULL, 10));
	uint8_t operandType = flags & 0x0F;
	uint8_t maxLength = 0;
	switch (operandType) {
	case OPERANDTYPE_INT16:
		maxLength = 6;
		break;
	case OPERANDTYPE_FLOAT:
		return true;

	case OPERANDTYPE_STRING:
		return true;

	case OPERANDTYPE_UINT32:
		maxLength = 10;
		break;
	}

#ifdef ANTIKYTHERA_DEBUG
	program->print("[reading ");
	program->print(m_constantSize[operandIndex]);
	program->print(", type:");
	program->print((int)operandType);
	program->println(" constants]");
#endif
	for (int count = 0; count < m_constantSize[operandIndex]; count++) {
		memset(buffer, 0, 21);
		index = 0;
		valid = false;
		if (operandType == OPERANDTYPE_INT16) {
			while(Antikythera::readProgram(program)) {
				char c = (char)program->read();
				program->print(c);

				if (c == ',') {
					if (count == (m_constantSize[operandIndex] - 1)) {
#ifdef ANTIKYTHERA_DEBUG
						m_lastErrorString = m_name + "::load() - constant count is less than number of constants.";
#endif
						program->flush();
						return false;
					}
					valid = true;
					break;
				}
				if (c == ')') {
					if (count != (m_constantSize[operandIndex] - 1)) {
#ifdef ANTIKYTHERA_DEBUG
						m_lastErrorString = m_name + "::load() - constant count is greater than number of constants.";
#endif
						program->flush();
						return false;
					}
					valid = true;
					break;
				}
				if (index == maxLength) {
#ifdef ANTIKYTHERA_DEBUG
					m_lastErrorString = m_name + "::load() - constant has too many digits.";
#endif
					program->flush();
					return false;
				}
				if (c && (strchr("-0123456789", c) == NULL)) {
#ifdef ANTIKYTHERA_DEBUG
					m_lastErrorString = m_name + "::load() - constant contains invalid character: " + String(c);
#endif
					program->flush();
					return false;
				}
				buffer[index++] = c;
			}
			if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
				m_lastErrorString = m_name + "::load() - unexpected end of stream while reading constant.";
#endif
				program->flush();
				return false;
			}
		} else if (operandType == OPERANDTYPE_UINT32) {
			while(Antikythera::readProgram(program)) {
				char c = (char)program->read();
				program->print(c);

				if (c == ',') {
					if (count == (m_constantSize[operandIndex] - 1)) {
#ifdef ANTIKYTHERA_DEBUG
						m_lastErrorString = m_name + "::load() - constant count is less than number of constants.";
#endif
						program->flush();
						return false;
					}
					valid = true;
					break;
				}
				if (c == ')') {
					if (count != (m_constantSize[operandIndex] - 1)) {
#ifdef ANTIKYTHERA_DEBUG
						m_lastErrorString = m_name + "::load() - constant count is greater than number of constants.";
#endif
						program->flush();
						return false;
					}
					valid = true;
					break;
				}
				if (index == maxLength) {
#ifdef ANTIKYTHERA_DEBUG
					m_lastErrorString = m_name + "::load() - constant has too many digits.";
#endif
					program->flush();
					return false;
				}
				if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
					m_lastErrorString = m_name + "::load() - constant contains invalid character: " + String(c);
#endif
					program->flush();
					return false;
				}
				buffer[index++] = c;
			}
			if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
				m_lastErrorString = m_name + "::load() - unexpected end of stream while reading constant.";
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
		case OPERANDTYPE_INT16:
		{
			int16_t t0 = (int16_t)strtol(buffer, NULL, 10);
			setConstant(operandIndex, count, &t0);
		}
			break;

		case OPERANDTYPE_FLOAT:
			return true;

		case OPERANDTYPE_STRING:
			return true;

		case OPERANDTYPE_UINT32:
		{
			uint32_t t3 = (int32_t)strtoul(buffer, NULL, 10);
			setConstant(operandIndex, count, &t3);
		}
			break;
		}
	}

	return true;
}

bool ATKIOperator::initializeConstant(uint8_t operandIndex, uint16_t constantSize) {
	m_constantSize[operandIndex] = constantSize;
	return true;
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKIOperator::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKIOperator::evaluate(unsigned long now) {
#endif
	bool result = true;

	for (uint8_t count = 0; count < m_numOperands; count++) {
		if ((m_operands[count].flags & OPERANDFLAG_LINK)) {
#ifdef ANTIKYTHERA_DEBUG
			result &= Antikythera::operators[m_operands[count].operatorIndex]->evaluate(now, debug);
#else
			result &= Antikythera::operators[m_operands[count].operatorIndex]->evaluate(now);
#endif
		}
	}

	uint16_t max = 0;
	uint16_t min = 65535;
	for (uint8_t i = 0; i < m_numOperands; i++) {
		ATK_OPERAND o = m_operands[i];
		if (o.flags & OPERANDFLAG_LIMIT) {
			uint16_t temp = (o.flags & OPERANDFLAG_LINK) ? Antikythera::operators[o.operatorIndex]->m_resultSize[o.resultIndex] : m_constantSize[i];
			if (temp < min) {
				min = temp;
			}
		} else {
			uint16_t temp = (o.flags & OPERANDFLAG_LINK) ? Antikythera::operators[o.operatorIndex]->m_resultSize[o.resultIndex] : m_constantSize[i];
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

uint16_t ATKIOperator::operandElementIndex(uint8_t operandIndex, uint16_t iteration) {
	uint16_t operandElementSize = Antikythera::operators[m_operands[operandIndex].operatorIndex]->m_resultSize[m_operands[operandIndex].resultIndex];

	if (!(m_operands[operandIndex].flags & OPERANDFLAG_SINGLE)) {
		if (m_numOperations < operandElementSize) {
			return iteration;
		} else {
			if (m_operands[operandIndex].flags & OPERANDFLAG_EXTEND) {
				return operandElementSize - 1;
			} else {
				return iteration % operandElementSize;
			}
		}
	}

	return 0;
}

uint16_t ATKIOperator::constantElementIndex(uint8_t operandIndex, uint16_t iteration) {
	if (!(m_operands[operandIndex].flags & OPERANDFLAG_SINGLE)) {
		if (m_numOperations < m_constantSize[operandIndex]) {
			return iteration;
		} else {
			if (m_operands[operandIndex].flags & OPERANDFLAG_EXTEND) {
				return m_constantSize[operandIndex] - 1;
			} else {
				return iteration % m_constantSize[operandIndex];
			}
		}
	}

	return 0;
}
