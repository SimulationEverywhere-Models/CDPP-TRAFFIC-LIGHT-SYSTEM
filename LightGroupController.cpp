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

/** include files **/
#include "LightGroupController.h"      // class LightGroupController
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

/** public functions **/

/*******************************************************************
* Function Name: LightGroupController
* Description: 
********************************************************************/
LightGroupController::LightGroupController( const string &name )
: Atomic( name )
, standby_in( addInputPort( "standby_in" ) )
, lights( addInputPort( "lights" ) )
, standby_out( addInputPort( "standby_out" ) )
, green_time(0, 0, 30, 0)
, yellow_time(0, 0, 3, 0)
, standby_time(0, 0, 1, 0)
, current_state(RED)
{
	string time_g( MainSimulator::Instance().getParameter( description(), "green" ) ) ;
	string time_y( MainSimulator::Instance().getParameter( description(), "yellow" ) ) ;
	string time_s( MainSimulator::Instance().getParameter( description(), "standby" ) ) ;

	if( time_g != "" ) green_time = time_g ;
	if( time_y != "" ) yellow_time = time_y ;
	if( time_s != "" ) standby_time = time_s ;

	string init_state(MainSimulator::Instance().getParameter( description(), "state" ) );
	if (init_state != "")
	{
		if(init_state == "red")
			current_state = RED;
		else if (init_state == "green")
			current_state = GREEN;
		else if (init_state == "yellow")
			current_state = YELLOW;
		else if (init_state == "standby")
			current_state = STANDBY;
		else
		{
			MException e;
			e.addText( "The model " + description() + " has unknown initial state!" ) ;
			e.print(cerr);
			MTHROW( e ) ;
		}
	}

}

/*******************************************************************
* Function Name: initFunction
* Description:
* Precondition:
********************************************************************/
Model &LightGroupController::initFunction()
{
	switch(current_state)
	{
	case RED:
		passivate();
		break;
	case YELLOW:
		holdIn(active, yellow_time);
		break;
	case GREEN:
		holdIn(active, green_time);
		break;
	case STANDBY:
		holdIn(active, standby_time);
		break;
	}
	
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &LightGroupController::externalFunction( const ExternalMessage &msg )
{
//	printf("%s: received %d\n", description().data(), current_state);
	
	if(current_state != RED)
	{
		MException e;
		e.addText( "The initial states of two light group controllers are wrong in the [" + parent().description() + "] model!!" ) ;
		e.addText( "They must be one in RED, and another is NOT RED." ) ;
		e.print(cerr);
		//MTHROW( e ) ;
		exit(1);
		return *this;
	}

	current_state = STANDBY;
	this->holdIn(active, standby_time);

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &LightGroupController::internalFunction( const InternalMessage & )
{
//	printf("%s: internal in state %d\n", description().data(), current_state);
	switch(current_state)
	{
	case GREEN:
		current_state = YELLOW;
		this->holdIn(active, yellow_time);
		break;
	case YELLOW:
		current_state = RED;
		passivate();
		break;
	case STANDBY:
		current_state = GREEN;
		this->holdIn(active, green_time);
		break;
	case RED:
		MException e;
		e.addText( "The model " + description() + " has wrong RED state for internal message!" ) ;
		e.print(cerr);
		//MTHROW( e ) ;
		exit(1);
		break;
	}
	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description: 
********************************************************************/
Model &LightGroupController::outputFunction( const InternalMessage &msg )
{
//	printf("%s: output in state %d\n", description().data(), current_state);
	switch(current_state)
	{
	case GREEN:
		sendOutput(msg.time(), lights, YELLOW);
		break;
	case YELLOW:
		sendOutput(msg.time(), lights, RED);
		sendOutput(msg.time(), standby_out, RED);
		break;
	case STANDBY:
		sendOutput(msg.time(), lights, GREEN);
		break;
	case RED:
		MException e;
		e.addText( "The model " + description() + " has wrong RED state for output message!" ) ;
		e.print(cerr);
		//MTHROW( e ) ;
		exit(1);
		break;
	}

	return *this ;
}
