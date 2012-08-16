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


// <display type><display width><display height><frame width><frame height><num layers>
bool ATKDisplay::load(Stream *program) {
	bool result = true;
//	while(!program->available()) { }
//	int c = program->read();
	return result;
}
/*
	Operands:
		- operators that draw to the frame in order of back to front
*/

bool ATKDisplay::process(long now) {
	bool result = ATKIOperator::process(now);
	
	// assume display number input is set to 0 and Display start X,Y matches frame start
	Antikythera::displays[0]->render(0, 0);

	setProcessedFlag();

	return result;
}

