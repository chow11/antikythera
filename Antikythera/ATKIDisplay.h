/*
 * ATKIDisplay.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_IDISPLAY_H_
#define ATK_IDISPLAY_H_


#include <ATKIncludes.h>
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
	virtual ATKColor::RGBA* framebuffer(uint8_t layer);

	virtual void circle(int16_t x, int16_t y, int16_t r, ATKColor::HSVA c, int16_t thickness, uint8_t style, uint8_t layer);
	virtual void circle(int16_t x, int16_t y, int16_t r, ATKColor::RGBA c, int16_t thickness, uint8_t style, uint8_t layer);
	virtual void line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, ATKColor::HSVA c, int16_t thickness, uint8_t style, uint8_t mode, uint8_t layer);
	virtual void line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, ATKColor::RGBA c, int16_t thickness, uint8_t style, uint8_t mode, uint8_t layer);
	virtual void point(int16_t x, int16_t y, ATKColor::HSVA c, uint8_t style, uint8_t layer);
	virtual void point(int16_t x, int16_t y, ATKColor::RGBA c, uint8_t style, uint8_t layer);

protected:
	ATKIDisplay();
};


#endif

/* ATK_IDISPLAY_H_ */
