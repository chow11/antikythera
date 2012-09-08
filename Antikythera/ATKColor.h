/*
 * ATKColor.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_COLOR_H_
#define ATK_COLOR_H_


#include <Stream.h>
#include <ATKIncludes.h>
#include <ATKIColor.h>
#include <ATKIOperator.h>


/*
	Operands:
		- hue
		- saturation
		- value
		- alpha
	Results:
		- ATKColor struct
*/

class ATKColor : public ATKIOperator {
public:
	ATKColor();
	~ATKColor();

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
	ATKIColor::HSVA *m_result;

	int16_t *m_constHue;
	int16_t *m_constSaturation;
	int16_t *m_constValue;
	int16_t *m_constAlpha;
};


#endif

/* ATK_COLOR_H_ */
