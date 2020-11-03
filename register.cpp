/*******************************************************************
*
*  DESCRIPTION: Simulator::registerNewAtomics()
*
*  AUTHOR: Amir Barylko & Jorge Beyoglonian 
*
*  EMAIL: mailto://amir@dc.uba.ar
*         mailto://jbeyoglo@dc.uba.ar
*
*  DATE: 27/6/1998
*
*******************************************************************/

#include "modeladm.h" 
#include "mainsimu.h"
#include "queue.h"      // class Queue
#include "generat.h"    // class Generator
#include "cpu.h"        // class CPU
#include "transduc.h"   // class Transducer
#include "trafico.h"    // class Trafico
#include "TrafficLightController.h"    // class TrafficLightController
#include "LightGroupController.h"    // class LightGroupController
#include "GenericQueue.h"
#include "IntersectionQueue.h"
#include "IntersectionController.h"
#include "TrafficCollector.h"

void MainSimulator::registerNewAtomics()
{
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Queue>() , "Queue" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Generator>() , "Generator" ) ;
//	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<CPU>() , "CPU" ) ;
//	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Transducer>() , "Transducer" ) ;
//	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Trafico>() , "Trafico" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<LightGroupController>() , "LightGroupController" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<TrafficLightController>() , "TrafficLightController" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<GenericQueue>() , "GenericQueue" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<IntersectionQueue>() , "IntersectionQueue" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<IntersectionController>() , "IntersectionController" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<TrafficCollector>() , "TrafficCollector" ) ;
}
