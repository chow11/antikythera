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
	m_name = "Root";
	m_constLeaf = NULL;
}

ATKRoot::~ATKRoot() {
	delete[] m_constLeaf;
}

bool ATKRoot::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (m_numOperands != 1) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKRoot::load() - incorrect number(" + String(m_numOperands) + ") of operands specified, expected 4.";
#endif
		program->flush();
		return false;
	}

	return true;
}

bool ATKRoot::loadProperties(Stream *program) {
	return ATKIOperator::loadProperties(program);
}

void ATKRoot::initializeOperands(uint8_t numOperands) {
	ATKIOperator::initializeOperands(program);
}

bool ATKRoot::initializeConstant(uint8_t operandIndex, uint16_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

	switch (operandIndex) {
	case 0:
		m_constLeaf = new int16_t[constantSize];
		break;

	default:
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKRoot::initializeConstant() - operandIndex out of range.";
#endif
		return false;
	}

	return true;
}

void ATKRoot::setConstant(uint8_t operandIndex, uint16_t element, void *value) {
	switch (operandIndex) {
	case 0:
		m_constLeaf[element] = *((int16_t *)value);
		break;
	}
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKRoot::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKRoot::evaluate(unsigned long now) {
#endif
	if (m_isEvaluated) {
		return true;
	}
#ifdef ANTIKYTHERA_DEBUG
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	for (int16_t i; i < m_numOperations; i++) {
		int16_t leaf;
		OPERAND_ELEMENT(leaf, m_constLeaf, 0, i)

#ifdef ANTIKYTHERA_DEBUG
		debug->println("ATKRoot::evaluate(" + String(now) + ", " + String(i) + ": " + String(leaf) + ")");
#endif
#ifdef ANTIKYTHERA_DEBUG
		result &= Antikythera::operators[leaf]->evaluate(now, debug);
#else
		result &= Antikythera::operators[leaf]->evaluate(now);
#endif
	}

	m_isEvaluated = true;

	return result;
};

void ATKRoot::getResult(uint8_t resultIndex, uint16_t element, void *value) {
}
