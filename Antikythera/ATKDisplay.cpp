/*
 * ATKDisplay.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <ATKDisplay.h>
#include <Antikythera.h>


ATKDisplay::ATKDisplay() {

}

ATKDisplay::~ATKDisplay() {

}

// display type,display width,display height,frame width,frame height,num layers)
bool ATKDisplay::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (numOperands() != 0) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKDisplay::load() - incorrect number(" + String(numOperands()) + ") of operands specified, expected 0.";
#endif
		program->flush();
		return false;
	}

	return true;
}
/*
	Operands:
		- operators that draw to the frame in order of back to front
*/

#ifdef ANTIKYTHERA_DEBUG
bool ATKDisplay::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKDisplay::evaluate(unsigned long now) {
#endif
#ifdef ANTIKYTHERA_DEBUG
	debug->println("ATKDisplay::evaluate()");
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

//	for (uint8_t i; i < numOperations(); i++) {
		// assume display number input is set to 0 and Display start X,Y matches frame start
		Antikythera::displays[0]->render(0, 0);
//	}

	setEvaluatedFlag();

	return result;
}

void *ATKDisplay::constantGeneric(uint8_t index) {
	return NULL;
}

bool ATKDisplay::initializeConstant(uint8_t operandIndex, uint8_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

#ifdef ANTIKYTHERA_DEBUG
	m_lastErrorString = "ATKDisplay::initializeConstant() - operandIndex out of range.";
#endif
	return false;
}

bool ATKDisplay::loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program) {
	return ATKIOperator::loadConstant(operandIndex, flags, program);
}
