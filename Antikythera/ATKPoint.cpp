/*
 * ATKPoint.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <ATKPoint.h>
#include <Antikythera.h>


ATKPoint::ATKPoint() {
	m_name = "Signal";

	m_constX = NULL;
	m_constY = NULL;
	m_constColor = NULL;
	m_constStyle = NULL;
	m_constDisplay = NULL;
	m_constLayer = NULL;
}

ATKPoint::~ATKPoint() {
	delete[] m_constX;
	delete[] m_constY;
	delete[] m_constColor;
	delete[] m_constStyle;
	delete[] m_constDisplay;
	delete[] m_constLayer;
}

bool ATKPoint::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (m_numOperands != 6) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKPoint::load() - incorrect number(" + String(m_numOperands) + ") of operands specified, expected 6.";
#endif
		program->flush();
		return false;
	}

	return true;
}

bool ATKPoint::loadProperties(Stream *program) {
	return ATKIOperator::loadProperties(program);
}

bool ATKPoint::initializeConstant(uint8_t operandIndex, uint16_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

	switch (operandIndex) {
	case 0:
		m_constX = new int16_t[constantSize];
		break;

	case 1:
		m_constY = new int16_t[constantSize];
		break;

	case 2:
		m_constColor = new ATKColor::HSVA[constantSize];
		break;

	case 3:
		m_constStyle = new int16_t[constantSize];
		break;

	case 4:
		m_constDisplay = new int16_t[constantSize];
		break;

	case 5:
		m_constLayer = new int16_t[constantSize];
		break;

	default:
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKPoint::initializeConstant() - operandIndex out of range.";
#endif
		return false;
	}

	return true;
}

void ATKPoint::setConstant(uint8_t operandIndex, uint16_t element, void *value) {
	switch (operandIndex) {
	case 0:
		m_constX[element] = *((int16_t *)value);
		break;

	case 1:
		m_constY[element] = *((int16_t *)value);
		break;

	case 2:
		m_constColor[element] = *((ATKColor::HSVA *)value);
		break;

	case 3:
		m_constStyle[element] = *((int16_t *)value);
		break;

	case 4:
		m_constDisplay[element] = *((int16_t *)value);
		break;

	case 5:
		m_constLayer[element] = *((int16_t *)value);
		break;
	}
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKPoint::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKPoint::evaluate(unsigned long now) {
#endif
	if (m_isEvaluated) {
		return true;
	}
#ifdef ANTIKYTHERA_DEBUG
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	for (int16_t i; i < m_numOperations; i++) {
		int16_t x;
		OPERAND_ELEMENT(x, m_constX, 0, i)
		int16_t y;
		OPERAND_ELEMENT(y, m_constY, 1, i)
		ATKColor::HSVA color;
		OPERAND_ELEMENT(color, m_constColor, 2, i)
		int16_t style;
		OPERAND_ELEMENT(style, m_constStyle, 3, i)
		int16_t display;
		OPERAND_ELEMENT(display, m_constDisplay, 4, i)
		int16_t layer;
		OPERAND_ELEMENT(layer, m_constLayer, 5, i)

		Antikythera::displays[display]->point(x, y, color, style, layer);
#ifdef ANTIKYTHERA_DEBUG
		ATKColor::RGBA rgb = ATKColor::HSVAtoRGBA(color);
		debug->println("ATKPoint::evaluate(" + String(now) + ", " + String(i) + ": " + String(x) + ", " + String(y) + ", h" + String((int)color.h) + ", s" + String((int)color.s) + ", v" + String((int)color.v) + ", r" + String((int)rgb.r) + ", g" + String((int)rgb.g) + ", b" + String((int)rgb.b) + ")");
#endif
	}

	m_isEvaluated = true;

	return result;
}

void ATKPoint::getResult(uint8_t resultIndex, uint16_t element, void *value) {
}
