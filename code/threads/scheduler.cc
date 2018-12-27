// scheduler.cc 
//	Routines to choose the next process to run, and to dispatch to
//	that process.
//
// 	These routines assume that interrupts are already disabled.
//	If interrupts are disabled, we can assume mutual exclusion
//	(since we are on a uniprocessor).
//
// 	NOTE: We can't use Locks to provide mutual exclusion here, since
// 	if we needed to wait for a lock, and the lock was busy, we would 
//	end up calling FindNextToRun(), and that would put us in an 
//	infinite loop.
//
// 	Very simple implementation -- no priorities, straight FIFO.

#include "copyright.h"
#include "debug.h"
#include "scheduler.h"
#include "main.h"

// comparision function to sort the processes based on their priority 
int PriorComp(Process *p1, Process *p2){
	if(p1->prior > p2->prior) return -1; 
		else if(p1->prior < p2->prior) return 1;
		else return 0;
}

// Initializes the the list of ready but not running processes
Scheduler::Scheduler()
{ 
    processList = new List<Process *>;
    readyProcList = new SortedList<Process *>(PriorComp);
    toBeDestroyed = NULL;
} 

// deallocates the list of ready processes
Scheduler::~Scheduler()
{ 
    delete readyProcList; 
} 


void
Scheduler::ReadyToRun (Thread *thread){

}


//Marks the status of the process as ready, but not running and puts them in the ready process list
void
Scheduler::ReadyToRun(Process *proc){
	ASSERT(kernel->interrupt->getLevel() == IntOff);
	
	proc->setStatus(Ready);
	readyProcList->Insert(proc);
}


// returns the next process from the sorted list of processes which is sorted based on priority
//If there are no processes to run NULL will be returned
Process *
Scheduler::FindNextProcToRun(){
	ASSERT(kernel->interrupt->getLevel() == IntOff);
	if(readyProcList->IsEmpty()){
		return NULL;
	} 
	else{
		return readyProcList->RemoveFront();
	}
}

// Allocates the next ready process to the CPU, and save the state of the old peocess and load the state of the new process. 
// nextProcess indicates the process which is scheduled to run next and finishing is set if the current process is to be deleted after the execution 
void
Scheduler::Run(Process *nextProcess, bool finishing){
	Process *oldProcess = kernel->currentProcess;
	Thread *oldThread = oldProcess->currentThread;

	ASSERT(kernel->interrupt->getLevel() == IntOff);

	if(finishing){
		ASSERT(toBeDestroyed == NULL);
		toBeDestroyed = oldProcess;
	}

	if(oldThread->space != NULL){
		oldThread->SaveUserState();
		oldThread->space->SaveState();
	}
	
	oldThread->CheckOverflow();

	Thread *nextThread = nextProcess->currentThread;
	kernel->currentProcess = nextProcess;
	
	nextProcess->setStatus(Running);
	nextThread->setStatus(RUNNING);

	SWITCH(oldThread, nextThread);

	ASSERT(kernel->interrupt->getLevel() == IntOff);

	CheckToBeDestroyed();

	if(oldThread->space != NULL){
		oldThread->RestoreUserState();
		oldThread->space->RestoreState();
	}
}


// if the process has gave-up the CPU because it is finishing, we delete that particular process
void
Scheduler::CheckToBeDestroyed()
{
    if (toBeDestroyed != NULL) {
        delete toBeDestroyed;
	toBeDestroyed = NULL;
    }
} 
