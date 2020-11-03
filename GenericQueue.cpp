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

/** include files **/
#include "GenericQueue.h"      // class GenericQueue
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

/** public functions **/

/*******************************************************************
* Function Name: GenericQueue
* Description: 
********************************************************************/
GenericQueue::GenericQueue( const string &name )
: Atomic( name )
, in( addInputPort( "in" ) )
, done( addInputPort( "done" ) )
, stop_in( addInputPort( "stop_in" ) )
, stop_out( addInputPort( "stop_out" ) )
, out( addOutputPort( "out" ) )
, next( addOutputPort( "next" ) )
, block( addOutputPort( "block" ) )
, preparationTime( 0, 0, 2, 0 )
, responseTime(0, 0, 0, 0)
, N(0)
, input(true)
, output(true)
, pending(false)
, action(0)
, sendingTimeLeft(Time::Inf)
{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;
	if( time != "" )
		preparationTime = time ;

	string para1( MainSimulator::Instance().getParameter( description(), "capacity" ) ) ;
	if( para1 != "" )
		N = atoi(para1.data());
	if(N<0) N=0;

	string para2( MainSimulator::Instance().getParameter( description(), "input" ) ) ;
	if( para2 == "disable" )
		input = false;
	else 
		input = true;

	string para3( MainSimulator::Instance().getParameter( description(), "output" ) ) ;
	if( para3 == "disable" )
		output = false;
	else
		output = true;
		
	

}

/*******************************************************************
* Function Name: initFunction
* Description: 
* Precondition:
********************************************************************/
Model &GenericQueue::initFunction()
{
	elements.erase( elements.begin(), elements.end() ) ;
	if(input==false)
	{
		action = BLOCKING;
		holdIn(active, responseTime);
	}
	else
	{
		passivate();
	}
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &GenericQueue::externalFunction( const ExternalMessage &msg )
{
	const Port &port = msg.port();
	int n = elements.size();
	bool was_full = full();
	Time e = msg.time() - lastChange();
	bool wasSending = ((action & SENDING) ? true: false);
	char* inputPort = "unknown";
	
	if(port == in)
	{
		inputPort = "in";
		if(!was_full)
		{
			elements.push_back(msg.value());
		}
		else
		{
			printf("%s: queue overflowed \n", description().data());
		}
		if(n==0 && output==true)
		{
			action |= SENDING;
			if(!wasSending)
			{
				sendingTimeLeft = preparationTime;
			}
		}
		if(input==false || full())
		{
			action |= BLOCKING;	action &= ~NEXTING;
		}
		else
		{
			action &= ~BLOCKING; action |= NEXTING;
		}
		
	}
	else if(port==done)
	{
		inputPort = "done";
		if(n>0 && pending==true)
		{
			elements.pop_front();
			pending = false;
		}
		if(!elements.empty())
		{
			action |= SENDING;
			if(!wasSending)
			{
				sendingTimeLeft = preparationTime;
			}
		}
		if(input==true && was_full)
		{
			action &= ~BLOCKING; action |= NEXTING;
		}
		output = true;
	}
	else if(port==stop_out)
	{
		inputPort = "stop_out";
		if(output==true)
		{
			output = false;
			action &= ~SENDING;
			sendingTimeLeft = Time::Inf;
		}
	}
	else if(port==stop_in)
	{
		inputPort = "stop_in";
		if(msg.value()==0) // stop_in==0
		{
			if(input==false)
			{
				if(!was_full)
				{
					action &= ~BLOCKING; action |= NEXTING;
				}
				input = true;
			}
		}
		else // stop_in==1
		{
			if(input==true)
			{
				if(!was_full)
				{
					action |= BLOCKING; action &= ~NEXTING;
				}
				input = false;
			}
		}
	}

//	printf("%s(%s): X / %s / %s = %f / action = %d / size = %d\n",
//			msg.time().asString().data(), e.asString().data(), description().data(),
//			inputPort, msg.value(), action, elements.size());

	if(action==0)
	{
		passivate();
	}
	else
	{
		if(wasSending && (action & SENDING))
		{
			sendingTimeLeft -= e;
		}
//		if((action & SENDING))
//		{
//			printf("%s(%s): X %s  sendingTimeLeft=%s\n",
//				msg.time().asString().data(), e.asString().data(),
//				description().data(), sendingTimeLeft.asString().data());
//			
//		}
			
		if(action & (BLOCKING | NEXTING))
		{
			if(state()==active && (sigma-e)<responseTime)
				holdIn(active, sigma-e);
			else
				holdIn(active, responseTime);
		}
		else // if (action & SENDING)
		{
			holdIn(active, sendingTimeLeft);
		}
	}

	//printf("%s: external action = %d\n", description().data(), action);	
	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &GenericQueue::internalFunction( const InternalMessage &msg )
{
//	Time e = msg.time() - lastChange();

//	if(action & SENDING)
//	{
//		printf("%s(%s): I %s  sendingTimeLeft=%s\n",
//			msg.time().asString().data(), e.asString().data(),
//			description().data(), sendingTimeLeft.asString().data());
//			
//	}
		
	if(action & (NEXTING | BLOCKING))
	{
		action &= ~(NEXTING | BLOCKING);
		if(action & SENDING)
		{
			holdIn(active, sendingTimeLeft);
		}
		else
		{
			passivate();
		}
	}
	else if(action & SENDING)
	{
		//action &= ~SENDING;
		action = 0;
		passivate();
	}
	else
	{
		passivate();
	}
//	printf("%s(%s): I / %s / action = %d / size = %d\n",
//			msg.time().asString().data(), e.asString().data(),
//			description().data(), action, elements.size());

	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description: 
********************************************************************/
Model &GenericQueue::outputFunction( const InternalMessage &msg )
{
	Time e = msg.time() - lastChange();

	if(action & SENDING)
	{
		sendingTimeLeft = sendingTimeLeft - e;
//		printf("%s(%s): Y %s  sendingTimeLeft=%s\n",
//			msg.time().asString().data(), e.asString().data(),
//			description().data(), sendingTimeLeft.asString().data());
			
	}

	if((action & (NEXTING | BLOCKING))!=0)
	{
		if(action & NEXTING)
		{
			sendOutput(msg.time(), next, 1);
//			printf("%s(%s): Y / %s / next = 1 / action = %d / size = %d\n",
//				msg.time().asString().data(), e.asString().data(),
//				description().data(), action, elements.size());
		}
		if(action & BLOCKING)
		{
			sendOutput(msg.time(), block, 1);
//			printf("%s(%s): Y / %s / block = 1 / action = %d / size = %d\n",
//				msg.time().asString().data(), e.asString().data(),
//				description().data(), action, elements.size());
		}
	}
	else if((action & SENDING)!=0)
	{
		sendOutput(msg.time(), out, elements.front());
		pending = true;
		sendingTimeLeft = Time::Inf;
//		printf("%s(%s): Y / %s / out = %f / action = %d / size = %d\n",
//				msg.time().asString().data(), e.asString().data(), description().data(),
//				elements.front(), action, elements.size());
	}
	else
	{
		printf("%s: error state %d !!\n", description().data(), action);
	}
	return *this ;
}
