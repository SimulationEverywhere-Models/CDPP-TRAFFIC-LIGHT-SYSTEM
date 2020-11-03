/*******************************************************************
*
*  DESCRIPTION: class IntersectionController
*
*  AUTHOR: Jidong cao
*
*  EMAIL: mailto://caojidong@hotmail.com
*
*
*  DATE: 18/10/2003
*
*******************************************************************/

#ifndef __INTERSECTION_CONTROLLER_H
#define __INTERSECTION_CONTROLLER_H

/** include files **/
#include "atomic.h"  // class Atomic


/** declarations **/
class IntersectionController: public Atomic
{
	enum {BUSYING=1, OUTPUTING=2};
public:
	
	IntersectionController( const string &name = "IntersectionController" ) ;

	virtual ~IntersectionController() { }

	virtual string className() const {return "IntersectionController";}

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
	const Port &in;
	Port &out;
	Port &busy;
	
	Time responseTime;
	int action;
	
	Time serviceTime;
	
};	// class IntersectionController


#endif   //__INTERSECTION_CONTROLLER_H
