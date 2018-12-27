#ifndef THREADSCHEDULER_H
#define THREADSCHEDULER_H

#include "list.h"
#include "thread.h"
#include "copyright.h"

class ThrdScheduler{
	public:
		ThrdScheduler();
		~ThrdScheduler();

		void ReadyToRun(Thread* thread);
		Thread* FindNextToRun();
		void Run(Thread* nextThread, bool finishing);
		void CheckToBeDestroyed();
		void Print();
		List<Thread *> *readyList;

	private:
		Thread *toBeDestroyed;
};

#endif 
