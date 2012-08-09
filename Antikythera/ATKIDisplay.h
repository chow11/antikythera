/*
 * ATKIDisplay.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_IDisplay_H_
#define ATK_IDisplay_H_

#include <ATKColor.h>

class ATKIDisplay {
public:
	virtual ~ATKIDisplay();

	virtual void initialize(uint16_t displayWidth, uint16_t displayHeight, uint16_t frameWidth, uint16_t frameHeight, uint8_t numLayers);

	virtual uint16_t displayWidth();
	virtual uint16_t displayHeight();
	virtual uint16_t frameWidth();
	virtual uint16_t frameHeight();
	virtual uint8_t numLayers();	// number of additional layers other than the background layer

	virtual void render(uint16_t frameX, uint16_t frameY);
	virtual RGBA* framebuffer(uint8_t layer);

	virtual void circle(int8_t x, int8_t y, uint8_t r, HSVA c, uint8_t layer);
	virtual void circle(int8_t x, int8_t y, uint8_t r, RGBA c, uint8_t layer);
	virtual void line(int8_t x1, int8_t y1, int8_t x2, int8_t y2, HSVA c, uint8_t width, uint8_t style, uint8_t mode, uint8_t layer);
	virtual void line(int8_t x1, int8_t y1, int8_t x2, int8_t y2, RGBA c, uint8_t width, uint8_t style, uint8_t mode, uint8_t layer);
	virtual void point(int8_t x, int8_t y, HSVA c, uint8_t layer);
	virtual void point(int8_t x, int8_t y, RGBA c, uint8_t layer);

protected:
	ATKIDisplay() { }
};

#endif

/* ATK_IDisplay_H_ */
