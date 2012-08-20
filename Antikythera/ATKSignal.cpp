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
	m_result = new int16_t[1];
	m_result[0] = 0;

	m_constWaveform = 0;
	m_constWavelength = 0;
	m_constAmplitude = 0;
	m_constOffset = 0;
}

bool ATKSignal::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (numOperands() != 4) {
#ifdef ANTIKYTHERA_DEBUG
		this->lastErrorString = "ATKSignal::load() - incorrect number(" + String(numOperands()) + ") of operands specified, expected 4.";
#endif
		return false;
	}

	return true;
}

void *ATKSignal::constantGeneric(uint8_t index) {
	switch (index) {
	case 0:
		return m_constWaveform;

	case 1:
		return m_constWavelength;

	case 2:
		return m_constAmplitude;

	case 3:
		return m_constOffset;
	}
	return NULL;
}

uint8_t ATKSignal::loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program) {
	return ATKIOperator::loadConstant(operandIndex, flags, program);
}

bool ATKSignal::process(unsigned long now) {
	delete[] m_result;
	bool result = ATKIOperator::evaluate(now);
	m_result = new int16_t[operationCount()];

	for (uint8_t i; i < operationCount(); i++) {

		ATK_OPERAND o = operand(0);
		uint8_t waveform = Antikythera::operators[o.operatorIndex]->result<uint8_t *>(o.resultIndex)[operandElementIndex(o, i)];
		o = operand(1);
		uint32_t wavelength = Antikythera::operators[o.operatorIndex]->result<uint32_t *>(o.resultIndex)[operandElementIndex(o, i)];
		o = operand(2);
		int16_t amplitude = Antikythera::operators[o.operatorIndex]->result<int16_t *>(o.resultIndex)[operandElementIndex(o, i)];
		o = operand(3);
		uint32_t offset = Antikythera::operators[o.operatorIndex]->result<uint32_t *>(o.resultIndex)[operandElementIndex(o, i)];

		// Because the millisecond counter overflows on an even data boundary, differential millisecond calculations will produce correct results across the millisecond counter overflow.
		double phase = ((now - (unsigned long)offset) % wavelength) / wavelength;

		switch (waveform) {
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
			m_result[i] = (amplitude < 0) ? 0 - random(0, abs(amplitude)) : random(0, amplitude);
			break;

		case SIGNAL_PULSE:
			if (phase == 0) {
				m_result[i] = 1;
			} else {
				m_result[i] = (phase < 0.5) ? amplitude * sin(phase * 4 * M_PI) / phase * 4 * M_PI : amplitude * sin((phase - 1) * 4 * M_PI) / (phase - 1) * 4 * M_PI;
			}
			break;

		case SIGNAL_RAMP:
			m_result[i] = amplitude * phase;
			break;

		case SIGNAL_SAWTOOTH:
			m_result[i] = (phase < 0.5) ? 2 * amplitude * phase : 2 * amplitude * (phase - 1);
			break;

		case SIGNAL_SINE:
			m_result[i] = amplitude * sin(phase * M_TWOPI);
			break;

		case SIGNAL_SQUARE:
			m_result[i] = (phase < 0.5) ? amplitude : 0 - amplitude;
			break;

		case SIGNAL_TRIANGLE:
			m_result[i] = (phase < 0.5) ? 4 * amplitude * abs(phase - 0.25) : -4 * amplitude * abs(phase - 0.75);
			break;
		}
	}

	setEvaluatedFlag();

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
