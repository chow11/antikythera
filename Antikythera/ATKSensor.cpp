/*
 * ATKSensor.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <ATKSensor.h>
#include <Antikythera.h>


ATKSensor::ATKSensor() {
	m_name = "Sensor";
}

ATKSensor::~ATKSensor() {
}

// Sensor type,Sensor width,Sensor height,frame width,frame height,num layers)
bool ATKSensor::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (m_numOperands != 0) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "Sensor::load() - incorrect number(" + String(m_numOperands) + ") of operands specified, expected 0.";
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

bool ATKSensor::loadProperties(Stream *program) {
	return ATKIOperator::loadProperties(program);
}

void ATKSensor::initializeOperands(uint8_t numOperands) {
	ATKIOperator::initializeOperands(numOperands);
}

bool ATKSensor::initializeConstant(uint8_t operandIndex, uint16_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

#ifdef ANTIKYTHERA_DEBUG
	m_lastErrorString = "Sensor::initializeConstant() - operandIndex out of range.";
#endif
	return false;
}

void ATKSensor::setConstant(uint8_t operandIndex, uint16_t element, void *value) {
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKSensor::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKSensor::evaluate(unsigned long now) {
#endif
	if (m_isEvaluated) {
		return true;
	}
#ifdef ANTIKYTHERA_DEBUG
	debug->println("Sensor::evaluate()");
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	for (int16_t i; i < m_numOperations; i++) {
//		Antikythera::Sensors[0]->render(0, 0);
	}

	m_isEvaluated = true;

	return result;
}

void ATKSensor::getResult(uint8_t resultIndex, uint16_t element, void *value) {
}
