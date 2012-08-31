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
#include <ATKIMath.h>


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

		case ATKOPERATOR_IMATH:
			return new ATKIMath();
		break;

	};

	return NULL;
}
