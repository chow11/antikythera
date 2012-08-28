/*
 * ATK.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */


#include <stdlib.h>
#include <WProgram.h>
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
#endif


#ifdef ANTIKYTHERA_DEBUG
bool Antikythera::evaluate(unsigned long now, Stream *debug) {
#else
bool Antikythera::evaluate(unsigned long now) {
#endif
	if (numOperators == 0) {
		return true;
	}

	resetProcessedFlags();

#ifdef ANTIKYTHERA_DEBUG
	bool result = operators[0]->evaluate(now, debug);
#else
	bool result = operators[0]->evaluate(now);
#endif
	return result;
};
void Antikythera::unload() {
	for (uint8_t count = 0; count < numOperators; count++) {
		delete operators[count];
	}
	delete[] operators;
	numOperators = 0;
}

// operator count(operatortype0()...operatorNtype())
bool Antikythera::load(Stream *program) {
	unload();

	char buffer[21];
	memset(buffer, 0, 21);
	int index = 0;
	bool valid = false;
	while(readProgram(program)) {
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
#ifdef ANTIKYTHERA_DEBUG
	program->println("operator count: " + numOperators);
#endif

	for (int count = 0; count < numOperators; count++) {
#ifdef ANTIKYTHERA_DEBUG
		program->print("Processing operator #");
		program->println(count);
#endif
		memset(buffer, 0, 21);
		index = 0;
		valid = false;
		while(readProgram(program)) {
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
		if (operators[count] == NULL) {
#ifdef ANTIKYTHERA_DEBUG
			Antikythera::lastErrorString = "Antikythera::load() - unable to create operator type[" + String(operatorType) + "]";
#endif
			program->flush();
			return false;
		}
#ifdef ANTIKYTHERA_DEBUG
		program->println("Created operator: " + operators[count]->name());
#endif
		if (!operators[count]->load(program)) {
#ifdef ANTIKYTHERA_DEBUG
			Antikythera::lastErrorString = operators[count]->m_lastErrorString;
#endif
			program->flush();
			return false;
		}

		valid = false;
		while(readProgram(program)) {
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
	while(readProgram(program)) {
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

bool Antikythera::readProgram(Stream *program) {
	if (program->available() > 0) {
		return true;
	}

	program->println("READY");

	unsigned long timer;
	timer = millis() + ANTIKYTHERA_PROGRAM_TIMEOUT;
	while (program->available() == 0) {
		if (millis() > timer) {
			return false;
		}
	}
	return true;
}

void Antikythera::resetProcessedFlags() {
	for (uint8_t count = 0; count < numOperators; count++) {
		operators[count]->resetEvaluatedFlag();
	}
};
