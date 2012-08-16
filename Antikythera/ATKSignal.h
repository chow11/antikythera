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
		- frequency
		- minimum amplitude
		- maximum amplitude
		- waveform (cardiac, exp rise, exp fall, noise, pulse, ramp, sine, square, triangle)
	Results:
		- evaluation of waveform at the current time
*/

#define SIGNAL_CARDIAC		1
#define SIGNAL_EXP_FALL		2
#define SIGNAL_EXP_RISE		3
#define SIGNAL_NOISE		4
#define SIGNAL_PULSE		5
#define SIGNAL_RAMP			6
#define SIGNAL_SINE			7
#define SIGNAL_SQUARE		8
#define SIGNAL_TRIANGLE		9

class ATKSignal : public ATKIOperator {
public:
	ATKSignal();
	~ATKSignal();

	virtual bool load(Stream *program);

	virtual bool process(long now);

	virtual uint8_t operandCount() { return 4; }
	virtual ATK_OPERAND operand(uint8_t index) { return m_operands[index]; }
	virtual uint8_t resultCount() { return 1; }
	virtual uint8_t resultSize(uint8_t index) { return 1; }

protected:
	virtual void *resultGeneric(uint8_t index) { return m_result; }

private:
	ATK_OPERAND m_operands[];
	uint8_t m_result[1];

	uint8_t m_frequency;
	uint8_t m_minAmplitude;
	uint8_t m_maxAmplitude;
	uint8_t m_waveform;

	long m_periodStart;
};


#endif

/* ATK_Signal_H_ */
