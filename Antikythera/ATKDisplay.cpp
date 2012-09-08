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
	m_name = "Display";
}

ATKDisplay::~ATKDisplay() {
}

// display type,display width,display height,frame width,frame height,num layers)
bool ATKDisplay::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (m_numOperands != 0) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "Display::load() - incorrect number(" + String(m_numOperands) + ") of operands specified, expected 0.";
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

bool ATKDisplay::loadProperties(Stream *program) {
	return ATKIOperator::loadProperties(program);
}

void ATKDisplay::initializeOperands(uint8_t numOperands) {
	ATKIOperator::initializeOperands(numOperands);
}

bool ATKDisplay::initializeConstant(uint8_t operandIndex, uint16_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

#ifdef ANTIKYTHERA_DEBUG
	m_lastErrorString = "Display::initializeConstant() - operandIndex out of range.";
#endif
	return false;
}

void ATKDisplay::setConstant(uint8_t operandIndex, uint16_t element, void *value) {
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKDisplay::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKDisplay::evaluate(unsigned long now) {
#endif
	if (m_isEvaluated) {
		return true;
	}
#ifdef ANTIKYTHERA_DEBUG
	debug->println("Display::evaluate()");
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

//	for (int16_t i; i < m_numOperations; i++) {
		// assume display number input is set to 0 and Display start X,Y matches frame start
		Antikythera::displays[0]->render(0, 0);
//	}

	m_isEvaluated = true;

	return result;
}

void ATKDisplay::getResult(uint8_t resultIndex, uint16_t element, void *value) {
}
