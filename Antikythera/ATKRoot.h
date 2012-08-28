/*
 * ATKATKRoot.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Brian Chojnowski
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ATK_ROOT_H_
#define ATK_ROOT_H_


#include <Stream.h>
#include <ATKIncludes.h>
#include <ATKIOperator.h>


class ATKRoot : public ATKIOperator {
public:
	ATKRoot();
	~ATKRoot();

	virtual String name() { return "ATKRoot"; }

	// loading
public:
	virtual bool load(Stream *program);
protected:
	virtual bool loadProperties(Stream *program);
	virtual bool initializeConstant(uint8_t operandIndex, uint8_t constantSize);

	// evaluation
public:
	#ifdef ANTIKYTHERA_DEBUG
	virtual bool evaluate(unsigned long now, Stream *debug);
#else
	virtual bool evaluate(unsigned long now);
#endif

	// operands

	// constants
protected:
	virtual void *constantGeneric(uint8_t index);

	// operations

	// results
public:
	virtual uint8_t numResults() { return 0; }
	virtual uint8_t resultSize(uint8_t index) { return 0; }
	virtual void result(uint8_t index, uint8_t element, void *value, uint8_t valueType) { }

private:
	uint8_t *m_constLeaf;
};


#endif

/* ATK_ROOT_H_ */
