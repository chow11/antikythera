/*
 * ATKIOperator.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <Antikythera.h>
#include <ATKIOperator.h>
//#include <ATKValue.h>


ATKIOperator::ATKIOperator() {
	m_isProcessed = false;
	m_operationCount = 0;
}

bool ATKIOperator::load(Stream *program) {
	return true;
}

bool ATKIOperator::process(long now) {
	bool result = true;

	for (uint8_t count = 0; count < operandCount(); count++) {
		if ((operand(count).flags & OPERANDFLAG_LINK)) {
			result &= Antikythera::operators[operand(count).operatorIndex]->process(now);
		}
	}

	uint8_t max = 0;
	uint8_t min = 255;
	for (uint8_t i = 0; i < operandCount(); i++) {
		ATK_OPERAND o = operand(i);
		if (o.flags & OPERANDFLAG_LINK) {
			if (o.flags & OPERANDFLAG_LIMIT) {
				uint8_t temp = Antikythera::operators[o.operatorIndex]->resultSize(o.resultIndex);
				if (temp < min) {
					min = temp;
				}
			} else {
				uint8_t temp = Antikythera::operators[o.operatorIndex]->resultSize(o.resultIndex);
				if (temp > max) {
					min = max;
				}
			}
		}
	}
	m_operationCount = min;
	if (max < min) {
		m_operationCount = max;
	}


	return result;
};

uint8_t ATKIOperator::resultCount() {
	return 0;
}

bool ATKIOperator::isProcessed() {
	return m_isProcessed;
}

void ATKIOperator::resetProcessedFlag() {
	m_isProcessed = false;
}

void ATKIOperator::setProcessedFlag() {
	m_isProcessed = true;
}

ATKIOperator::~ATKIOperator() {
}

void *ATKIOperator::resultGeneric(uint8_t index) {
	return NULL;
}

uint8_t ATKIOperator::operandElementIndex(ATK_OPERAND o, uint8_t iteration) {
	uint8_t result = 0;

	uint8_t operandElementSize = Antikythera::operators[o.operatorIndex]->resultSize(o.resultIndex);
	if (o.flags & OPERANDFLAG_LINK) {
		if (o.flags & OPERANDFLAG_SINGLE) {
			result = 0;
		} else {
			if (m_operationCount < operandElementSize) {
				result = iteration;
			} else {
				if (o.flags & OPERANDFLAG_EXTEND) {
					result = operandElementSize - 1;
				} else {
					result = iteration % operandElementSize;
				}
			}
		}
	}

	return result;
}
