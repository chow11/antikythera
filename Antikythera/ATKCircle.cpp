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

bool ATKCircle::evaluate(unsigned long now) {
	bool result = ATKIOperator::evaluate(now);

	for (uint8_t i; i < numOperations(); i++) {
		ATK_OPERAND o = operand(0);
		int8_t x = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<int8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<int8_t>(i)[operandElementIndex(o, i)];
		o = operand(1);
		int8_t y = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<int8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<int8_t>(i)[operandElementIndex(o, i)];
		o = operand(2);
		int8_t radius = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<int8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<int8_t>(i)[operandElementIndex(o, i)];
		o = operand(3);
		ATKColor::HSVA color = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<ATKColor::HSVA>(o.resultIndex)[operandElementIndex(o, i)] : constant<ATKColor::HSVA>(i)[operandElementIndex(o, i)];
		o = operand(4);
		int8_t thickness = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<int8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<int8_t>(i)[operandElementIndex(o, i)];
		o = operand(5);
		uint8_t style = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<uint8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<uint8_t>(i)[operandElementIndex(o, i)];
		o = operand(6);
		uint8_t display = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<uint8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<uint8_t>(i)[operandElementIndex(o, i)];
		o = operand(7);
		uint8_t layer = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<uint8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<uint8_t>(i)[operandElementIndex(o, i)];

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

bool ATKCircle::initializeConstant(uint8_t operandIndex, uint8_t constantSize) {
	switch (operandIndex) {
	case 0:
		m_constX = new int8_t[constantSize];
		break;

	case 1:
		m_constY = new int8_t[constantSize];
		break;

	case 2:
		m_constRadius = new int8_t[constantSize];
		break;

	case 3:
		m_constColor = new ATKColor::HSVA[constantSize];
		break;

	case 4:
		m_constThickness = new int8_t[constantSize];
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
		this->lastErrorString = "ATKCircle::initializeConstant() - operandIndex out of range.";
#endif
		return false;
	}

	return true;
}

bool ATKCircle::loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program) {
	return ATKIOperator::loadConstant(operandIndex, flags, program);
}
