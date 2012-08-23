/*
 * ATK.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#define ANTIKYTHERA_DEBUG


#include <stdlib.h>
#include <Antikythera.h>
#include <ATKOperatorFactory.h>


uint16_t Antikythera::numOperators = 0;
ATKIOperator **Antikythera::operators = NULL;
uint8_t Antikythera::numDisplays = 0;
ATKIDisplay **Antikythera::displays = NULL;
uint8_t Antikythera::numSensors = 0;
ATKISensor **Antikythera::sensors = NULL;

#ifdef ANTIKYTHERA_DEBUG
String Antikythera::lastErrorString = "debug";
#else
String Antikythera::lastErrorString = "";
#endif


void Antikythera::unload() {
	for (uint8_t count = 0; count < numOperators; count++) {
		delete operators[count];
	}
	delete[] operators;
	numOperators = 0;
}

#ifdef ANTIKYTHERA_DEBUG
bool Antikythera::evaluate(unsigned long now, Stream *debug) {
#else
bool Antikythera::evaluate(unsigned long now) {
#endif
	if (numOperators == 0) {
		return true;
	}

	resetProcessedFlags();

	bool result = operators[0]->evaluate(now);
#ifdef ANTIKYTHERA_DEBUG
	Antikythera::lastErrorString = operators[1]->lastErrorString();
#endif
	return result;
};

// operator count(operatortype0()...operatorNtype())
bool Antikythera::load(Stream *program) {
	char buffer[21];
	memset(buffer, 0, 21);
	int index = 0;
	bool valid = false;
	while(program->available()) {
		char c = (char)program->read();
#ifdef ANTIKYTHERA_DEBUG
			program->println(c);
#endif
		if (c == '(') {
			valid = true;
			break;
		}
		if (index == 5) {
#ifdef ANTIKYTHERA_DEBUG
			Antikythera::lastErrorString = "Antikythera::load() - operator count has too many digits.";
#endif
			program->flush();
			return false;
		}
		if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
			Antikythera::lastErrorString = "Antikythera::load() - operator count contains invalid character: " + String(c);
#endif
			program->flush();
			return false;
		}
		buffer[index++] = c;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		Antikythera::lastErrorString = "Antikythera::load() - unexpected end of stream while reading operator count.";
#endif
		program->flush();
		return false;
	}

	numOperators = (uint16_t)strtoul(buffer, NULL, 10);
	operators = new ATKIOperator*[numOperators];

	memset(buffer, 0, 21);
	index = 0;
	valid = false;
	for (int count = 0; count < numOperators; count++) {
#ifdef ANTIKYTHERA_DEBUG
		program->print("Processing operator #");
		program->println(count);
#endif
		while(program->available()) {
			char c = (char)program->read();
#ifdef ANTIKYTHERA_DEBUG
			program->println(c);
#endif
			if (c == '(') {
				valid = true;
				break;
			}
			if (index == 5) {
#ifdef ANTIKYTHERA_DEBUG
				Antikythera::lastErrorString = "Antikythera::load() - operator[" + String(count) + "] type has too many digits.";
#endif
				program->flush();
				return false;
			}
			if (!isdigit(c)) {
#ifdef ANTIKYTHERA_DEBUG
				Antikythera::lastErrorString = "Antikythera::load() - operator[" + String(count) + "] contains invalid character: " + String(c);
#endif
				program->flush();
				return false;
			}
			buffer[index++] = c;
		}
		if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
			Antikythera::lastErrorString = "Antikythera::load() - unexpected end of stream while reading operator[" + String(count) + "] type.";
#endif
			program->flush();
			return false;
		}

		uint16_t operatorType = (uint16_t)strtoul(buffer, NULL, 10);
		operators[count] = ATKOperatorFactory::createOperator(operatorType);
		if (!operators[count]->load(program)) {
#ifdef ANTIKYTHERA_DEBUG
			Antikythera::lastErrorString = operators[count]->lastErrorString();
#endif
			program->flush();
			return false;
		}

		valid = false;
		while(program->available()) {
			char c = (char)program->read();
	#ifdef ANTIKYTHERA_DEBUG
				program->println(c);
	#endif
			if (c == ')') {
				valid = true;
				break;
			}
	#ifdef ANTIKYTHERA_DEBUG
			Antikythera::lastErrorString = "Antikythera::load() - expected closing parenthesis, read invalid character: " + String(c);
	#endif
			program->flush();
			return false;
		}
		if (!valid) {
	#ifdef ANTIKYTHERA_DEBUG
			Antikythera::lastErrorString = "Antikythera::load() - unexpected end of stream while reading closing parenthesis.";
	#endif
			program->flush();
			return false;
		}
	}

	valid = false;
	while(program->available()) {
		char c = (char)program->read();
#ifdef ANTIKYTHERA_DEBUG
			program->println(c);
#endif
		if (c == ')') {
			valid = true;
			break;
		}
#ifdef ANTIKYTHERA_DEBUG
		Antikythera::lastErrorString = "Antikythera::load() - expected closing parenthesis, read invalid character: " + String(c);
#endif
		program->flush();
		return false;
	}
	if (!valid) {
#ifdef ANTIKYTHERA_DEBUG
		Antikythera::lastErrorString = "Antikythera::load() - unexpected end of stream while reading closing parenthesis.";
#endif
		program->flush();
		return false;
	}

	program->flush();
	return true;
};

void Antikythera::resetProcessedFlags() {
	for (uint8_t count = 0; count < numOperators; count++) {
		operators[count]->resetEvaluatedFlag();
	}
};
