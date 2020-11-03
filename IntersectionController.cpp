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

/** include files **/
#include "IntersectionController.h"           // base header
#include "message.h"       // InternalMessage ....
#include "mainsimu.h"      // class MainSimulator

/*******************************************************************
* Function Name:
* Description: constructor
********************************************************************/
IntersectionController::IntersectionController( const string &name )
: Atomic( name )
, in( addInputPort( "in" ) )
, out( addOutputPort( "out" ) )
, busy( addOutputPort( "busy" ) )
, responseTime(0,0,0,0)
, serviceTime(0,0,0,0)
{
}

/*******************************************************************
* Function Name: initFunction
* Description:
* Precondition:
********************************************************************/
Model &IntersectionController::initFunction()
{
	action = 0;
	passivate();
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: the intersection controller receive a car
********************************************************************/
Model &IntersectionController::externalFunction( const ExternalMessage &msg )
{
	Time e = msg.time() - lastChange();
	if(state()==passive)
	{
		action |= BUSYING;
		holdIn(active, responseTime);
		serviceTime = msg.value()>0 ? Time(0, 0, 0, int(msg.value() * 1000)) : responseTime;
//		printf("%s@%s: service time %s\n",
//			description().data(), className().data(), serviceTime.asString().data());
	}
	else
	{
		printf("%s@%s: discarded input[%f] when busy\n",
			description().data(), className().data(), msg.value());
		holdIn(active, sigma-e);
	}
	
	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &IntersectionController::internalFunction( const InternalMessage & )
{
	if(action & BUSYING)
	{
		action &= ~BUSYING;
		action |= OUTPUTING;
		holdIn(active, serviceTime - responseTime);
	}
	else if(action & OUTPUTING)
	{
		action = 0;
		passivate();
	}
	else
	{
		printf("%s@%s: error state in output function %d\n",
			description().data(), className().data(), action);
		action = 0;
		passivate();
	}
	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model& IntersectionController::outputFunction( const InternalMessage &msg )
{
	if(action & BUSYING)
	{
		sendOutput(msg.time(), busy, 1);
	}
	else if(action & OUTPUTING)
	{
		sendOutput(msg.time(), out, 1);
	}
	else
	{
		printf("%s@%s: error state in output function %d\n",
			description().data(), className().data(), action);
	}
	return *this;
}
