/*******************************************************************
*
*  DESCRIPTION: Atomic Model IntersectionQueue
*
*  AUTHOR: Jidong Cao
*
*  EMAIL: mailto://caojidong@hotmail.com
*
*
*  DATE: 14/10/2003
*
*******************************************************************/

#ifndef __INTERSECTION_QUEUE_H
#define __INTERSECTION_QUEUE_H

#include <list>
#include "atomic.h"     // class Atomic

class IntersectionQueue : public Atomic
{
public:
	enum {SENDING=1, NEXTING = 2, BLOCKING=4};

	IntersectionQueue( const string &name = "IntersectionQueue" );	//Default constructor

	virtual string className() const ;
	virtual Model &holdIn( const State &, const Time & ) ;

protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

	bool full() const;

	struct Pair
	{
		int id; double time;
		Pair(int carId, double serviceTimeInSeconds)
		{
			id = carId; time = serviceTimeInSeconds;
		}
	};

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
	typedef list<Pair> ElementList ;
	ElementList elements ;

	unsigned int N;
	bool input, output;
	
	Time sigma;
	State phase;
	
	bool pending;
	int action;
	
	double crossingTime;
	Time sendingTimeLeft;

};	// class IntersectionQueue

// ** inline ** // 
inline
string IntersectionQueue::className() const
{
	return "IntersectionQueue" ;
}

inline
bool IntersectionQueue::full() const
{
	return (N>0 && elements.size()>=N);
}

inline
Model& IntersectionQueue::holdIn(const State &state, const Time &ta)
{
	sigma = ta; phase = state;
	return Atomic::holdIn(state, ta);	
}

#endif   //__GENERIC_QUEUE_H
