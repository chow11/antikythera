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

	virtual String name() { return "ATKPoint"; }

	// loading
public:
	virtual bool load(Stream *program);
protected:
	virtual bool loadProperties(Stream *program);
	virtual bool initializeConstant(uint8_t operandIndex, uint8_t constantSize);

	// evaluation
public:
	#ifdef ANTIKYTHERA_DEBUG
	virtual bool evaluate(unsigned long now, Stream *debug);
#else
	virtual bool evaluate(unsigned long now);
#endif

	// operands

	// constants
protected:
	virtual void *constantGeneric(uint8_t index);

	// operations

	// results
public:
	virtual uint8_t numResults() { return 0; }
	virtual uint8_t resultSize(uint8_t index) { return 0; }
	virtual void result(uint8_t index, uint8_t element, void *value, uint8_t valueType) { }

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
