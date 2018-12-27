# nachos-multi-threading-
Implementing multi-threading and priority based scheduling to Nachos 

1.	Objective
We are adding multi-threading capabilities to Nachos. From Unix point of
View Nachos is an operating system without multi-threading capabilities. We create a Process class and integrate the Process class with the Nachos Thread and System. Kernel schedules the processes and the processes in turn schedules their threads. Kernel has will have no control over the threads. The processes will be scheduled based on their priority using Priority-based scheduling scheduler. 

2.	What has been developed 
Following files has been newly created 
•	process.h
•	process.cc 
•	scheduler.h
•	scheduler.cc
•	threadtest.cc 
•	threadscheduler.h
•	threadscheduler.cc

Following files has been modified 
•	kernel.h
•	kernel.cc
•	thread.h
•	thread.cc
•	main.cc
•	timer.h
•	timer.cc
•	alarm.h
•	alarm.cc

process.h
The file declares the Process class. It has the data structure to manage the threads. Declares the private member variable and member functions require for the class. It also declares the setter and getter functions to set and get the private member variables of the class. It also declares the Fork, Yield, Sleep, Terminate the process. An enum variable is defined to store the process states. The process states are JustCreated, Running, Ready and Blocked. 

process.cc
•	This file gives the definition of all the member functions that were declared in process.h file. It defines the getters and setters for the private member variable of the class. 
•	The process constructor assigns the value for priority, status and processId of the process. The Process destructor deletes the process’s thread which calls it.  
•	The Process::Fork method forks a new process, appends it into  the processList and marks the process as ready to run. 
•	The Process::Yield method reliqueshes the CPU to the nextProcess,. 
•	The Process::forkChildProc creates a new child process instance and calls fork method from that instance. 
•	Process::ThreadFork forks off threads from the current process, after creating and instatiating each thread it’ll yield to create new thread of that process. 
•	The Process::Sleep function puts the current process into Idle state and runs the next process from the process list based on priority. 
•	The Process::Terminate function simply calls the Sleep function which pus the current process into Idle state. 

scheduler.h
Defines the scheduler class and all the member functions required to implement the process scheduling. Also defines a process list which has all the processes which are forked. A ready processes list is also defined which is of type SortedList which stores the processes based on their priority.  

scheduler.cc 
•	Provides the implementation for the process scheduler. 
•	The scheduler constructor creates a new processList which holds the processes that are forked and also creates a ready process list which is a sorted list which holds all the processes in a sorted order. 
•	The scheduler destructor deletes the ready process list. 
•	The ReadyToRun method marks each processes status as Ready and inserts the process into a ready process list. The ready process list is of type sorted list, so it compares the priorities of the processes and sorts it in the order of their priority. The FindNextProcToRun method checks if the ready process list is empty, if not it returns the front of the list which has the process with highest priority.  
•	The Run method allocates the next ready process to the CPU and save the state of the old process and load the state of the new process. 
•	The CheckToBeDestroyed function checks if any of the processes is to be destroyed, if so that process will be destroyed. 

threadtest.cc
•	Provides the test cases to prove the multi-threading capabilities of the nachos. The SimpleThread function begins the execution with the main thread and forks off processes and their child processes and Yields for the first process. 
•	When the first process begins its execution, it forks off its threads and runs them, then the first process yields for the second process. Once the second has Yielded the first process runs again since it has higher priority and terminates. 
•	This continues until all the processes and its child processes complete its execution. 

threadscheduler.h
•	This file declares what the scheduler.h used to define in a single threaded nachos. Just the declaration of Thread scheduling methods like ReadyToRun, FindNextToRun, Run, CheckToBeDestroyed. 
•	It also maintains a list called readyList which has all the threads which are in the Ready state. 

threadscheduler.cc
•	Implements the functions that were defined in threadscheduler.h which schedules the threads. This file is similar to that of scheduler.cc from single-threaded nachos. 
•	The ReadyToRun method puts the forked thread in the ready state and appends them into a readyList. 
•	The FindNextToRun method removes the thread from front of the readyList as it follows Fist In First Out (FIFO) model of scheduling. 
•	The Run method dispatch the CPU to nextThread.  Save the state of the old thread, and load the state of the new thread, by calling the machine dependent context switch routine, SWITCH. 
•	The CheckToBeDestroyed method checkes whether any thread is marked to be destroyed and if so it destroys it. 

3.	How to test your solution 
Navigate to nachos/code/build.linux 
Run the following two commands: 
•	Make clean; make depend; make 
•	./nachos -K 
	
	If time quantum needs to be added, following command is run
•	make clean; make depend; make
•	./nachos -K -quantum 1000

