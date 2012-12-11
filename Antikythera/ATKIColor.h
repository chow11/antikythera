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
	union RGBA {
		struct {
			uint8_t b;
			uint8_t g;
			uint8_t r;
			uint8_t a;
		} color;
		uint32_t bits;

		RGBA() { bits = 0; }
		RGBA(uint32_t color) { bits = color; }
		RGBA(uint8_t cr, uint8_t cg, uint8_t cb, uint8_t ca) { color.r = cr; color.g = cg; color.b = cb; color.a = ca; }

		operator uint32_t() { return bits; }
	};

	// aahhssvv
	union HSVA {
		struct {
			uint8_t v;
			uint8_t s;
			uint8_t h;
			uint8_t a;
		} color;
		uint32_t bits;

		HSVA() { bits = 0; }
		HSVA(uint32_t color) { bits = color; }
		HSVA(uint8_t ch, uint8_t cs, uint8_t cv, uint8_t ca) { color.h = ch; color.s = cs; color.v = cv; color.a = ca; }

		operator uint32_t() { return bits; }
		HSVA brightness(float b) { return HSVA(color.h, color.s, (uint8_t)(color.v * b), color.a); }
	};

	static ATKIColor::RGBA HSVAtoRGBA(ATKIColor::HSVA hsva);

	static const uint8_t DIM_CURVE[];
};


#endif

/* ATK_ICOLOR_H_ */
