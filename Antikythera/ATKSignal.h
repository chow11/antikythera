/*
 * ATKSignal.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_Signal_H_
#define ATK_Signal_H_


#include <ATKIOperator.h>


/*
	Operands:
		- waveform (cardiac, exp rise, exp fall, noise, pulse, ramp, sine, square, triangle)
		- wavelength (milliseconds)
		- amplitude
		- offset (phase shift in milliseconds)
	Results:
		- evaluation of waveform at the current time
*/

#define SIGNAL_NONE			0
#define SIGNAL_CARDIAC		1
#define SIGNAL_EXP_FALL		2
#define SIGNAL_EXP_RISE		3
#define SIGNAL_NOISE		4
#define SIGNAL_PULSE		5		// normalized sinc()
#define SIGNAL_RAMP			6
#define SIGNAL_SAWTOOTH		7
#define SIGNAL_SINE			8
#define SIGNAL_SQUARE		9
#define SIGNAL_TRIANGLE		10

class ATKSignal : public ATKIOperator {
public:
	ATKSignal();
	~ATKSignal();

	virtual bool load(Stream *program);

	virtual bool evaluate(unsigned long now);

	virtual uint8_t resultCount() { return 1; }
	virtual uint8_t resultSize(uint8_t index) { return 1; }

protected:
	virtual void *resultGeneric(uint8_t index) { return m_result; }
	virtual void *constantGeneric(uint8_t index);
	virtual bool loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program);

#ifdef ANTIKYTHERA_DEBUG
	virtual String name() { return "ATKSignal"; }
#else
	virtual String name() { return ""; }
#endif

private:
	int16_t f_cardiac(double phase);

	int16_t *m_result;

	uint8_t *m_constWaveform;
	uint32_t *m_constWavelength;
	int16_t *m_constAmplitude;
	uint32_t *m_constOffset;
};


#endif

/* ATK_Signal_H_ */
