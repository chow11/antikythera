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

	if (numOperands() != 8) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKCircle::load() - incorrect number(" + String(numOperands()) + ") of operands specified, expected 8.";
#endif
		program->flush();
		return false;
	}

	return true;
}

bool ATKCircle::loadProperties(Stream *program) {
	return ATKIOperator::loadProperties(program);
}

bool ATKCircle::initializeConstant(uint8_t operandIndex, uint8_t constantSize) {
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
		m_constStyle = new uint8_t[constantSize];
		break;

	case 6:
		m_constDisplay = new uint8_t[constantSize];
		break;

	case 7:
		m_constLayer = new uint8_t[constantSize];
		break;

	default:
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKCircle::initializeConstant() - operandIndex out of range.";
#endif
		return false;
	}

	return true;
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKCircle::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKCircle::evaluate(unsigned long now) {
#endif
#ifdef ANTIKYTHERA_DEBUG
	debug->println("ATKCircle::evaluate()");
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	for (uint8_t i; i < numOperations(); i++) {
		OPERAND_ELEMENT(x, OPERANDTYPE_INT16, int16_t, 0, i)
		OPERAND_ELEMENT(y, OPERANDTYPE_INT16, int16_t, 1, i)
		OPERAND_ELEMENT(radius, OPERANDTYPE_INT16, int16_t, 2, i)
		OPERAND_ELEMENT(color, OPERANDTYPE_UINT32, ATKColor::HSVA, 3, i)
		OPERAND_ELEMENT(thickness, OPERANDTYPE_INT16, int16_t, 4, i)
		OPERAND_ELEMENT(style, OPERANDTYPE_UINT8, uint8_t, 5, i)
		OPERAND_ELEMENT(display, OPERANDTYPE_UINT8, uint8_t, 6, i)
		OPERAND_ELEMENT(layer, OPERANDTYPE_UINT8, uint8_t, 7, i)

		Antikythera::displays[display]->circle(x, y, radius, color, thickness, style, layer);
	}

	setEvaluatedFlag();

	return true;
}

void *ATKCircle::constantGeneric(uint8_t index) {
	switch (index) {
	case 0:
		return m_constX;

	case 1:
		return m_constY;

	case 2:
		return m_constRadius;

	case 3:
		return m_constColor;

	case 4:
		return m_constThickness;

	case 5:
		return m_constStyle;

	case 6:
		return m_constDisplay;

	case 7:
		return m_constLayer;
	}

	return NULL;
}
