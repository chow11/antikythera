/*
 * ATKOperatorFactory.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_OPERATOR_FACTORY_H_
#define ATK_OPERATOR_FACTORY_H_


#include <ATKIncludes.h>
#include <ATKIOperator.h>


#define	ATKOPERATOR_ROOT		0
#define	ATKOPERATOR_DISPLAY		1
#define ATKOPERATOR_POINT		2
#define ATKOPERATOR_LINE		3
#define ATKOPERATOR_CIRCLE		4
#define ATKOPERATOR_SIGNAL		5
#define ATKOPERATOR_IMATH		6		// + OPERANDTYPE_
#define ATKOPERATOR_FMATH		22		// + OPERANDTYPE_
#define ATKOPERATOR_STRING		24


class ATKOperatorFactory {
public:
	static ATKIOperator *createOperator(uint16_t OperatorType);

protected:
	ATKOperatorFactory();
};


#endif

/* ATK_OPERATOR_FACTORY_H_ */
