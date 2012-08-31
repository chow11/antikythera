/*
 * ATKCircle.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_CIRCLE_H_
#define ATK_CIRCLE_H_


#include <Stream.h>
#include <ATKIncludes.h>
#include <ATKIOperator.h>
#include <ATKColor.h>


/*
	Operands:
		- x location
		- y location
		- radius
		- color (HSVA)
		- thickness
		- style
		- display (number of the display - must match the order provided by the sketch
		- layer (0 = background)
	Results:
		- none
*/

class ATKCircle : public ATKIOperator {
public:
	ATKCircle();
	~ATKCircle();

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
	int16_t *m_constX;
	int16_t *m_constY;
	int16_t *m_constRadius;
	ATKColor::HSVA *m_constColor;
	int16_t *m_constThickness;
	int16_t *m_constStyle;
	int16_t *m_constDisplay;
	int16_t *m_constLayer;
};


#endif

/* ATK_CIRCLE_H_ */
