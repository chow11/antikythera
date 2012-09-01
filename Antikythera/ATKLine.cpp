/*
 * ATKLine.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include "ATKLine.h"
#include <Antikythera.h>


ATKLine::ATKLine() {
	m_name = "Line";

	m_constX1 = NULL;
	m_constY1 = NULL;
	m_constX2 = NULL;
	m_constY2 = NULL;
	m_constColor = NULL;
	m_constThickness = NULL;
	m_constStyle = NULL;
	m_constDisplay = NULL;
	m_constLayer = NULL;
}

ATKLine::~ATKLine() {
	delete[] m_constX1;
	delete[] m_constY1;
	delete[] m_constX2;
	delete[] m_constY2;
	delete[] m_constColor;
	delete[] m_constThickness;
	delete[] m_constStyle;
	delete[] m_constDisplay;
	delete[] m_constLayer;
}

// <display type><display width><display height><frame width><frame height><num layers>
bool ATKLine::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (m_numOperands != 9) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKLine::load() - incorrect number(" + String(m_numOperands) + ") of operands specified, expected 9.";
#endif
		program->flush();
		return false;
	}

	return true;
}

bool ATKLine::loadProperties(Stream *program) {
	return ATKIOperator::loadProperties(program);
}

bool ATKLine::initializeConstant(uint8_t operandIndex, uint16_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

	switch (operandIndex) {
	case 0:
		m_constX1 = new int16_t[constantSize];
		break;

	case 1:
		m_constY1 = new int16_t[constantSize];
		break;

	case 2:
		m_constX2 = new int16_t[constantSize];
		break;

	case 3:
		m_constY2 = new int16_t[constantSize];
		break;

	case 4:
		m_constColor = new ATKIColor::HSVA[constantSize];
		break;

	case 5:
		m_constThickness = new int16_t[constantSize];
		break;

	case 6:
		m_constStyle = new int16_t[constantSize];
		break;

	case 7:
		m_constDisplay = new int16_t[constantSize];
		break;

	case 8:
		m_constLayer = new int16_t[constantSize];
		break;

	default:
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKLine::initializeConstant() - operandIndex out of range.";
#endif
		return false;
	}

	return true;
}

void ATKLine::setConstant(uint8_t operandIndex, uint16_t element, void *value) {
	switch (operandIndex) {
	case 0:
		m_constX1[element] = *((int16_t *)value);
		break;

	case 1:
		m_constY1[element] = *((int16_t *)value);
		break;

	case 2:
		m_constX2[element] = *((int16_t *)value);
		break;

	case 3:
		m_constY2[element] = *((int16_t *)value);
		break;

	case 4:
		m_constColor[element] = *((ATKIColor::HSVA *)value);
		break;

	case 5:
		m_constThickness[element] = *((int16_t *)value);
		break;

	case 6:
		m_constStyle[element] = *((int16_t *)value);
		break;

	case 7:
		m_constDisplay[element] = *((int16_t *)value);
		break;

	case 8:
		m_constLayer[element] = *((int16_t *)value);
		break;
	}
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKLine::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKLine::evaluate(unsigned long now) {
#endif
	if (m_isEvaluated) {
		return true;
	}
#ifdef ANTIKYTHERA_DEBUG
	debug->println("ATKLine::evaluate()");
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	for (int16_t i; i < m_numOperations; i++) {
		int16_t x1;
		OPERAND_ELEMENT(x1, m_constX1, 0, i)
		int16_t y1;
		OPERAND_ELEMENT(y1, m_constY1, 1, i)
		int16_t x2;
		OPERAND_ELEMENT(x2, m_constX2, 2, i)
		int16_t y2;
		OPERAND_ELEMENT(y2, m_constY2, 3, i)
		ATKIColor::HSVA color;
		OPERAND_ELEMENT(color, m_constColor, 4, i)
		int16_t thickness;
		OPERAND_ELEMENT(thickness, m_constThickness, 5, i)
		int16_t style;
		OPERAND_ELEMENT(style, m_constStyle, 6, i)
		int16_t display;
		OPERAND_ELEMENT(display, m_constDisplay, 7, i)
		int16_t layer;
		OPERAND_ELEMENT(layer, m_constLayer, 8, i)

		Antikythera::displays[display]->line(x1, y1, x2, y2, color, thickness, style, layer);
#ifdef ANTIKYTHERA_DEBUG
		debug->println("ATKLine::evaluate(" + String(now) + ", " + String(i) + ": [(" + String(x1) + ", " + String(y1) + ") - (" + String(x2) + ", " + String(y2) + ")], h" + String((int)color.h) + ", s" + String((int)color.s) + ", v" + String((int)color.v) + ")");
#endif
	}

	m_isEvaluated = true;

	return result;
}

void ATKLine::getResult(uint8_t resultIndex, uint16_t element, void *value) {
}
