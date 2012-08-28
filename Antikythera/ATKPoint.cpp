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

	if (numOperands() != 6) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKPoint::load() - incorrect number(" + String(numOperands()) + ") of operands specified, expected 6.";
#endif
		program->flush();
		return false;
	}

	return true;
}

bool ATKPoint::loadProperties(Stream *program) {
	return ATKIOperator::loadProperties(program);
}

bool ATKPoint::initializeConstant(uint8_t operandIndex, uint8_t constantSize) {
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
		m_constStyle = new uint8_t[constantSize];
		break;

	case 4:
		m_constDisplay = new uint8_t[constantSize];
		break;

	case 5:
		m_constLayer = new uint8_t[constantSize];
		break;

	default:
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKPoint::initializeConstant() - operandIndex out of range.";
#endif
		return false;
	}

	return true;
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKPoint::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKPoint::evaluate(unsigned long now) {
#endif
#ifdef ANTIKYTHERA_DEBUG
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	for (uint8_t i; i < numOperations(); i++) {
		OPERAND_ELEMENT(x, OPERANDTYPE_INT16, int16_t, 0, i)
		OPERAND_ELEMENT(y, OPERANDTYPE_INT16, int16_t, 1, i)
		OPERAND_ELEMENT(color, OPERANDTYPE_UINT32, ATKColor::HSVA, 2, i)
		OPERAND_ELEMENT(style, OPERANDTYPE_UINT8, uint8_t, 3, i)
		OPERAND_ELEMENT(display, OPERANDTYPE_UINT8, uint8_t, 4, i)
		OPERAND_ELEMENT(layer, OPERANDTYPE_UINT8, uint8_t, 5, i)

		Antikythera::displays[display]->point(x, y, color, style, layer);
#ifdef ANTIKYTHERA_DEBUG
		ATKColor::RGBA rgb = ATKColor::HSVAtoRGBA(color);
		debug->println("ATKPoint::evaluate(" + String(now) + ", " + String((int)i) + ": " + String(x) + ", " + String(y) + ", h" + String((int)color.h) + ", s" + String((int)color.s) + ", v" + String((int)color.v) + ", r" + String((int)rgb.r) + ", g" + String((int)rgb.g) + ", b" + String((int)rgb.b) + ")");
#endif
	}

	setEvaluatedFlag();

	return result;
}

void *ATKPoint::constantGeneric(uint8_t index) {
	switch (index) {
	case 0:
		return m_constX;

	case 1:
		return m_constY;

	case 2:
		return m_constColor;

	case 3:
		return m_constStyle;

	case 4:
		return m_constDisplay;

	case 5:
		return m_constLayer;
	}

	return NULL;
}
