// scheduler.h 
//	Data structures for the process dispatcher and scheduler.
//	Primarily, the list of processes that are ready to run.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "copyright.h"
#include "list.h"
#include "process.h"
#include "thread.h"

// The following class defines the scheduler/dispatcher abstraction -- 
// the data structures and operations needed to keep track of which 
// process is running, and which process are ready but not running.

class Scheduler {
  public:
    Scheduler();		// Initialize list of ready threads 
    ~Scheduler();		// De-allocate ready list

    void ReadyToRun(Process* proc);
    void ReadyToRun(Thread* thread);	
    
    Process* FindNextProcToRun(); // removes the first element from the list	
    void Run(Process* nextProcess, bool finishing); //Causes the next process to start running 				
    void CheckToBeDestroyed(); //Checks whether the process that has been running needs to be destroyed   
    SortedList<Process *> *readyProcList;
    List<Process *> *processList;    
  private:
    Process *toBeDestroyed;	//holds the process that needs to be destroyed 
};

#endif
