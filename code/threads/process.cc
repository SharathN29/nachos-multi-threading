#pragma once
#include "switch.h"
#include "synch.h"
#include "sysdep.h"
#include <string>
#include "process.h"

int threadID = rand()%50;
int pid = rand()%50+50;
using namespace std;

// Process contructor, assigns the priority and sets the state of the process 
Process::Process(char* name, int prior){
	this->prior = prior;
	this->stat = JustCreated;
	this->procid = pid++;
	currentThread = new Thread("Main thread created");
	sh = new ThrdScheduler();
}

// Process destructor, deletes the currentThread of the process 
Process::~Process(){
	delete(currentThread);
}

// sets the process id
void
Process::setProcId(int id){
	procid = id;
}

// getter for processId 
int
Process::getProcId(){
	return procid; 
}

//setter to set the status of the process
void 
Process::setStatus(ProcStatus s){
	stat = s;
}

// getter to get the status of the process
ProcStatus
Process::getStatus(){
	return stat;
}

// Function to fork a new process, we append the newly forked process to a list
// and set the status to ready and add the process to the ready to run list. 
void
Process::Fork(VoidFunctionPtr func, void *args){
	Interrupt *interrupt = kernel->interrupt;
	IntStatus oldLevel; 
	fun = func; 

	this->currentThread->Fork(func, (void *) args);

	kernel->interrupt->SetLevel(IntOff);
	sh->FindNextToRun();
	kernel->interrupt->SetLevel(IntOn);
	kernel->scheduler->processList->Append(this);

	oldLevel = interrupt->SetLevel(IntOff);
	kernel->scheduler->ReadyToRun(this);

	(void) interrupt->SetLevel(oldLevel);	
	std::cout << "Process with processID "<<procid<<" and priority " << prior <<" has been forked " << endl;
}

// Function to yield a process, reliquishes the CPU to the next runnable process. 
void
Process::Yield(){
	Process *nextProcess;
	IntStatus oldLevel = kernel->interrupt->SetLevel(IntOff);

	ASSERT(this == kernel->currentProcess);

	nextProcess = kernel->scheduler->FindNextProcToRun();
	if(nextProcess != NULL){
		kernel->scheduler->ReadyToRun(this);
		kernel->scheduler->Run(nextProcess, FALSE);
	}
	(void) kernel->interrupt->SetLevel(oldLevel);
}

// forks the child process for the process. The child process will have the same id and priority as the parent process 
void
Process::forkChildProc(){
	cout<<"Child ";
	Process *childProc = new Process("c",prior);
	childProc->Fork((VoidFunctionPtr)fun, (void *) 3);
}

// forks the threads of each of the processes
void
Process::ThreadFork(int num, VoidFunctionPtr fun){
	Thread *t[num+5];
	for(int i=1; i <= num; i++){
		t[i] = new Thread("New thread forked");
		t[i]->Fork((VoidFunctionPtr) fun, (void *) threadID);
		threadID++;
	}
	kernel->interrupt->SetLevel(IntOff);
	kernel->currentProcess->currentThread->Yield();
	kernel->interrupt->SetLevel(IntOn);
}
   
// puts the sleep into sleep and reliquishes the cpu to the other runnable processes
void
Process::Sleep(bool finishing){
	Process *nextProcess;

	ASSERT(this == kernel->currentProcess);
	ASSERT(kernel->interrupt->getLevel() == IntOff);

	stat = Blocked; 
	this->currentThread->status = BLOCKED;
	while((nextProcess = kernel->scheduler->FindNextProcToRun()) == NULL)
	kernel->interrupt->Idle();

	kernel->scheduler->Run(nextProcess, finishing);
}

// Terminates the process 
void
Process::Terminate(){
	(void) kernel->interrupt->SetLevel(IntOff);
	ASSERT(this == kernel->currentProcess);
	cout<<"\nProcess with priority "<<prior << " and process ID "<<getProcId()<<" is being terminated\n";
	Sleep(TRUE);
}
