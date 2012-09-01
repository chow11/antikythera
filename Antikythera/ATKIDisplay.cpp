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

void ATKIDisplay::initialize(int16_t displayWidth, int16_t displayHeight, int16_t frameWidth, int16_t frameHeight, int16_t numLayers) {
}

int16_t ATKIDisplay::displayWidth() {
	return 0;
}

int16_t ATKIDisplay::displayHeight() {
	return 0;
}

int16_t ATKIDisplay::frameWidth() {
	return 0;
}

int16_t ATKIDisplay::frameHeight() {
	return 0;
}

int16_t ATKIDisplay::numLayers() {
	return 0;
}

void ATKIDisplay::render(int16_t frameX, int16_t frameY) {
}

ATKIColor::RGBA* ATKIDisplay::framebuffer(int16_t layer) {
	return NULL;
}

void ATKIDisplay::circle(int16_t x, int16_t y, int16_t r, ATKIColor::HSVA c, int16_t thickness, int16_t style, int16_t layer) {
}


void ATKIDisplay::circle(int16_t x, int16_t y, int16_t r, ATKIColor::RGBA c, int16_t thickness, int16_t style, int16_t layer) {
}


void ATKIDisplay::line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, ATKIColor::HSVA c, int16_t thickness, int16_t style, int16_t layer) {
}


void ATKIDisplay::line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, ATKIColor::RGBA c, int16_t thickness, int16_t style, int16_t layer) {
}


void ATKIDisplay::point(int16_t x, int16_t y, ATKIColor::HSVA c, int16_t style, int16_t layer) {
}


void ATKIDisplay::point(int16_t x, int16_t y, ATKIColor::RGBA c, int16_t style, int16_t layer) {
}
