// File: process.h
// Author: Sharath Nagendra
// Declares the variables and member functions required to fork the new process. 
// Fork, Yield, Sleep, Terminate functions for the process are also defined here. 
// Getter ans setter funcrtions for the private member variables of the class are also defined here. 

#pragma once
#include "utility.h"
#include "threadscheduler.h"
#include "thread.h"
#include <string>

extern int threadID;
extern int pid;

using namespace std; 

enum ProcStatus {JustCreated, Running, Ready, Blocked}; // Process states 

class Process{
	private:
		int procid;
		ProcStatus stat; 
		VoidFunctionPtr fun;
	public:
		Process(char* name, int prior); // Process Constructor
		~Process(); //Process destructor
		int prior; // priority of the process

		//setters and getters for processID
		void setProcId(int procid);
		int getProcId();

		//setters and getters for Process status
		void setStatus(ProcStatus s);
		ProcStatus getStatus();


		void Fork(VoidFunctionPtr func, void *arg); // fork method to fork a new process
		void Yield(); //Method to reliquish the CPU to any other runnable processes
		void Sleep(bool finishing); // Method to put the Process to sleep to reliquish the cpu to any other runnable process
		void Terminate(); // Method to terminate the process
		void ThreadFork(int numThreads, VoidFunctionPtr fun); // Method to fork a new thread from the process 
		void forkChildProc(); //
		ThrdScheduler *sh;
		Thread *currentThread;
		List<Thread *> *forkedThreadList; //List to hold the forked threads
};
