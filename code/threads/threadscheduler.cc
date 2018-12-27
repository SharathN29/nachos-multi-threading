#include "debug.h"
#include "scheduler.h"
#include "main.h"
#include "copyright.h"

// Initialize the list of ready but not running threads.
// Initially, no ready threads.
ThrdScheduler::ThrdScheduler(){
	readyList = new List<Thread *>;
	toBeDestroyed = NULL;
}

// De-allocate the list of ready threads.
ThrdScheduler::~ThrdScheduler(){
	delete readyList;
}

// Mark a thread as ready, but not running.
//Put it on the ready list, for later scheduling onto the CPU.
//"thread" is the thread to be put on the ready list.
void
ThrdScheduler::ReadyToRun(Thread *thread){
	ASSERT(kernel->interrupt->getLevel() == IntOff);
	DEBUG(dbgThread, "Putting thread to ready list: "<< thread->getName());
		
	thread->setStatus(READY);
	readyList->Append(thread);
}

// Return the next thread to be scheduled onto the CPU.
//If there are no ready threads, return NULL.
// Side effect:
//Thread is removed from the ready list.
Thread*
ThrdScheduler::FindNextToRun(){
	ASSERT(kernel->interrupt->getLevel() == IntOff);
	
	if(readyList->IsEmpty()){
		return NULL;
	}
	else{
		return readyList->RemoveFront();
	}
}

// Dispatch the CPU to nextThread.  Save the state of the old thread,
//and load the state of the new thread, by calling the machine
//dependent context switch routine, SWITCH.
//"nextThread" is the thread to be put into the CPU.
//"finishing" is set if the current thread is to be deleted
//once we're no longer running on its stack
//(when the next thread starts running)
void
ThrdScheduler::Run(Thread *nextThread, bool finishing){
	Thread *oldThread = kernel->currentProcess->currentThread;
	ASSERT(kernel->interrupt->getLevel() == IntOff);

	if(finishing){
		ASSERT(toBeDestroyed == NULL);
		toBeDestroyed = oldThread;
	}
	
	if (oldThread->space != NULL) {     
        	oldThread->SaveUserState();     
        	oldThread->space->SaveState();
    	}

	oldThread->CheckOverflow();

	kernel->currentProcess->currentThread = nextThread;
	nextThread->setStatus(RUNNING);

	DEBUG(dbgThread, "Switching From: " <<oldThread->getName() << "to: " << nextThread->getName());
	
	SWITCH(oldThread, nextThread);
	ASSERT(kernel->interrupt->getLevel() == IntOff);
	DEBUG(dbgThread, "Now in thread: " << oldThread->getName());

	CheckToBeDestroyed();

	if(oldThread->space != NULL) {        
        	oldThread->RestoreUserState();     
        	oldThread->space->RestoreState();
    	}
}

// If the old thread gave up the processor because it was finishing,
// we need to delete its carcass.  Note we cannot delete the thread
// before now (for example, in Thread::Finish()), because up to this
// point, we were still running on the old thread's stack!
void
ThrdScheduler::CheckToBeDestroyed()
{
    if (toBeDestroyed != NULL) {
        delete toBeDestroyed;
        toBeDestroyed = NULL;
    }
}

// Print the scheduler state -- in other words, the contents of
// the ready list.  For debugging.
void
ThrdScheduler::Print()
{
    cout << "Ready list contents:\n";
    readyList->Apply(ThreadPrint);
}
