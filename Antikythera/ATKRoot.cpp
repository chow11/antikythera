/*
 * ATKRoot.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <ATKRoot.h>
#include <Antikythera.h>


ATKRoot::ATKRoot() {

}

ATKRoot::~ATKRoot() {

}

bool ATKRoot::load(Stream *program) {
	return true;
}

bool ATKRoot::evaluate(unsigned long now) {
	bool result = ATKIOperator::evaluate(now);

	setEvaluatedFlag();

	return result;
};

void *ATKRoot::constantGeneric(uint8_t index) {
	return NULL;
}

bool ATKRoot::initializeConstant(uint8_t operandIndex, uint8_t constantSize) {
#ifdef ANTIKYTHERA_DEBUG
	this->lastErrorString = "ATKRoot::initializeConstant() - operandIndex out of range.";
#endif
	return false;
}

bool ATKRoot::loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program) {
	return ATKIOperator::loadConstant(operandIndex, flags, program);
}
