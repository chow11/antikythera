/*
 * ATKRoot.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include "ATKRoot.h"
#include "Antikythera.h"

bool ATKRoot::load(Stream *program) {
	return true;
}

bool ATKRoot::process(long now) {
	bool result = true;

	for (uint8_t count = 0; count < operandCount(); count++) {
		if ((operand(count).flags & OPERANDFLAG_LINK)) {
			result &= Antikythera::operators[operand(count).operatorIndex]->process(now);
		}
	}

	setProcessedFlag();

	return result;
};

