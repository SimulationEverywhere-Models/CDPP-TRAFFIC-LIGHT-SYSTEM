/*******************************************************************
*
*  DESCRIPTION: class TrafficCollector
*
*  AUTHOR: Jidong cao
*
*  EMAIL: mailto://caojidong@hotmail.com
*
*
*  DATE: 18/10/2003
*
*******************************************************************/

#ifndef __TRAFFIC_COLLECTOR_H
#define __TRAFFIC_COLLECTOR_H

/** include files **/
#include "atomic.h"  // class Atomic


/** declarations **/
class TrafficCollector: public Atomic
{
public:
	
	TrafficCollector( const string &name = "TrafficCollector" ) ;

	virtual ~TrafficCollector() { }

	virtual string className() const {return "TrafficCollector";}

	virtual Model& holdIn(const State &state, const Time &ta)
	{
		sigma = ta; phase = state;
		return Atomic::holdIn(state, ta);	
	}
	
protected:
	Model &initFunction();

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

	Time sigma;
	State phase;
	
private:
	const Port &entered ;
	const Port &leaved;
	Port &throughput ;
	Port &curQueueLength;
	Port &aveQueueLength;

	//Time timeLeft;
	Time frequence;
	
	long enterCount;
	long leaveCount;
	
	long lastEnterCount;
	long lastLeaveCount;
	
	long checkCount;
	long lengthTotal;

	double freqInMin;
	
};	// class TrafficCollector


#endif   //__INTERSECTION_CONTROLLER_H
