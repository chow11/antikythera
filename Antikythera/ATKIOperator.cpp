/*
 * ATKIOperator.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <Antikythera.h>
#include <ATKIOperator.h>


ATKIOperator::ATKIOperator() {
	m_isProcessed = false;
	m_numOperands = 0;
	m_operands = NULL;
	m_numOperations = 0;
}

ATKIOperator::~ATKIOperator() {
	delete[] m_operands;
}

// operand count((operand0 flags, operand0 operator index, operand0 result index)...(operand0 flags, [operandN operator index, operandN result index|operandN constant value]))
// alternate operand specification:
// (operand0 flags, operand operator index, operand constant value)
bool ATKIOperator::load(Stream *program) {
	char buffer[21];
	memset(buffer, 0, 21);
	int index = 0;
	bool valid = false;
	while(program->available()) {
		char c = (char)program->read();
		if (c == '(') {
			valid = true;
			break;
		}
		if (index == 3) {
#ifdef ANTIKYTHERA_DEBUG
			this->lastErrorString = name() + "::load() - operand count has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			this->lastErrorString = name() + "::load() - operand count contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		this->lastErrorString = name() + "::load() - unexpected end of stream while reading operand count.";
#endif
		return false;
	}

	m_numOperands = (uint8_t)strtoul(buffer, NULL, 10);
	m_operands = new ATK_OPERAND[m_numOperands];

	for (int count = 0; count < numOperands(); count++) {
		valid = false;
		while(program->available()) {
			char c = (char)program->read();
			if (c == '(') {
				valid = true;
				break;
			}
	#ifdef ANTIKYTHERA_DEBUG
			this->lastErrorString = name() + "::load() - operand[" + String(count) + "] expected opening parenthesis, read invalid character: " + String(c);
	#endif
			program->flush();
			return false;
		}
		if (!valid) {
	#ifdef ANTIKYTHERA_DEBUG
			this->lastErrorString = name() + "::load() - operand[" + String(count) + "] unexpected end of stream while reading opening parenthesis.";
	#endif
			return false;
		}

		m_operands[count].flags = loadFlags(program);
		if (m_operands[count].flags & OPERANDFLAG_LINK) {
			m_operands[count].operatorIndex = loadOperatorIndex(program);
			m_operands[count].resultIndex = loadResultIndex(program);
		} else {
			m_operands[count].resultIndex = loadConstant(count, m_operands[count].flags, program);
		}
	}

	valid = false;
	while(program->available()) {
		char c = (char)program->read();
		if (c == ')') {
			valid = true;
			break;
		}
#ifdef ANTIKYTHERA_DEBUG
		this->lastErrorString = name() + "::load() - expected closing parenthesis, read invalid character: " + String(c);
#endif
		program->flush();
		return false;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		this->lastErrorString = name() + "::load() - unexpected end of stream while reading closing parenthesis.";
#endif
		return false;
	}

	return true;
}

uint8_t ATKIOperator::loadFlags(Stream *program) {
	char buffer[21];
	memset(buffer, 0, 21);
	int index = 0;
	bool valid = false;
	while(program->available()) {
		char c = (char)program->read();
		if (c == ',') {
			valid = true;
			break;
		}
		if (index == 3) {
#ifdef ANTIKYTHERA_DEBUG
			this->lastErrorString = name() + "::load() - operand flags has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			this->lastErrorString = name() + "::load() - operand flags contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		this->lastErrorString = name() + "::load() - unexpected end of stream while reading flags.";
#endif
		return false;
	}

	return (uint8_t)strtoul(buffer, NULL, 10);
}

uint16_t ATKIOperator::loadOperatorIndex(Stream *program) {
	char buffer[21];
	memset(buffer, 0, 21);
	int index = 0;
	bool valid = false;
	while(program->available()) {
		char c = (char)program->read();
		if (c == ',') {
			valid = true;
			break;
		}
		if (index == 5) {
#ifdef ANTIKYTHERA_DEBUG
			this->lastErrorString = name() + "::load() - operator index has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			this->lastErrorString = name() + "::load() - operator index contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		this->lastErrorString = name() + "::load() - unexpected end of stream while reading operator index.";
#endif
		return false;
	}

	return (uint16_t)strtoul(buffer, NULL, 10);
}

uint8_t ATKIOperator::loadResultIndex(Stream *program) {
	char buffer[21];
	memset(buffer, 0, 21);
	int index = 0;
	bool valid = false;
	while(program->available()) {
		char c = (char)program->read();
		if (c == ')') {
			valid = true;
			break;
		}
		if (index == 3) {
#ifdef ANTIKYTHERA_DEBUG
			this->lastErrorString = name() + "::load() - result index has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			this->lastErrorString = name() + "::load() - result index contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		this->lastErrorString = name() + "::load() - unexpected end of stream while reading result index.";
#endif
		return false;
	}

	return (uint8_t)strtoul(buffer, NULL, 10);
}

uint8_t ATKSignal::loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program) {
	return 0;
}

bool ATKIOperator::process(long now) {
	bool result = true;

	for (uint8_t count = 0; count < numOperands(); count++) {
		if ((operand(count).flags & OPERANDFLAG_LINK)) {
			result &= Antikythera::operators[operand(count).operatorIndex]->process(now);
		}
	}

	uint8_t max = 0;
	uint8_t min = 255;
	for (uint8_t i = 0; i < numOperands(); i++) {
		ATK_OPERAND o = operand(i);
		if (o.flags & OPERANDFLAG_LINK) {
			if (o.flags & OPERANDFLAG_LIMIT) {
				uint8_t temp = Antikythera::operators[o.operatorIndex]->resultSize(o.resultIndex);
				if (temp < min) {
					min = temp;
				}
			} else {
				uint8_t temp = Antikythera::operators[o.operatorIndex]->resultSize(o.resultIndex);
				if (temp > max) {
					min = max;
				}
			}
		}
	}
	m_numOperations = min;
	if (max < min) {
		m_numOperations = max;
	}


	return result;
};

uint8_t ATKIOperator::resultCount() {
	return 0;
}

bool ATKIOperator::isProcessed() {
	return m_isProcessed;
}

void ATKIOperator::resetProcessedFlag() {
	m_isProcessed = false;
}

void ATKIOperator::setProcessedFlag() {
	m_isProcessed = true;
}

void *ATKIOperator::resultGeneric(uint8_t index) {
	return NULL;
}

uint8_t ATKIOperator::operandElementIndex(ATK_OPERAND o, uint8_t iteration) {
	uint8_t result = 0;

	uint8_t operandElementSize = Antikythera::operators[o.operatorIndex]->resultSize(o.resultIndex);
	if (o.flags & OPERANDFLAG_LINK) {
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
	}

	return result;
}
