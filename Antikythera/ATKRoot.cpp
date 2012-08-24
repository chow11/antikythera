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
	m_constLeaf = NULL;
}

ATKRoot::~ATKRoot() {
	delete[] m_constLeaf;
}

bool ATKRoot::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (numOperands() != 1) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKRoot::load() - incorrect number(" + String(numOperands()) + ") of operands specified, expected 4.";
#endif
		program->flush();
		return false;
	}

	return true;
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKRoot::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKRoot::evaluate(unsigned long now) {
#endif
#ifdef ANTIKYTHERA_DEBUG
	bool result = ATKIOperator::evaluate(now, debug);
#else
	bool result = ATKIOperator::evaluate(now);
#endif

	for (uint8_t i; i < numOperations(); i++) {
		ATK_OPERAND o = operand(0);
		uint8_t leaf = (o.flags & OPERANDFLAG_LINK) ? Antikythera::operators[o.operatorIndex]->result<uint8_t>(o.resultIndex)[operandElementIndex(o, i)] : constant<uint8_t>(i)[operandElementIndex(o, i)];

#ifdef ANTIKYTHERA_DEBUG
		result &= Antikythera::operators[leaf]->evaluate(now, debug);
#else
	result &= Antikythera::operators[leaf]->evaluate(now);
#endif
	}

	setEvaluatedFlag();

	return result;
};

void *ATKRoot::constantGeneric(uint8_t index) {
	switch (index) {
	case 0:
		return m_constLeaf;
	}

	return NULL;
}

bool ATKRoot::initializeConstant(uint8_t operandIndex, uint8_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

	switch (operandIndex) {
	case 0:
		m_constLeaf = new uint8_t[constantSize];
		break;

	default:
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "ATKRoot::initializeConstant() - operandIndex out of range.";
#endif
		return false;
	}

	return true;
}

bool ATKRoot::loadConstant(uint8_t operandIndex, uint8_t flags, Stream *program) {
	return ATKIOperator::loadConstant(operandIndex, flags, program);
}
