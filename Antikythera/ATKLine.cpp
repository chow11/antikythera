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

bool ATKLine::evaluate(unsigned long now) {
	bool result = ATKIOperator::evaluate(now);

	for (uint8_t i; i < numOperations(); i++) {
		ATK_OPERAND o = operand(0);
		int8_t x1 = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<int8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<int8_t>(i)[operandElementIndex(o, i)];
		o = operand(1);
		int8_t y1 = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<int8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<int8_t>(i)[operandElementIndex(o, i)];
		o = operand(2);
		int8_t x2 =(o.flags & OPERANDFLAG_LIMIT) ?  Antikythera::operators[o.operatorIndex]->result<int8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<int8_t>(i)[operandElementIndex(o, i)];
		o = operand(3);
		int8_t y2 = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<int8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<int8_t>(i)[operandElementIndex(o, i)];
		o = operand(4);
		ATKColor::HSVA color = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<ATKColor::HSVA>(o.resultIndex)[operandElementIndex(o, i)] : constant<ATKColor::HSVA>(i)[operandElementIndex(o, i)];
		o = operand(5);
		int8_t thickness = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<int8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<int8_t>(i)[operandElementIndex(o, i)];
		o = operand(6);
		uint8_t style = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<uint8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<uint8_t>(i)[operandElementIndex(o, i)];
		o = operand(7);
		uint8_t mode = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<uint8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<uint8_t>(i)[operandElementIndex(o, i)];
		o = operand(8);
		uint8_t display = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<uint8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<uint8_t>(i)[operandElementIndex(o, i)];
		o = operand(9);
		uint8_t layer = (o.flags & OPERANDFLAG_LIMIT) ? Antikythera::operators[o.operatorIndex]->result<uint8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<uint8_t>(i)[operandElementIndex(o, i)];

		Antikythera::displays[display]->line(x1, y1, x2, y2, color, thickness, style, mode, layer);
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

bool ATKLine::initializeConstant(uint8_t operandIndex, uint8_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

	switch (operandIndex) {
	case 0:
		m_constX1 = new int8_t[constantSize];
		break;

	case 1:
		m_constY1 = new int8_t[constantSize];
		break;

	case 2:
		m_constX2 = new int8_t[constantSize];
		break;

	case 3:
		m_constY2 = new int8_t[constantSize];
		break;

	case 4:
		m_constColor = new ATKColor::HSVA[constantSize];
		break;

	case 5:
		m_constThickness = new int8_t[constantSize];
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

bool ATKLine::loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program) {
	return ATKIOperator::loadConstant(operandIndex, flags, program);
}
