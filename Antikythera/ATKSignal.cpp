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
#include <WProgram.h>

ATKSignal::ATKSignal() {
	m_waveform = 0;
	m_wavelength = 0;
	m_amplitude = 0;
	m_offset = 0;

	m_result = new int16_t[1];
	m_result[0] = 0;
}

// <display type><display width><display height><frame width><frame height><num layers>
bool ATKSignal::load(Stream *program) {
	bool result = true;
//	while(!program->available()) { }
//	int c = program->read();
	return result;
}

bool ATKSignal::process(unsigned long now) {
	delete[] m_result;
	bool result = ATKIOperator::process(now);
	m_result = new int16_t[operationCount()];

	for (uint8_t i; i < operationCount(); i++) {
		ATK_OPERAND o = operand(0);
		m_waveform = *(Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(1);
		m_wavelength = *(Antikythera::operators[o.operatorIndex]->result<uint32_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(2);
		m_amplitude = *(Antikythera::operators[o.operatorIndex]->result<int16_t *>(o.resultIndex)[operandElementIndex(o, i)]);
		o = operand(3);
		m_offset = *(Antikythera::operators[o.operatorIndex]->result<uint32_t *>(o.resultIndex)[operandElementIndex(o, i)]);

		// Because the millisecond counter overflows on an even data boundary, differential millisecond calculations will produce correct results across the millisecond counter overflow.
		double phase = ((now - (unsigned long)m_offset) % m_wavelength) / m_wavelength;

		switch (m_waveform) {
		case SIGNAL_NONE:
			m_result[i] = 0;
			break;

		case SIGNAL_CARDIAC:
			m_result[i] = f_cardiac(phase);
			break;

		case SIGNAL_EXP_FALL:
//			m_result[i] = f_exponentialFall(phase);
			break;

		case SIGNAL_EXP_RISE:
//			m_result[i] = m_amplitude * exp(phase);
			break;

		case SIGNAL_NOISE:
			m_result[i] = (m_amplitude < 0) ? 0 - random(0, abs(m_amplitude)) : random(0, m_amplitude);
			break;

		case SIGNAL_PULSE:
			if (phase == 0) {
				m_result[i] = 1;
			} else {
				m_result[i] = (phase < 0.5) ? m_amplitude * sin(phase * 4 * M_PI) / phase * 4 * M_PI : m_amplitude * sin((phase - 1) * 4 * M_PI) / (phase - 1) * 4 * M_PI;
			}
			break;

		case SIGNAL_RAMP:
			m_result[i] = m_amplitude * phase;
			break;

		case SIGNAL_SAWTOOTH:
			m_result[i] = (phase < 0.5) ? 2 * m_amplitude * phase : 2 * m_amplitude * (phase - 1);
			break;

		case SIGNAL_SINE:
			m_result[i] = m_amplitude * sin(phase * M_TWOPI);
			break;

		case SIGNAL_SQUARE:
			m_result[i] = (phase < 0.5) ? m_amplitude : 0 - m_amplitude;
			break;

		case SIGNAL_TRIANGLE:
			m_result[i] = (phase < 0.5) ? 4 * m_amplitude * abs(phase - 0.25) : -4 * m_amplitude * abs(phase - 0.75);
			break;
		}
	}

	setProcessedFlag();

	return result;
}

/*
 * Algorithm is based in part on the jounal article:
 * Ruha, Antti and Seppo Nissila, "A Real-Time Microprocessor QRS Detector System with a 1-ms Timing Accuracy
 * for the Measurement of Ambulatory HRV", IEEE Trans. Biomed. Eng. Vol. 44, No. 3, 1997
 * The artificial ECG signal they describe is based on the recommendations in the Association for the Advancement
 * of Medical Instrumentation (AAMI) "Standard for Cardiac Monitors, Heart Rate Meters and Alarms (draft), Aug. 1981"
 * and
 * QRSPulse.m - contributed to PhysioNet by Floyd Harriott of Stellate Systems
 * https://docs.google.com/viewer?a=v&q=cache:kKekMtz9_XYJ:ac.inf.elte.hu/Vol_037_2012/247_37.pdf+&hl=en&gl=us&pid=bl&srcid=ADGEEShfZQSNYQQc2SqZdxGJgMVVvCyxWScsfVf1JLBlWh0sEAwnUIv2d8FxMfdQqnj0E0m2HW3D6Feq54oKNaa2Cv2qrzGxHxGrQAetFKHlPIVjU9kydLoW5aQpjZxPzRrWcpvaDckF&sig=AHIEtbSkyG_uJ6drSRJNzAsHXQGcqKKnPw
 */

int16_t ATKSignal::f_cardiac(double phase) {
	return 0;
}
