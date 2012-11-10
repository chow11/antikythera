/*
 * ATKSignal.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_SIGNAL_H_
#define ATK_SIGNAL_H_


#include <Stream.h>
#include <ATKIncludes.h>
#include <ATKIOperator.h>


/*
	Operands:
		- waveform (cardiac, exp rise, exp fall, noise, pulse, ramp, sine, square, triangle)
		- wavelength high bytes (milliseconds)
		- wavelength low bytes (milliseconds)
		- amplitude
		- offset high bytes (phase shift in milliseconds)
		- offset low bytes (phase shift in milliseconds)
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
#define SIGNAL_A_CARDIAC	101
#define SIGNAL_A_EXP_FALL	102
#define SIGNAL_A_EXP_RISE	103
#define SIGNAL_A_NOISE		104
#define SIGNAL_A_PULSE		105		// normalized sinc()
#define SIGNAL_A_RAMP		106
#define SIGNAL_A_SAWTOOTH	107
#define SIGNAL_A_SINE		108
#define SIGNAL_A_SQUARE		109
#define SIGNAL_A_TRIANGLE	110

class ATKSignal : public ATKIOperator {
public:
	ATKSignal();
	~ATKSignal();

public:
	virtual bool load(Stream *program);
protected:
	virtual bool loadProperties(Stream *program);
	virtual void initializeOperands(uint8_t numOperands);
	virtual bool initializeConstant(uint8_t operandIndex, uint16_t constantSize);
	virtual void setConstant(uint8_t operandIndex, uint16_t element, void *value);

public:
	#ifdef ANTIKYTHERA_DEBUG
	virtual bool evaluate(unsigned long now, Stream *debug);
#else
	virtual bool evaluate(unsigned long now);
#endif

	virtual void getResult(uint8_t resultIndex, uint16_t element, void *value);

private:
	float sin(float x);
	int16_t f_cardiac(float phase);

	int16_t *m_result;

	int16_t *m_constWaveform;
	int16_t *m_constWavelengthH;
	int16_t *m_constWavelengthL;
	int16_t *m_constAmplitude;
	int16_t *m_constOffsetH;
	int16_t *m_constOffsetL;
};


#endif

/* ATK_SIGNAL_H_ */
