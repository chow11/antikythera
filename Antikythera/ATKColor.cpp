/*
 * ATKColor.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <WProgram.h>
#include <ATKColor.h>
#include <Antikythera.h>


ATKColor::ATKColor() {
	m_name = "Color";

	m_result = NULL;
	m_resultSize = new uint16_t[1];
	m_resultSize[0] = 0;

	m_constHue = NULL;
	m_constSaturation = NULL;
	m_constValue = NULL;
}

ATKColor::~ATKColor() {
	delete[] m_result;

	delete[] m_constHue;
	delete[] m_constSaturation;
	delete[] m_constValue;
}

bool ATKColor::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (m_numOperands != 3) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "Color::load() - incorrect number(" + String(m_numOperands) + ") of operands specified, expected 3.";
#endif
		program->flush();
		return false;
	}

	return true;
}

bool ATKColor::loadProperties(Stream *program) {
	return ATKIOperator::loadProperties(program);
}

bool ATKColor::initializeConstant(uint8_t operandIndex, uint16_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

	switch (operandIndex) {
	case 0:
		m_constHue = new int16_t[constantSize];
		break;

	case 1:
		m_constSaturation = new int16_t[constantSize];
		break;

	case 2:
		m_constValue = new int16_t[constantSize];
		break;

	default:
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "Color::initializeConstant() - operandIndex out of range.";
#endif
		return false;
	}

	return true;
}

void ATKColor::setConstant(uint8_t operandIndex, uint16_t element, void *value) {
	switch (operandIndex) {
	case 0:
		m_constHue[element] = *((int16_t *)value);
		break;

	case 1:
		m_constSaturation[element] = *((int16_t *)value);
		break;

	case 2:
		m_constValue[element] = *((int16_t *)value);
		break;
	}
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKColor::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKColor::evaluate(unsigned long now) {
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
	m_result = new ATKIColor::HSVA[m_numOperations];
	m_resultSize[0] = m_numOperations;

	for (int16_t i; i < m_numOperations; i++) {
		int16_t hue;
		OPERAND_ELEMENT(hue, m_constHue, 0, i)
		int16_t saturation;
		OPERAND_ELEMENT(saturation, m_constSaturation, 1, i)
		int16_t value;
		OPERAND_ELEMENT(value, m_constValue, 2, i)

		m_result[i] = ATKIColor::HSVA(hue, saturation, value, 255);
#ifdef ANTIKYTHERA_DEBUG
//		debug->println("Color::evaluate(" + String(now) + ", " + String(i) + ": " + String(hue) + ", " + String(saturation) + ", " + String(value) + ") = " + String((uint32_t)m_result[i]));
		debug->println("Color::evaluate(" + String(now) + ", " + String(i) + ": " + String(hue) + ", " + String(saturation) + ", " + String(value) + ")");
#endif
	}

	m_isEvaluated = true;

	return result;
}

void ATKColor::getResult(uint8_t resultIndex, uint16_t element, void *value) {
	if (resultIndex < m_numResults) {
		if (element < m_resultSize[0]) {
			*((ATKIColor::HSVA *)value) = m_result[element];
		}
	}
}
