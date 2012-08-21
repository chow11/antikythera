/*
 * ATKIDisplay.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <ATKIDisplay.h>


ATKIDisplay::ATKIDisplay() {

}

ATKIDisplay::~ATKIDisplay() {

}

void ATKIDisplay::initialize(uint16_t displayWidth, uint16_t displayHeight, uint16_t frameWidth, uint16_t frameHeight, uint8_t numLayers) {
}

uint16_t ATKIDisplay::displayWidth() {
	return 0;
}

uint16_t ATKIDisplay::displayHeight() {
	return 0;
}

uint16_t ATKIDisplay::frameWidth() {
	return 0;
}

uint16_t ATKIDisplay::frameHeight() {
	return 0;
}

uint8_t ATKIDisplay::numLayers() {
	return 0;
}

void ATKIDisplay::render(uint16_t frameX, uint16_t frameY) {
}

ATKColor::RGBA* ATKIDisplay::framebuffer(uint8_t layer) {
	return NULL;
}

void ATKIDisplay::circle(int8_t x, int8_t y, int8_t r, ATKColor::HSVA c, int8_t thickness, uint8_t style, uint8_t layer) {
}


void ATKIDisplay::circle(int8_t x, int8_t y, int8_t r, ATKColor::RGBA c, int8_t thickness, uint8_t style, uint8_t layer) {
}


void ATKIDisplay::line(int8_t x1, int8_t y1, int8_t x2, int8_t y2, ATKColor::HSVA c, int8_t thickness, uint8_t style, uint8_t mode, uint8_t layer) {
}


void ATKIDisplay::line(int8_t x1, int8_t y1, int8_t x2, int8_t y2, ATKColor::RGBA c, int8_t thickness, uint8_t style, uint8_t mode, uint8_t layer) {
}


void ATKIDisplay::point(int8_t x, int8_t y, ATKColor::HSVA c, uint8_t style, uint8_t layer) {
}


void ATKIDisplay::point(int8_t x, int8_t y, ATKColor::RGBA c, uint8_t style, uint8_t layer) {
}
