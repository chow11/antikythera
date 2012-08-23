/*
 * ATK.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ANTIKYTHERA_H_
#define ANTIKYTHERA_H_


#include <Stream.h>
#include <ATKIncludes.h>
#include <ATKIOperator.h>
#include <ATKIDisplay.h>
#include <ATKISensor.h>


class Antikythera {
public:
#ifdef ANTIKYTHERA_DEBUG
	static bool evaluate(unsigned long now, Stream *debug);
#else
	static bool evaluate(unsigned long now);
#endif

	static bool load(Stream *patch);
	static void unload();

	static void resetProcessedFlags();

	static uint16_t numOperators;
	static ATKIOperator **operators;

	// installed hardware components added during setup()
	static uint8_t numDisplays;
	static ATKIDisplay **displays;

	static uint8_t numSensors;
	static ATKISensor **sensors;

	static String lastErrorString;
};


#endif

/* ANTIKYTHERA_H_ */
