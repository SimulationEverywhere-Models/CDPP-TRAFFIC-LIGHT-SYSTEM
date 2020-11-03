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

/** include files **/
#include "IntersectionQueue.h"      // class IntersectionQueue
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

/** public functions **/

/*******************************************************************
* Function Name: IntersectionQueue
* Description:
********************************************************************/
IntersectionQueue::IntersectionQueue( const string &name )
: Atomic( name )
, in( addInputPort( "in" ) )
, done( addInputPort( "done" ) )
, stop_in( addInputPort( "stop_in" ) )
, stop_out( addInputPort( "stop_out" ) )
, out( addOutputPort( "out" ) )
, next( addOutputPort( "next" ) )
, block( addOutputPort( "block" ) )
, preparationTime( 0, 0, 0, 0 )
, responseTime(0, 0, 0, 0)
, N(0)
, input(false)
, output(true)
, pending(false)
, action(0)
, crossingTime(2)
{
	//string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;
	//if( time != "" )
	//	preparationTime = time ;

	string crosstime( MainSimulator::Instance().getParameter( description(), "crossing" ) ) ;
	if( crosstime != "" )
		crossingTime = Time(crosstime).asMsecs() / 1000.0;
		
//	printf("crossing time = %f\n", crossingTime);
	
	string para1( MainSimulator::Instance().getParameter( description(), "capacity" ) ) ;
	if( para1 != "" )
		N = atoi(para1.data());

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
Model &IntersectionQueue::initFunction()
{
	elements.erase( elements.begin(), elements.end() ) ;
	sendingTimeLeft = Time::Inf;
	if(input==false)
	{
		action = BLOCKING;
		holdIn(active, responseTime);
	}
	else
	{
		action = NEXTING;
		holdIn(active, responseTime);
	}
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &IntersectionQueue::externalFunction( const ExternalMessage &msg )
{
	const Port &port = msg.port();
	unsigned int n = elements.size();
	bool was_full = full();
	Time e = msg.time() - lastChange();
	bool wasSending = (action & SENDING) ? true: false;

	double e_as_seconds = e.asMsecs() / 1000.0;
	
	char* inputPort = "unknown";

	if(port == in)
	{
		inputPort = "in";
		if(!was_full)
		{
			elements.push_back(Pair((int)msg.value(), crossingTime) );
		}
		else
		{
			printf("%s: overflowed (%f) \n", description().data(), msg.value());
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
		if(n>0 && pending)
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

	// reduce service time for all items in list excluing the new one.
//	printf("%s(%s): / %s / %s = %f / action = %d / size = %d\n",
//			msg.time().asString().data(), e.asString().data(), description().data(),
//			inputPort, msg.value(), action, elements.size());

	if(elements.size()>0)
	{
		unsigned int count = 0;
		ElementList::reverse_iterator ite;
		for(ite=elements.rbegin(); ite!=elements.rend(); ite++)
		{
			Pair &pair = *ite;
//	 		printf("\titem[%d] = (%d, %f)", count, pair.id, pair.time);
			if(port!=in || count>0 || n>=elements.size())
			{
				pair.time -= e_as_seconds;
			}
//		 	printf(" --> (%d, %f)", pair.id, pair.time);
			if(pair.time<0)
			{
	  			 pair.time = 0;
//				 printf(" --> (%d, %f)", pair.id, pair.time);
			}
			if(pending==true && count==elements.size()-1)
			{
//				printf("*");
			}
//			printf("\n");
			count++;
		}
	}		
	
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
		if(action & (BLOCKING | NEXTING))
		{
			if(state()==active && (sigma-e) < responseTime)
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
Model &IntersectionQueue::internalFunction( const InternalMessage &msg )
{
	if((action & (NEXTING | BLOCKING)) != 0)
	{
		action &= ~NEXTING;
		action &= ~BLOCKING;
		if(action & SENDING)
		{
			holdIn(active, preparationTime);
		}
		else
		{
			passivate();
		}
	}
	else if((action & SENDING) != 0)
	{
		action &= ~SENDING;
		passivate();
	}
	else
	{
		passivate();
	}
		
	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &IntersectionQueue::outputFunction( const InternalMessage &msg )
{
	Time e = msg.time() - lastChange();
	unsigned int n = elements.size();
	double e_as_seconds = e.asMsecs() / 1000.0;

	if(n>0)
	{
		unsigned int count = 0;
		ElementList::reverse_iterator ite;
		for(ite=elements.rbegin(); ite!=elements.rend(); ite++)
		{
			Pair &pair = *ite;
//		 	printf("\titem[%d] = (%d, %f)", count, pair.id, pair.time);
			pair.time -= e_as_seconds;
//		 	printf(" --> (%d, %f)", pair.id, pair.time);
			if(pair.time<0)
			{
				printf("%s: negative service time (%d, %f) !!\n",
					    description().data(), pair.id, pair.time);
	  			pair.time = 0;
//				printf(" --> (%d, %f)", pair.id, pair.time);
			}
			if(pending==true && count==n-1)
			{
//				printf("*");
			}
//			printf("\n");
			count++;
		}
	}		
	
	if(action & SENDING)
	{
		sendingTimeLeft -= e;
	}
	
	if((action & (NEXTING | BLOCKING))!=0)
	{
		if(action & NEXTING)
		{
			sendOutput(msg.time(), next, 1);
//			printf("%s(%s): / %s / next = 1 / action = %d / size = %d\n",
//				msg.time().asString().data(), e.asString().data(),
//				description().data(), action, elements.size());
		}
		if(action & BLOCKING)
		{
			sendOutput(msg.time(), block, 1);
//			printf("%s(%s): / %s / block = 1 / action = %d / size = %d\n",
//				msg.time().asString().data(), e.asString().data(),
//				description().data(), action, elements.size());
		}
	}
	else if((action & SENDING)!=0)
	{
		sendOutput(msg.time(), out, elements.front().time);
		pending = true;
		sendingTimeLeft = Time::Inf;
//		printf("%s(%s): / %s / out = %f / action = %d / size = %d\n",
//				msg.time().asString().data(), e.asString().data(),
//				description().data(), elements.front().time, action, elements.size());
	}
	else
	{
		printf("%s: error state %d !!\n", description().data(), action);
	}
	//printf("%s: outout action = %d\n", description().data(), action);	
	return *this ;
}
