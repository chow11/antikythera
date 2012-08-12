/*
 * ATKLine.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include "ATKLine.h"
#include "Antikythera.h"

ATKLine::ATKLine() {
	m_x1 = 0;
	m_y1 = 0;
	m_x2 = 0;
	m_y2 = 0;
	m_width = 0;
	m_style = 0;
	m_mode = 0;
	m_displayNumber = 0;
	m_layer = 0;
}

// <display type><display width><display height><frame width><frame height><num layers>
bool ATKLine::load(Stream *program) {
	bool result = true;
//	while(!program->available()) { }
//	int c = program->read();
	return result;
}

bool ATKLine::process(long now) {
	bool result = ATKIOperator::process(now);

	for (uint8_t i; i < operationCount(); i++) {
		ATK_OPERAND o = operand(0);
		m_x1 = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(1);
		m_y1 = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(2);
		m_x2 = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(3);
		m_y2 = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(4);
		m_color = *(Antikythera::operators[o.operatorIndex]->result<HSVA *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(5);
		m_width = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(6);
		m_style = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(7);
		m_mode = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(8);
		m_displayNumber = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(9);
		m_layer = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);

		Antikythera::displays[0]->line(m_x1, m_y1, m_x2, m_y2, m_color, m_width, m_style, m_mode, m_layer);
	}

	setProcessedFlag();

	return result;
}
