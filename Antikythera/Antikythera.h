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

#define ANTIKYTHERA_DEBUG


#include <stddef.h>
#include <inttypes.h>
#include <Stream.h>
#include <ATKIOperator.h>
#include <ATKIDisplay.h>
#include <ATKISensor.h>


class Antikythera {
public:
	static bool process(long now);

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
