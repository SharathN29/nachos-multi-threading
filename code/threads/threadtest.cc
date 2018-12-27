#pragma once
#include "kernel.h"
#include "main.h"
#include "thread.h"

using namespace std;

void
printFunc(int n){
	cout<<"Thread with ID "<< n << "  is currently running" <<endl;
}

void
SimpleThread(int which)
{	
	cout<<endl;
	cout<<"Process with process ID "<<kernel->currentProcess->getProcId()<<" and priority "<< kernel->currentProcess->prior << " is currently running\n";
	cout<<"---Forking "<<which<<" threads of process with priority "<<kernel->currentProcess->prior<<" and process ID "<<kernel->currentProcess->getProcId()<<"---"<<endl;

	kernel->currentProcess->ThreadFork(which, (VoidFunctionPtr) printFunc);
	kernel->interrupt->SetLevel(IntOff);
	 
	cout<<"\nProcess with priority "<< kernel->currentProcess->prior <<" and process ID "<<kernel->currentProcess->getProcId()<< " is currently yielding\n";

	kernel->currentProcess->Yield();
	kernel->currentProcess->Terminate();
   
    
    
}

void
ThreadTest()
{
	Process *p1 = new Process("Forked Process 1", 2);
	Process *p2 = new Process("Forked Process 2", 4);
	Process *p3 = new Process("Forked Process 3", 5);

	cout<<"******************Implementing multi-threading capabilities in nachos******************\n";
	cout<< "Forking all the processes " <<endl;
	cout<<"---------------------------"<<endl;
	p1->Fork((VoidFunctionPtr) SimpleThread, (void *) 2);
	p2->Fork((VoidFunctionPtr) SimpleThread, (void *) 2);
	p3->Fork((VoidFunctionPtr) SimpleThread, (void *) 3);
	
	p2->forkChildProc();
	p3->forkChildProc();

	kernel->currentProcess->Yield();
	kernel->interrupt->SetLevel(IntOff);
	
	while(kernel->scheduler->readyProcList->NumInList()>0){
		kernel->currentProcess->Yield();
	}	
	kernel->interrupt->SetLevel(IntOn);
}
