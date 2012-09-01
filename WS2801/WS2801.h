/*
 * WS2801.h
 *
 *  Created on: Jul 20, 2011
 *      Author: chow
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef WS2801_H_
#define WS2801_H_


#include <p32xxxx.h>
#include <p32_defs.h>
#include <ATKIncludes.h>
#include <ATKIDisplay.h>
#include <ATKIColor.h>


#define	DSPI_MODE0	((0 << _SPICON_CKP)|(1 << _SPICON_CKE))		// CKP = 0 CKE = 1
#define	DSPI_MODE1	((0 << _SPICON_CKP)|(0 << _SPICON_CKE))		// CKP = 0 CKE = 0
#define	DSPI_MODE2	((1 << _SPICON_CKP)|(1 << _SPICON_CKE))		// CKP = 1 CKE = 1
#define	DSPI_MODE3	((1 << _SPICON_CKP)|(0 << _SPICON_CKE))		// CKP = 1 CKE = 0

#define	_DSPI_SPD_DEFAULT	1000000L

#define SPRITE_WIDTH			5
#define SPRITE_HEIGHT			5


class WS2801 : public ATKIDisplay {
public:
	WS2801(p32_spi *pspi, uint8_t pinSS);
	virtual ~WS2801();

	virtual void initialize(int16_t displayWidth, int16_t displayHeight, int16_t frameWidth, int16_t frameHeight, int16_t numLayers);

	virtual int16_t displayWidth() { return m_displayWidth; }
	virtual int16_t displayHeight() { return m_displayHeight; }
	virtual int16_t frameWidth() { return m_frameWidth; }
	virtual int16_t frameHeight() { return m_frameHeight; }
	virtual int16_t numLayers() { return m_numLayers; }

	virtual void render(int16_t frameX, int16_t frameY);
	virtual ATKIColor::RGBA* framebuffer(int16_t layer);

	virtual void circle(int16_t x, int16_t y, int16_t r, ATKIColor::HSVA c, int16_t thickness, int16_t style, int16_t layer);
	virtual void circle(int16_t x, int16_t y, int16_t r, ATKIColor::RGBA c, int16_t thickness, int16_t style, int16_t layer);
	virtual void line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, ATKIColor::HSVA c, int16_t thickness, int16_t style, int16_t layer);
	virtual void line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, ATKIColor::RGBA c, int16_t thickness, int16_t style, int16_t layer);
	virtual void point(int16_t x, int16_t y, ATKIColor::HSVA c, int16_t style, int16_t layer);
	virtual void point(int16_t x, int16_t y, ATKIColor::RGBA c, int16_t style, int16_t layer);

	void begin();
	void end();
	void setSpeed(uint32_t speed);
	void setMode(uint16_t mode);
	void setPinSelect(uint8_t pin);

protected:
	p32_spi *m_pspi;
	uint8_t m_pinSS;
	
	int16_t m_displayWidth;
	int16_t m_displayHeight;
	int16_t m_frameWidth;
	int16_t m_frameHeight;
	int16_t m_numLayers;
	ATKIColor::RGBA *m_frames;

private:
	int16_t m_frameSize;
};


/*
	Based on code from kasperkamperman.com and <http://www.codeproject.com/miscctrl/CPicker.asp>

  dim_curve 'lookup table' to compensate for the nonlinearity of human vision.
  Used in the getRGB function on saturation and brightness to make 'dimming' look more natural. 
  Exponential function used to create values below : 
  x from 0 - 255 : y = round(pow( 2.0, x+64/40.0) - 1)   

*/

// 5x5 sprite format is [x][y] where the y values are (0, 7) -> (0, 0) so that the sprite definitions are not inverted
// coordinates are cylindrical so "x" values wrap around
/*
void WS2801::draw_sprite(void* DISPLAY_start, uint8_t x, uint8_t y, const void* sprite, RGBA color) {
	register uint8_t *p;
	register uint8_t *s;

	for (uint8_t i = 0; i < SPRITE_WIDTH; i++) {
		if ((x+i) % 2 == 0) {		// even columns are top to bottom
			s = (uint8_t *)sprite + i * SPRITE_HEIGHT;
			p = (uint8_t *)DISPLAY_start + ((x + i) % DISPLAY_WIDTH) * DISPLAY_HEIGHT * 3;

			for (uint8_t j = 0; j < SPRITE_HEIGHT; j++) {
				if ((j + y) >= DISPLAY_HEIGHT) {
					break;
				}
				*p++ = (uint8_t)((*s * color.b) >> 8);
				*p++ = (uint8_t)((*s * color.g) >> 8);
				*p++ = (uint8_t)((*s++ * color.r) >> 8);
			}
		} else {		// odd columns are bottom to top
			s = (uint8_t *)sprite + i * SPRITE_HEIGHT;
			p = (uint8_t *)DISPLAY_start + (((x + i + 1) % DISPLAY_WIDTH) * DISPLAY_HEIGHT - y) * 3;

			for (uint8_t j = 0; j < SPRITE_WIDTH; j++) {
				if ((j + x) >= DISPLAY_WIDTH) {
					break;
				}
				*--p = (uint8_t)((*s * color.b) >> 8);
				*--p = (uint8_t)((*s * color.g) >> 8);
				*--p = (uint8_t)((*s++ * color.r) >> 8);
			}
		}
	}
};
*/


#endif

/* WS2801_H_ */
