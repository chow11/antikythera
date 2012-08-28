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
	m_constX1 = NULL;
	m_constY1 = NULL;
	m_constX2 = NULL;
	m_constY2 = NULL;
	m_constColor = NULL;
	m_constThickness = NULL;
	m_constStyle = NULL;
	m_constMode = NULL;
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
	delete[] m_constMode;
	delete[] m_constDisplay;
	delete[] m_constLayer;
}

// <display type><display width><display height><frame width><frame height><num layers>
bool ATKLine::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (numOperands() != 10) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKLine::load() - incorrect number(" + String(numOperands()) + ") of operands specified, expected 10.";
#endif
		program->flush();
		return false;
	}

	return true;
}

bool ATKLine::loadProperties(Stream *program) {
	return ATKIOperator::loadProperties(program);
}

bool ATKLine::initializeConstant(uint8_t operandIndex, uint8_t constantSize) {
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
		m_constColor = new ATKColor::HSVA[constantSize];
		break;

	case 5:
		m_constThickness = new int16_t[constantSize];
		break;

	case 6:
		m_constStyle = new uint8_t[constantSize];
		break;

	case 7:
		m_constMode = new uint8_t[constantSize];
		break;

	case 8:
		m_constDisplay = new uint8_t[constantSize];
		break;

	case 9:
		m_constLayer = new uint8_t[constantSize];
		break;

	default:
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKLine::initializeConstant() - operandIndex out of range.";
#endif
		return false;
	}

	return true;
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKLine::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKLine::evaluate(unsigned long now) {
#endif
	if (isEvaluated()) {
		return true;
	}
#ifdef ANTIKYTHERA_DEBUG
	debug->println("ATKLine::evaluate()");
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	for (uint8_t i; i < numOperations(); i++) {
		OPERAND_ELEMENT(x1, OPERANDTYPE_INT16, int16_t, 0, i)
		OPERAND_ELEMENT(y1, OPERANDTYPE_INT16, int16_t, 1, i)
		OPERAND_ELEMENT(x2, OPERANDTYPE_INT16, int16_t, 2, i)
		OPERAND_ELEMENT(y2, OPERANDTYPE_INT16, int16_t, 3, i)
		OPERAND_ELEMENT(color, OPERANDTYPE_UINT32, ATKColor::HSVA, 4, i)
		OPERAND_ELEMENT(thickness, OPERANDTYPE_INT16, int16_t, 5, i)
		OPERAND_ELEMENT(style, OPERANDTYPE_UINT8, uint8_t, 6, i)
		OPERAND_ELEMENT(mode, OPERANDTYPE_UINT8, uint8_t, 7, i)
		OPERAND_ELEMENT(display, OPERANDTYPE_UINT8, uint8_t, 8, i)
		OPERAND_ELEMENT(layer, OPERANDTYPE_UINT8, uint8_t, 9, i)

		Antikythera::displays[display]->line(x1, y1, x2, y2, color, thickness, style, mode, layer);
#ifdef ANTIKYTHERA_DEBUG
		debug->println("ATKLine::evaluate(" + String(now) + ", " + String((int)i) + ": " + String(x1) + ", " + String(y1) + " - " + String(x2) + ", " + String(y2) + ", h" + String((int)color.h) + ", s" + String((int)color.s) + ", v" + String((int)color.v) + ")");
#endif
	}

	setEvaluatedFlag();

	return result;
}

void *ATKLine::constantGeneric(uint8_t index) {
	switch (index) {
	case 0:
		return m_constX1;

	case 1:
		return m_constY1;

	case 2:
		return m_constX2;

	case 3:
		return m_constY2;

	case 4:
		return m_constColor;

	case 5:
		return m_constThickness;

	case 6:
		return m_constStyle;

	case 7:
		return m_constMode;

	case 8:
		return m_constDisplay;

	case 9:
		return m_constLayer;
	}

	return NULL;
}
