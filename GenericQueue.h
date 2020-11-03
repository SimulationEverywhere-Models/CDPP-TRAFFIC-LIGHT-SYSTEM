/*******************************************************************
*
*  DESCRIPTION: Atomic Model GenericQueue
*
*  AUTHOR: Jidong Cao
*
*  EMAIL: mailto://caojidong@hotmail.com
*
*
*  DATE: 14/10/2003
*
*******************************************************************/

#ifndef __GENERIC_QUEUE_H
#define __GENERIC_QUEUE_H

#include <list>
#include "atomic.h"     // class Atomic

class GenericQueue : public Atomic
{
public:
	enum {SENDING=1, NEXTING = 2, BLOCKING=4};

	GenericQueue( const string &name = "GenericQueue" );					//Default constructor

	virtual string className() const ;
	virtual Model &holdIn( const State &, const Time & ) ;

protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

	bool full() const;

protected:
	const Port &in;
	const Port &done;
	const Port &stop_in;
	const Port &stop_out;
	Port &out;
	Port &next;
	Port &block;
	Time preparationTime;
	Time responseTime;
	typedef list<Value> ElementList ;
	ElementList elements ;

	unsigned int N;
	bool input, output;
	
	Time sigma;
	State phase;
	
	bool pending;
	int action;
	Time sendingTimeLeft;

};	// class GenericQueue

// ** inline ** // 
inline
string GenericQueue::className() const
{
	return "GenericQueue" ;
}

inline
bool GenericQueue::full() const
{
	return (N>0 && elements.size()>=N);
}

inline
Model& GenericQueue::holdIn(const State &state, const Time &ta)
{
	sigma = ta; phase = state;
	return Atomic::holdIn(state, ta);	
}

#endif   //__GENERIC_QUEUE_H
