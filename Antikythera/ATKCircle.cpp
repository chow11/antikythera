/*
 * ATKCircle.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <ATKCircle.h>
#include <Antikythera.h>


ATKCircle::ATKCircle() {
	m_name = "Circle";

	m_constX = NULL;
	m_constY = NULL;
	m_constRadius = NULL;
	m_constColor = NULL;
	m_constThickness = NULL;
	m_constStyle = NULL;
	m_constDisplay = NULL;
	m_constLayer = NULL;
}

ATKCircle::~ATKCircle() {
	delete[] m_constX;
	delete[] m_constY;
	delete[] m_constRadius;
	delete[] m_constColor;
	delete[] m_constThickness;
	delete[] m_constStyle;
	delete[] m_constDisplay;
	delete[] m_constLayer;
}

// <display type><display width><display height><frame width><frame height><num layers>
bool ATKCircle::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (m_numOperands != 8) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKCircle::load() - incorrect number(" + String(m_numOperands) + ") of operands specified, expected 8.";
#endif
		program->flush();
		return false;
	}

	return true;
}

bool ATKCircle::loadProperties(Stream *program) {
	return ATKIOperator::loadProperties(program);
}

bool ATKCircle::initializeConstant(uint8_t operandIndex, uint16_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

	switch (operandIndex) {
	case 0:
		m_constX = new int16_t[constantSize];
		break;

	case 1:
		m_constY = new int16_t[constantSize];
		break;

	case 2:
		m_constRadius = new int16_t[constantSize];
		break;

	case 3:
		m_constColor = new ATKColor::HSVA[constantSize];
		break;

	case 4:
		m_constThickness = new int16_t[constantSize];
		break;

	case 5:
		m_constStyle = new int16_t[constantSize];
		break;

	case 6:
		m_constDisplay = new int16_t[constantSize];
		break;

	case 7:
		m_constLayer = new int16_t[constantSize];
		break;

	default:
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKCircle::initializeConstant() - operandIndex out of range.";
#endif
		return false;
	}

	return true;
}

void ATKCircle::setConstant(uint8_t operandIndex, uint16_t element, void *value) {
	switch (operandIndex) {
	case 0:
		m_constX[element] = *((int16_t *)value);
		break;

	case 1:
		m_constY[element] = *((int16_t *)value);
		break;

	case 2:
		m_constRadius[element] = *((int16_t *)value);
		break;

	case 3:
		m_constColor[element] = *((ATKColor::HSVA *)value);
		break;

	case 4:
		m_constThickness[element] = *((int16_t *)value);
		break;

	case 5:
		m_constStyle[element] = *((int16_t *)value);
		break;

	case 6:
		m_constDisplay[element] = *((int16_t *)value);
		break;

	case 7:
		m_constLayer[element] = *((int16_t *)value);
		break;
	}
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKCircle::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKCircle::evaluate(unsigned long now) {
#endif
	if (m_isEvaluated) {
		return true;
	}
#ifdef ANTIKYTHERA_DEBUG
	debug->println("ATKCircle::evaluate()");
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	for (int16_t i; i < m_numOperations; i++) {
		int16_t x;
		OPERAND_ELEMENT(x, m_constX, 0, i)
		int16_t y;
		OPERAND_ELEMENT(y, m_constY, 1, i)
		int16_t radius;
		OPERAND_ELEMENT(radius, m_constRadius, 2, i)
		ATKColor::HSVA color;
		OPERAND_ELEMENT(color, m_constColor, 3, i)
		int16_t thickness;
		OPERAND_ELEMENT(thickness, m_constThickness, 4, i)
		int16_t style;
		OPERAND_ELEMENT(style, m_constStyle, 5, i)
		int16_t display;
		OPERAND_ELEMENT(display, m_constDisplay, 6, i)
		int16_t layer;
		OPERAND_ELEMENT(layer, m_constLayer, 7, i)

		Antikythera::displays[display]->circle(x, y, radius, color, thickness, style, layer);
	}

	m_isEvaluated = true;

	return true;
}

void ATKCircle::getResult(uint8_t resultIndex, uint16_t element, void *value) {
}
