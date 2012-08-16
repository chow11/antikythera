/*
 * ATK.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <Antikythera.h>
#include <ATKOperatorFactory.h>


ATKIOperator *Antikythera::operators[256];
uint8_t Antikythera::numDisplays = 0;
ATKIDisplay **Antikythera::displays = NULL;
uint8_t Antikythera::numSensors = 0;
ATKISensor **Antikythera::sensors = NULL;


bool Antikythera::process(long now) {
	if (!operators[0]) {
		return true;
	}

	resetProcessedFlags();

	return operators[0]->process(now);
};

bool Antikythera::load(Stream *program) {
	bool result = true;

	return result;
};

void Antikythera::unload() {
	for (uint8_t count = 0; count < 256; count++) {
		delete operators[count];
	}
}

void Antikythera::resetProcessedFlags() {
	for (uint8_t count = 0; count < 256; count++) {
		ATKIOperator *node = operators[count];
		if (node == NULL) {
			break;
		}
		node->resetProcessedFlag();
	}
};
