/*
 * ATKLine.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_LINE_H_
#define ATK_LINE_H_


#include <Stream.h>
#include <ATKIncludes.h>
#include <ATKIOperator.h>
#include <ATKIColor.h>


/*
	Operands:
		- first x location
		- first y location
		- second x location
		- second y location
		- color (HSVA)
		- line width
		- style
		- display
		- layer (0 = background)
	Results:
		- none
*/

class ATKLine : public ATKIOperator {
public:
	ATKLine();
	~ATKLine();

public:
	virtual bool load(Stream *program);
protected:
	virtual bool loadProperties(Stream *program);
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
	int16_t *m_constX1;
	int16_t *m_constY1;
	int16_t *m_constX2;
	int16_t *m_constY2;
	ATKIColor::HSVA *m_constColor;
	int16_t *m_constThickness;
	int16_t *m_constStyle;
	int16_t *m_constDisplay;
	int16_t *m_constLayer;
};


#endif

/* ATK_LINE_H_ */
