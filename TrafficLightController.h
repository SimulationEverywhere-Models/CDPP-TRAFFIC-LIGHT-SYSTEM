/*******************************************************************
*
*  DESCRIPTION: Atomic Model TrafficLightController
*
*  AUTHOR: Jidong Cao
*
*  EMAIL: mailto://caojidong@hotmail.com
*
*
*  DATE: 13/10/2003
*
*******************************************************************/

#ifndef __TrafficLightController_H
#define __TrafficLightController_H

//#include <list>
#include "atomic.h"     // class Atomic
#include "LightGroupController.h"

class TrafficLightController : public Atomic
{
typedef	LightGroupController::TrafficLightState LightState;
public:
	TrafficLightController( const string &name = "TrafficLightController" );

	virtual string className() const { return "TrafficLightController"; }
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

	Time sigma;
	State phase;
	
	Model& holdIn(const State &state, const Time &ta)
	{
        sigma = ta; phase = state;
        return Atomic::holdIn(state, ta);
	}

private:
	const Port &in;
	Port &light;
	Port &out;
	Time preparationTime;

	LightState current_state, next_state;

};	// class TrafficLightController


#endif   //TrafficLightController_H
