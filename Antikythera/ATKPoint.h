/*
 * ATKPoint.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_POINT_H_
#define ATK_POINT_H_


#include <Stream.h>
#include <ATKIncludes.h>
#include <ATKIOperator.h>
#include <ATKColor.h>


/*
	Operands:
		- x location
		- y location
		- color (HSVA)
		- thickness
		- style
		- display
		- layer (0 = background)
	Results:
		- layer (0 = background)
*/

class ATKPoint : public ATKIOperator {
public:
	ATKPoint();
	~ATKPoint();

#ifdef ANTIKYTHERA_DEBUG
	virtual String name() { return "ATKPoint"; }
#else
	virtual String name() { return ""; }
#endif

	virtual bool load(Stream *program);
#ifdef ANTIKYTHERA_DEBUG
	virtual bool evaluate(unsigned long now, Stream *debug);
#else
	virtual bool evaluate(unsigned long now);
#endif
	virtual uint8_t numResults() { return 0; }
	virtual uint8_t resultSize(uint8_t index) { return 0; }

protected:
	virtual void *resultGeneric(uint8_t index) { return NULL; }
	virtual void *constantGeneric(uint8_t index);
	virtual bool initializeConstant(uint8_t operandIndex, uint8_t constantSize);
	virtual bool loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program);

private:
	int16_t *m_constX;
	int16_t *m_constY;
	ATKColor::HSVA *m_constColor;
	uint8_t *m_constStyle;
	uint8_t *m_constDisplay;
	uint8_t *m_constLayer;
};


#endif

/* ATK_POINT_H_ */
