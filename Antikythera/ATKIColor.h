/*
 * ATKColor.h
 *
 *  Created on: Jul 20, 2011
 *      Author: chow
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */
 
#ifndef ATK_ICOLOR_H_
#define ATK_ICOLOR_H_


#include <ATKIncludes.h>


class ATKIColor {
public:
	// aarrggbb
	struct RGBA {
		uint8_t b;
		uint8_t g;
		uint8_t r;
		uint8_t a;

		RGBA() { b = 0; g = 0; r = 0; a = 0; }
		RGBA(uint8_t cr, uint8_t cg, uint8_t cb, uint8_t ca) { r = cr; g = cg; b = cb; a = ca; }
	};

	// aahhssvv
	struct HSVA {
		uint8_t v;
		uint8_t s;
		uint8_t h;
		uint8_t a;

		HSVA() { v = 0; s = 0; h = 0; a = 0; }
		HSVA(uint8_t ch, uint8_t cs, uint8_t cv, uint8_t ca) { h = ch; s = cs; v = cv; a = ca; }

		HSVA brightness(float b) { return HSVA(h, s, (uint8_t)(v * b), a); }
	};

	static ATKIColor::RGBA HSVAtoRGBA(ATKIColor::HSVA hsva);

	static const uint8_t DIM_CURVE[];
};


#endif

/* ATK_ICOLOR_H_ */
