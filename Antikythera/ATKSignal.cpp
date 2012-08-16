/*
 * ATKSignal.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <ATKSignal.h>
#include <Antikythera.h>
#include <math.h>


ATKSignal::ATKSignal() {
	m_frequency = 0;
	m_minAmplitude = 0;
	m_maxAmplitude = 0;
	m_waveform = 0;

	m_periodStart = 0;
}

// <display type><display width><display height><frame width><frame height><num layers>
bool ATKSignal::load(Stream *program) {
	bool result = true;
//	while(!program->available()) { }
//	int c = program->read();
	return result;
}

bool ATKSignal::process(long now) {
	bool result = ATKIOperator::process(now);

	for (uint8_t i; i < operationCount(); i++) {
		ATK_OPERAND o = operand(0);
		m_frequency = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(1);
		m_minAmplitude = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(2);
		m_maxAmplitude = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(3);
		m_waveform = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);

		long periods = (now - m_periodStart) / m_frequency;
		m_periodStart += periods * m_frequency;
		long phase = now - m_periodStart;

		switch (m_waveform) {
		case SIGNAL_CARDIAC:
			m_result[0] = f_cardiac(phase);
			break;

		case SIGNAL_EXP_FALL:
			m_result[0] = f_exponentialFall(phase);
			break;

		case SIGNAL_EXP_RISE:
			m_result[0] = f_exponentialRise(phase);
			break;

		case SIGNAL_NOISE:
			m_result[0] = f_noise(phase);
			break;

		case SIGNAL_PULSE:
			m_result[0] = f_pulse(phase);
			break;

		case SIGNAL_RAMP:
			m_result[0] = f_ramp(phase);
			break;

		case SIGNAL_SINE:
			m_result[0] = f_sine(phase);
			break;

		case SIGNAL_SQUARE:
			m_result[0] = f_square(phase);
			break;

		case SIGNAL_TRIANGLE:
			m_result[0] = f_triangle(phase);
			break;

		}
	}

	setProcessedFlag();

	return result;
}

uint8_t f_cardiac(long phase) {
	return 0;
}

uint8_t f_exponentialFall(long phase) {
	return 0;
}

uint8_t f_exponentialRise(long phase) {
	return 0;
}

uint8_t f_noise(long phase) {
	return 0;
}

uint8_t f_pulse(long phase) {
	return 0;
}

uint8_t f_ramp(long phase) {
	return 0;
}

uint8_t f_sine(long phase) {
	return 0;
}

uint8_t f_square(long phase) {
	return 0;
}

uint8_t f_triangle(long phase) {
	return 0;
}
