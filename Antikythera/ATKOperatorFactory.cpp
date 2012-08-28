/*
 * ATKOperatorFactory.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <ATKOperatorFactory.h>
#include <ATKRoot.h>
#include <ATKDisplay.h>
#include <ATKPoint.h>
#include <ATKLine.h>
#include <ATKCircle.h>
#include <ATKSignal.h>
#include <ATKIntegerMath.h>


ATKIOperator *ATKOperatorFactory::createOperator(uint16_t operatorType) {

	switch (operatorType) {
		case ATKOPERATOR_ROOT:		// root Operator - always added before loading a patch to point to leaf Operators
			return new ATKRoot();
		break;

		case ATKOPERATOR_DISPLAY:
			return new ATKDisplay();
		break;

		case ATKOPERATOR_POINT:
			return new ATKPoint();
		break;

		case ATKOPERATOR_LINE:
			return new ATKLine();
		break;

		case ATKOPERATOR_CIRCLE:
			return new ATKCircle();
		break;

		case ATKOPERATOR_SIGNAL:
			return new ATKSignal();
		break;

		case ATKOPERATOR_IMATH + OPERANDTYPE_INT8:
			return new ATKIntegerMath<int8_t>();
		break;

		case ATKOPERATOR_IMATH + OPERANDTYPE_INT16:
			return new ATKIntegerMath<int16_t>();
		break;

		case ATKOPERATOR_IMATH + OPERANDTYPE_INT32:
			return new ATKIntegerMath<int32_t>();
		break;

		case ATKOPERATOR_IMATH + OPERANDTYPE_UINT8:
			return new ATKIntegerMath<uint8_t>();
		break;

		case ATKOPERATOR_IMATH + OPERANDTYPE_UINT16:
			return new ATKIntegerMath<uint16_t>();
		break;

		case ATKOPERATOR_IMATH + OPERANDTYPE_UINT32:
			return new ATKIntegerMath<uint32_t>();
		break;

	};

	return NULL;
}
