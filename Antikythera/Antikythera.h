/*
 * ATK.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_H_
#define ATK_H_

#include <Stream.h>
#include "ATKIOperator.h"
#include "ATKIDisplay.h"
#include "ATKISensor.h"

class Antikythera {
public:
	static bool process(long now);

	static bool load(Stream *patch);
	static void unload();

	static void resetProcessedFlags();

	static ATKIOperator *operators[256];

	// installed hardware components added during setup()
	static uint8_t numDisplays;
	static ATKIDisplay **displays;

	static uint8_t numSensors;
	static ATKISensor **sensors;
};

#endif

/* ATK_H_ */
