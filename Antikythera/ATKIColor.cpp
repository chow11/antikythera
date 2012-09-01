/*
 * ATKColor.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <ATKIColor.h>


/*
	Based on code from kasperkamperman.com and <http://www.codeproject.com/miscctrl/CPicker.asp>

  DIM_CURVE 'lookup table' to compensate for the nonlinearity of human vision.
  Used in the getRGB function on saturation and brightness to make 'dimming' look more natural.
  Exponential function used to create values below :
  x from 0 - 255 : y = round(pow( 2.0, x+64/40.0) - 1)
*/

const uint8_t ATKIColor::DIM_CURVE[] = {
    0,   1,   1,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,
    3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,   4,
    4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   6,   6,   6,
    6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,
    8,   8,   9,   9,   9,   9,   9,   9,   10,  10,  10,  10,  10,  11,  11,  11,
    11,  11,  12,  12,  12,  12,  12,  13,  13,  13,  13,  14,  14,  14,  14,  15,
    15,  15,  16,  16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,
    20,  20,  21,  21,  22,  22,  22,  23,  23,  24,  24,  25,  25,  25,  26,  26,
    27,  27,  28,  28,  29,  29,  30,  30,  31,  32,  32,  33,  33,  34,  35,  35,
    36,  36,  37,  38,  38,  39,  40,  40,  41,  42,  43,  43,  44,  45,  46,  47,
    48,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,
    63,  64,  65,  66,  68,  69,  70,  71,  73,  74,  75,  76,  78,  79,  81,  82,
    83,  85,  86,  88,  90,  91,  93,  94,  96,  98,  99,  101, 103, 105, 107, 109,
    110, 112, 114, 116, 118, 121, 123, 125, 127, 129, 132, 134, 136, 139, 141, 144,
    146, 149, 151, 154, 157, 159, 162, 165, 168, 171, 174, 177, 180, 183, 186, 190,
    193, 196, 200, 203, 207, 211, 214, 218, 222, 226, 230, 234, 238, 242, 248, 255,
};


/*
	http://www.kasperkamperman.com/blog/arduino/arduino-programming-hsb-to-rgb/

	convert hue, saturation and brightness ( HSB/HSV ) to RGB
	The dim_curve is used only on brightness/value and on saturation (inverted).
	This looks the most natural.
	Hue presicion is 1/255ths rather than 1 degree increments
*/
ATKIColor::RGBA ATKIColor::HSVAtoRGBA(ATKIColor::HSVA hsva) {
	ATKIColor::RGBA rgba;
	uint8_t h = hsva.h;
	uint8_t s = 255 - ATKIColor::DIM_CURVE[255 - hsva.s];
	uint8_t v = ATKIColor::DIM_CURVE[hsva.v];

	if (s == 0) { // Acromatic color (gray). Hue doesn't mind.
		rgba.r = v;
		rgba.g = v;
		rgba.b = v;
	} else  {
		float f = (h % 42) / 42.0;
		uint8_t p = (v * (256 - s)) >> 8; // p = v * (1 - s)
		uint8_t q = (v * (256 - (uint8_t)(s * f))) >> 8; // q = v * (1 - s * f)
		uint8_t t = (v * (256 - (uint8_t)(s * (1 - f)))) >> 8; // t = v * (1 - s * (1 - f))

		switch (h / 42) {
		case 0:
			rgba.r = v;
			rgba.g = t;
			rgba.b = p;
		break;

		case 1:
			rgba.r = q;
			rgba.g = v;
			rgba.b = p;
		break;

		case 2:
			rgba.r = p;
			rgba.g = v;
			rgba.b = t;
		break;

		case 3:
			rgba.r = p;
			rgba.g = q;
			rgba.b = v;
		break;

		case 4:
			rgba.r = t;
			rgba.g = p;
			rgba.b = v;
		break;

		case 5:
			rgba.r = v;
			rgba.g = p;
			rgba.b = q;
		break;
		}
	}

	return rgba;
}
