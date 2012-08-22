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

#ifdef ANTIKYTHERA_DEBUG
	virtual String name() { return "ATKCircle"; }
#else
	virtual String name() { return ""; }
#endif

	virtual bool load(Stream *program);
	virtual bool evaluate(unsigned long now);
	virtual uint8_t numResults() { return 0; }
	virtual uint8_t resultSize(uint8_t index) { return 0; }

protected:
	virtual void *resultGeneric(uint8_t index) { return NULL; }
	virtual void *constantGeneric(uint8_t index);
	virtual bool initializeConstant(uint8_t operandIndex, uint8_t constantSize);
	virtual bool loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program);

private:
	int8_t *m_constX;
	int8_t *m_constY;
	int8_t *m_constRadius;
	ATKColor::HSVA *m_constColor;
	int8_t *m_constThickness;
	uint8_t *m_constStyle;
	uint8_t *m_constDisplay;
	uint8_t *m_constLayer;
};


#endif

/* ATK_CIRCLE_H_ */
