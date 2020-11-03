/*******************************************************************
*
*  DESCRIPTION: Atomic Model LightGroupController
*
*  AUTHOR: Jidong Cao	
*
*  EMAIL: mailto://caojidong@hotmail.com
*
*
*  DATE: 13/10/2003
*
*******************************************************************/

#ifndef __LightGroupController_H
#define __LightGroupController_H

#include "atomic.h"     // class Atomic

class LightGroupController : public Atomic
{
public:
	enum TrafficLightState {RED=1, GREEN, YELLOW, STANDBY};
	LightGroupController( const string &name = "LightGroupController" );					//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &standby_in;
	Port &lights;
	Port &standby_out;

	Time green_time, yellow_time, standby_time;

	TrafficLightState current_state;

};	// class LightsGroupController

// ** inline ** // 
inline
string LightGroupController::className() const
{
	return "LightGroupController" ;
}

#endif   //__LightGroupController_H
