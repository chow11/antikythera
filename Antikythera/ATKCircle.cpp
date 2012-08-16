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
	m_x = 0;
	m_y = 0;
	m_radius = 0;
	m_displayNumber = 0;
	m_layer = 0;
}

// <display type><display width><display height><frame width><frame height><num layers>
bool ATKCircle::load(Stream *program) {
	bool result = true;
//	while(!program->available()) { }
//	int c = program->read();
	return result;
}

bool ATKCircle::process(long now) {
	bool result = ATKIOperator::process(now);

	for (uint8_t i; i < operationCount(); i++) {
		ATK_OPERAND o = operand(0);
		
		m_x = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(1);
		m_y = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(2);
		m_radius = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(3);
		m_color = *(Antikythera::operators[o.operatorIndex]->result<HSVA *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(4);
		m_displayNumber = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(5);
		m_layer = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);

		Antikythera::displays[0]->circle(m_x, m_y, m_radius, m_color, m_layer);
		m_result[0] = m_layer;
	}

	setProcessedFlag();

	return result;
}
