/*
 * ATKSelect.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <WProgram.h>
#include <ATKSelect.h>
#include <Antikythera.h>


ATKSelect::ATKSelect() {
	m_name = "Select";

	m_resultSize = new uint16_t[1];
	m_resultSize[0] = 0;

	m_evaluatedIndex = 0;
	m_consts = NULL;
}

ATKSelect::~ATKSelect() {
	for (uint8_t count = 0; count < m_numOperands; count++) {
		delete[] m_consts[count];
	}
	delete[] m_consts;
}

bool ATKSelect::load(Stream *program) {
	if (!ATKIOperator::load(program)) {
		return false;
	}

	if (m_numOperands < 2) {
#ifdef ANTIKYTHERA_DEBUG
		m_lastErrorString = "Select::load() - incorrect number(" + String(m_numOperands) + ") of operands specified, expected at least 2.";
#endif
		program->flush();
		return false;
	}

	return true;
}

bool ATKSelect::loadProperties(Stream *program) {
	return ATKIOperator::loadProperties(program);
}

void ATKSelect::initializeOperands(uint8_t numOperands) {
	ATKIOperator::initializeOperands(program);

	m_consts = new int16_t *[numOperands];
}

bool ATKSelect::initializeConstant(uint8_t operandIndex, uint16_t constantSize) {
	ATKIOperator::initializeConstant(operandIndex, constantSize);

	m_consts[operandIndex] = new int16_t[constantSize];

	return true;
}

void ATKSelect::setConstant(uint8_t operandIndex, uint16_t element, void *value) {
	m_consts[operandIndex][element] = *((int16_t *)value);
}

#ifdef ANTIKYTHERA_DEBUG
bool ATKSelect::evaluate(unsigned long now, Stream *debug) {
#else
bool ATKSelect::evaluate(unsigned long now) {
#endif
	if (m_isEvaluated) {
		return true;
	}

	bool result = true;

	if ((m_operands[0].flags & OPERANDFLAG_LINK)) {
#ifdef ANTIKYTHERA_DEBUG
		result &= Antikythera::operators[m_operands[0].operatorIndex]->evaluate(now, debug);
#else
		result &= Antikythera::operators[m_operands[0].operatorIndex]->evaluate(now);
#endif
	}

	m_numResults = 0;
	delete[] m_resultSize;

	ATK_OPERAND o = m_operands[0];
	uint16_t operations = (o.flags & OPERANDFLAG_LINK) ? Antikythera::operators[o.operatorIndex]->m_resultSize[o.resultIndex] : m_constantSize[0];
	if (operations > 0) {
		int16_t m_evaluatedIndex;
		OPERAND_ELEMENT(m_evaluatedIndex, m_consts[0], 0, 0)
		if (m_evaluatedIndex > 0 && m_evaluatedIndex < m_numOperands) {
			if ((m_operands[m_evaluatedIndex].flags & OPERANDFLAG_LINK)) {
		#ifdef ANTIKYTHERA_DEBUG
				result &= Antikythera::operators[m_operands[m_evaluatedIndex].operatorIndex]->evaluate(now, debug);
		#else
				result &= Antikythera::operators[m_operands[m_evaluatedIndex].operatorIndex]->evaluate(now);
		#endif
				m_numResults = Antikythera::operators[m_operands[m_evaluatedIndex].operatorIndex]->m_numResults;
				m_resultSize = new int16_t[m_numResults];
				for (uint8_t count = 0; count < m_numResults; count++) {
					m_resultSize[count] = Antikythera::operators[m_operands[m_evaluatedIndex].operatorIndex]->m_resultSize[count];
				}
			} else {
				m_numResults = 1;
				m_resultSize = new int16_t[1];
				m_resultSize[0] = m_constantSize[m_evaluatedIndex];
			}
		} else {
			m_evaluatedIndex = 0;
		}
	}

#ifdef ANTIKYTHERA_DEBUG
		debug->println("Select::evaluate(" + String(now) + ", " + String(i) + ": " + String(m_evaluatedIndex));
#endif

	m_isEvaluated = true;

	return result;
}

void ATKSelect::getResult(uint8_t resultIndex, uint16_t element, void *value) {
	if ((m_operands[m_evaluatedIndex].flags & OPERANDFLAG_LINK)) {
		Antikythera::operators[m_operands[m_evaluatedIndex].operatorIndex]->getResult(resultIndex, element, value);
	} else {
		if (element < m_constantSize[resultIndex]) {
			*value = m_consts[resultIndex][element];
		}
	}
}
