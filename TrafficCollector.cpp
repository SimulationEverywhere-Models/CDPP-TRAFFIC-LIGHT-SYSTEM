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

/** include files **/
#include "TrafficCollector.h"           // base header
#include "message.h"       // InternalMessage ....
#include "mainsimu.h"      // class MainSimulator

/*******************************************************************
* Function Name: CPU
* Description: constructor
********************************************************************/
TrafficCollector::TrafficCollector( const string &name )
: Atomic( name )
, entered( addInputPort( "entered" ) )
, leaved( addOutputPort( "leaved" ) )
, throughput( addOutputPort( "throughput" ) )
, curQueueLength( addOutputPort( "curQueueLength" ) )
, aveQueueLength( addOutputPort( "aveQueueLength" ) )
, frequence(0,0,30,0)
{
	enterCount = leaveCount = 0;
	lastEnterCount = lastLeaveCount = 0;
	checkCount = lengthTotal = 0;
	
	string time( MainSimulator::Instance().getParameter( description(), "frequence" ) ) ;
	if(time != "")
		frequence = time;
	
	freqInMin = 1.0 * frequence.asMsecs() / 60000;

}

/*******************************************************************
* Function Name: initFunction
* Description:
* Precondition:
********************************************************************/
Model &TrafficCollector::initFunction()
{
	passivate();
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: the intersection controller receive a car
********************************************************************/
Model &TrafficCollector::externalFunction( const ExternalMessage &msg )
{
	const Port &port = msg.port();
	Time e = msg.time() - lastChange();
	
	if(state()==passive)
	{
		holdIn(active, frequence);
	}
	else // if(state()== active)
	{
		if(port==entered)
		{
			enterCount++;
		}
		else // if(port == leaved)
		{
			leaveCount++;
		}
		holdIn(active, sigma-e);
	}
	
	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &TrafficCollector::internalFunction( const InternalMessage & )
{
	holdIn(active, frequence);
	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model& TrafficCollector::outputFunction( const InternalMessage &msg )
{
	checkCount++;
	lengthTotal +=  enterCount - leaveCount;
	
	sendOutput(msg.time(), throughput, (leaveCount-lastLeaveCount)/freqInMin);
	sendOutput(msg.time(), curQueueLength, (enterCount-leaveCount));
	sendOutput(msg.time(), aveQueueLength, 1.0 * lengthTotal/checkCount );
	
	lastLeaveCount =  leaveCount;
	lastEnterCount =  enterCount;
	
	return *this;
}
