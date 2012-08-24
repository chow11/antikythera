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
#include <ATKColor.h>


/*
	Operands:
		- first x location
		- first y location
		- second x location
		- second y location
		- color (HSVA)
		- line width
		- style
		- mode
		- display
		- layer (0 = background)
	Results:
		- none
*/

class ATKLine : public ATKIOperator {
public:
	ATKLine();
	~ATKLine();

#ifdef ANTIKYTHERA_DEBUG
	virtual String name() { return "ATKLine"; }
#else
	virtual String name() { return ""; }
#endif

	virtual bool load(Stream *program);
#ifdef ANTIKYTHERA_DEBUG
	static bool evaluate(unsigned long now, Stream *debug);
#else
	static bool evaluate(unsigned long now);
#endif
	virtual uint8_t numResults() { return 0; }
	virtual uint8_t resultSize(uint8_t index) { return 0; }

protected:
	virtual void *resultGeneric(uint8_t index) { return NULL; }
	virtual void *constantGeneric(uint8_t index);
	virtual bool initializeConstant(uint8_t operandIndex, uint8_t constantSize);
	virtual bool loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program);

private:
	int8_t *m_constX1;
	int8_t *m_constY1;
	int8_t *m_constX2;
	int8_t *m_constY2;
	ATKColor::HSVA *m_constColor;
	int8_t *m_constThickness;
	uint8_t *m_constStyle;
	uint8_t *m_constMode;
	uint8_t *m_constDisplay;
	uint8_t *m_constLayer;
};


#endif

/* ATK_LINE_H_ */
