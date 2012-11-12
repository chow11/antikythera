/*
 * ATKSignal.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <WProgram.h>
#include <ATKSignal.h>
#include <Antikythera.h>


ATKSignal::ATKSignal() {
	m_name = "Signal";

	m_result = NULL;
	m_resultSize = new uint16_t[1];
	m_resultSize[0] = 0;

	m_constWaveform = NULL;
	m_constWavelengthH = NULL;
	m_constWavelengthL = NULL;
	m_constAmplitude = NULL;
	m_constOffsetH = NULL;
	m_constOffsetL = NULL;
}

ATKSignal::~ATKSignal() {
	delete[] m_result;

	delete[] m_constWaveform;
	delete[] m_constWavelengthH;
	delete[] m_constWavelengthL;
	delete[] m_constAmplitude;
	delete[] m_constOffsetH;
	delete[] m_constOffsetL;
}

bool ATKSignal::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (m_numOperands != 6) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "Signal::load() - incorrect number(" + String(m_numOperands) + ") of operands specified, expected 6.";
#endif
		program->flush();
		return false;
	}

	return true;
}

bool ATKSignal::loadProperties(Stream *program) {
	return ATKIOperator::loadProperties(program);
}

void ATKSignal::initializeOperands(uint8_t numOperands) {
	ATKIOperator::initializeOperands(numOperands);
}

bool ATKSignal::initializeConstant(uint8_t operandIndex, uint16_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

	switch (operandIndex) {
	case 0:
		m_constWaveform = new int16_t[constantSize];
		break;

	case 1:
		m_constWavelengthH = new int16_t[constantSize];
		break;

	case 2:
		m_constWavelengthL = new int16_t[constantSize];
		break;

	case 3:
		m_constAmplitude = new int16_t[constantSize];
		break;

	case 4:
		m_constOffsetH = new int16_t[constantSize];
		break;

	case 5:
		m_constOffsetL = new int16_t[constantSize];
		break;

	default:
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "Signal::initializeConstant() - operandIndex out of range.";
#endif
		return false;
	}

	return true;
}

void ATKSignal::setConstant(uint8_t operandIndex, uint16_t element, void *value) {
	switch (operandIndex) {
	case 0:
		m_constWaveform[element] = *((int16_t *)value);
		break;

	case 1:
		m_constWavelengthH[element] = *((int16_t *)value);
		break;

	case 2:
		m_constWavelengthL[element] = *((int16_t *)value);
		break;

	case 3:
		m_constAmplitude[element] = *((int16_t *)value);
		break;

	case 4:
		m_constOffsetH[element] = *((int16_t *)value);
		break;

	case 5:
		m_constOffsetL[element] = *((int16_t *)value);
		break;
	}
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKSignal::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKSignal::evaluate(unsigned long now) {
#endif
	if (m_isEvaluated) {
		return true;
	}

#ifdef ANTIKYTHERA_DEBUG
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	delete[] m_result;
	m_numResults = 1;
	m_result = new int16_t[m_numOperations];
	m_resultSize[0] = m_numOperations;

	for (int16_t i; i < m_numOperations; i++) {
		int16_t waveform;
		OPERAND_ELEMENT(waveform, m_constWaveform, 0, i)
		int16_t wavelengthH;
		OPERAND_ELEMENT(wavelengthH, m_constWavelengthH, 1, i)
		int16_t wavelengthL;
		OPERAND_ELEMENT(wavelengthL, m_constWavelengthL, 2, i)
		int16_t amplitude;
		OPERAND_ELEMENT(amplitude, m_constAmplitude, 3, i)
		int16_t offsetH;
		OPERAND_ELEMENT(offsetH, m_constOffsetH, 4, i)
		int16_t offsetL;
		OPERAND_ELEMENT(offsetL, m_constOffsetL, 5, i)

		uint32_t wavelength = ((uint32_t)wavelengthH << 16) + wavelengthL;
		uint32_t offset = ((uint32_t)offsetH << 16) + offsetL;
		// Because the millisecond counter overflows on an even data boundary, differential millisecond calculations will produce correct results across the millisecond counter overflow.
		float phase = ((now - offset) % wavelength) / (float)wavelength;

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
				m_result[i] = (phase < 0.5) ? amplitude * sin(phase * 4 * M_PI) / (phase * 4 * M_PI) : amplitude * sin((phase - 1) * 4 * M_PI) / ((phase - 1) * 4 * M_PI);
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

		case SIGNAL_A_CARDIAC:
			m_result[i] = f_cardiac(phase);
			break;

		case SIGNAL_A_EXP_FALL:
//			m_result[i] = f_exponentialFall(phase);
			break;

		case SIGNAL_A_EXP_RISE:
//			m_result[i] = m_amplitude * exp(phase);
			break;

		case SIGNAL_A_NOISE:
			m_result[i] = (amplitude < 0) ? 0 - random(0, abs(amplitude)) : random(0, amplitude);
			break;

		case SIGNAL_A_PULSE:
			if (phase == 0) {
				m_result[i] = 1;
			} else {
				m_result[i] = (phase < 0.5) ? amplitude * sin(phase * 4 * M_PI) / (phase * 4 * M_PI) : amplitude * sin((phase - 1) * 4 * M_PI) / ((phase - 1) * 4 * M_PI);
				m_result[i] = abs(m_result[i]);
			}
			break;

		case SIGNAL_A_RAMP:
			m_result[i] = amplitude * phase;
			break;

		case SIGNAL_A_SAWTOOTH:
			m_result[i] = (phase < 0.5) ? 2 * amplitude * phase : 2 * amplitude * (phase - 1);
			break;

		case SIGNAL_A_SINE:
			m_result[i] = amplitude * sin(phase * M_TWOPI);
			break;

		case SIGNAL_A_SQUARE:
			m_result[i] = (phase < 0.5) ? amplitude : 0 - amplitude;
			break;

		case SIGNAL_A_TRIANGLE:
			m_result[i] = (phase < 0.5) ? 2 * amplitude * phase : 2 * amplitude * (1 - phase);
			break;
		}
#ifdef ANTIKYTHERA_DEBUG
		debug->println("Signal::evaluate(" + String(now) + ", " + String(i) + ": " + String(waveform) + ", " + String(wavelength) + ", " + String(amplitude) + ", " + String(offset) + ") = " + String(m_result[i]));
#endif
	}

	m_isEvaluated = true;

	return result;
}

void ATKSignal::getResult(uint8_t resultIndex, uint16_t element, void *value) {
	if (resultIndex < m_numResults) {
		if (element < m_resultSize[0]) {
			*((int16_t *)value) = m_result[element];
		}
	}
}

float ATKSignal::sin(float x) {
	x -= M_PI;
	float value = x;
	value -= (x*x*x)/6.0;
	value += (x*x*x*x*x)/120.0;
	value *= -1.0;

	return value;
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

int16_t ATKSignal::f_cardiac(float phase) {
	return 0;
}
