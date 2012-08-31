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

	virtual void initialize(int16_t displayWidth, int16_t displayHeight, int16_t frameWidth, int16_t frameHeight, int16_t numLayers);

	virtual int16_t displayWidth();
	virtual int16_t displayHeight();
	virtual int16_t frameWidth();
	virtual int16_t frameHeight();
	virtual int16_t numLayers();	// number of additional layers other than the background layer

	virtual void render(int16_t frameX, int16_t frameY);
	virtual ATKColor::RGBA* framebuffer(int16_t layer);

	virtual void circle(int16_t x, int16_t y, int16_t r, ATKColor::HSVA c, int16_t thickness, int16_t style, int16_t layer);
	virtual void circle(int16_t x, int16_t y, int16_t r, ATKColor::RGBA c, int16_t thickness, int16_t style, int16_t layer);
	virtual void line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, ATKColor::HSVA c, int16_t thickness, int16_t style, int16_t layer);
	virtual void line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, ATKColor::RGBA c, int16_t thickness, int16_t style, int16_t layer);
	virtual void point(int16_t x, int16_t y, ATKColor::HSVA c, int16_t style, int16_t layer);
	virtual void point(int16_t x, int16_t y, ATKColor::RGBA c, int16_t style, int16_t layer);

protected:
	ATKIDisplay();
};


#endif

/* ATK_IDISPLAY_H_ */
