/*
 * ATK.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <Antikythera.h>
#include <ATKOperatorFactory.h>
#include <stdlib.h>


uint16_t Antikythera::numOperators = 0;
ATKIOperator **Antikythera::operators = NULL;
uint8_t Antikythera::numDisplays = 0;
ATKIDisplay **Antikythera::displays = NULL;
uint8_t Antikythera::numSensors = 0;
ATKISensor **Antikythera::sensors = NULL;


void Antikythera::unload() {
	for (uint8_t count = 0; count < numOperators; count++) {
		delete operators[count];
	}
	delete[] operators;
	numOperators = 0;
}

bool Antikythera::process(long now) {
	if (numOperators == 0) {
		return true;
	}

	resetProcessedFlags();

	return operators[0]->process(now);
};

// operator count(operatortype0()...operatorNtype())
bool Antikythera::load(Stream *program) {
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
		if (index == 5) {
#ifdef ANTIKYTHERA_DEBUG
			this->lastErrorString = "Antikythera::load() - operator count has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			this->lastErrorString = "Antikythera::load() - operator count contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		this->lastErrorString = "Antikythera::load() - unexpected end of stream while reading operator count.";
#endif
		return false;
	}

	numOperators = (uint16_t)strtoul(buffer, NULL, 10);
	operators = new ATKIOperator*[numOperators + 1];
	operators[0] = ATKOperatorFactory::createOperator(ATKOPERATOR_ROOT);

	memset(buffer, 0, 21);
	index = 0;
	valid = false;
	for (int count = 1; count < numOperators + 1; count++) {
		while(program->available()) {
			char c = (char)program->read();
			if (c == '(') {
				valid = true;
				break;
			}
			if (index == 5) {
#ifdef ANTIKYTHERA_DEBUG
				this->lastErrorString = "Antikythera::load() - operator[" + String(count) + "] type has too many digits.";
#endif
				program->flush();
				return false;
			}
			if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
				this->lastErrorString = "Antikythera::load() - operator[" + String(count) + "] contains invalid character: " + String(c);
#endif
				program->flush();
				return false;
			}
			buffer[index++] = c;
		}
		if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
			this->lastErrorString = "Antikythera::load() - unexpected end of stream while reading operator[" + String(count) + "] type.";
#endif
			return false;
		}

		uint16_t operatorType = (uint16_t)strtoul(buffer, NULL, 10);
		operators[count] = ATKOperatorFactory::createOperator(operatorType);
		if (!operators[count]->load(program)) {
			this->lastErrorString = operators[count]->lastErrorString();
			return false;
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
		this->lastErrorString = "Antikythera::load() - expected closing parenthesis, read invalid character: " + String(c);
#endif
		program->flush();
		return false;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		this->lastErrorString = "Antikythera::load() - unexpected end of stream while reading closing parenthesis.";
#endif
		return false;
	}

	return true;
};

void Antikythera::resetProcessedFlags() {
	for (uint8_t count = 0; count < numOperators; count++) {
		operators[count]->resetProcessedFlag();
	}
};
