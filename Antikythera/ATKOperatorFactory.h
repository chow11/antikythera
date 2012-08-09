/*
 * ATKOperatorFactory.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_Operator_Factory_H_
#define ATK_Operator_Factory_H_

#include "ATKIOperator.h"

#define	ATKOPERATOR_ROOT		0
#define	ATKOPERATOR_DISPLAY		1
#define ATKOPERATOR_POINT		2
#define ATKOPERATOR_LINE		3
#define ATKOPERATOR_CIRCLE		4

class ATKOperatorFactory {
public:
	static ATKIOperator *createOperator(uint16_t OperatorType);

protected:
	ATKOperatorFactory();
};

#endif

/* ATK_Operator_Factory_H_ */
