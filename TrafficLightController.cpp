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

/** include files **/
#include "TrafficLightController.h"      // class TrafficLightController
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

/** public functions **/
#define RED		LightGroupController::RED
#define YELLOW	LightGroupController::YELLOW
#define GREEN	LightGroupController::GREEN

/*******************************************************************
* Function Name: TrafficLightController
* Description: 
********************************************************************/
TrafficLightController::TrafficLightController( const string &name )
: Atomic( name )
, in( addInputPort( "in" ) )
, light( addInputPort( "light" ) )
, out( addOutputPort( "out" ) )
, preparationTime( 0, 0, 0, 10 )
, current_state(RED)
, next_state(RED)
{ }

/*******************************************************************
* Function Name: initFunction
* Description:
********************************************************************/
Model &TrafficLightController::initFunction()
{
	passivate();
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &TrafficLightController::externalFunction( const ExternalMessage &msg )
{
	Time e = msg.time() - lastChange();
	
	next_state = (LightState)msg.value();
	if(next_state != current_state)
	{
		holdIn(active, preparationTime);
	}
	else
	{
		printf("The model %s has discarded duplicated signal %d\n ",
		       description().data(), next_state ) ;
		if(state()==active)
		{
			holdIn(active, sigma - e);
		}
	}
	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &TrafficLightController::internalFunction( const InternalMessage &msg )
{
	if (current_state==RED && next_state==GREEN)
	{
		current_state = next_state;
		passivate();
	}
	else if(current_state==GREEN && next_state==YELLOW)
	{
		current_state = next_state;
		passivate();
	}
	else if(current_state==YELLOW && next_state==RED)
	{
		current_state = next_state;
		passivate();
	}
	else
	{
		MException e;
		e.addText( "The model " + description() + " has wrong state for internal message!" ) ;
		e.print(cerr);
		//MTHROW( e ) ;
		exit(1);
	}
	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description: 
********************************************************************/
Model &TrafficLightController::outputFunction( const InternalMessage &msg )
{
	if (current_state==RED && next_state==GREEN)
	{
		sendOutput(msg.time(), light, GREEN);
		sendOutput(msg.time(), out, 0);
	}
	else if(current_state==GREEN && next_state==YELLOW)
	{
		sendOutput(msg.time(), light, YELLOW);
	}
	else if(current_state==YELLOW && next_state==RED)
	{
		sendOutput(msg.time(), light, RED);
		sendOutput(msg.time(), out, 1);
	}
	else
	{
		MException e;
		e.addText( "The model " + description() + " has wrong state for output function!" ) ;
		e.print(cerr);
		//MTHROW( e ) ;
		exit(1);
		
	}
	return *this ;
}
