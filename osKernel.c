#include "osKernel.h"
#include <stdint.h>


#define NUM_OF_THREADS 3
#define STACK_SIZE 100


#define BUS_FREQ 16000000
uint32_t MILIS_PRESCALER;

void osSchedulerLaunch(void);


struct tcb{                                //conventional name of tcb
	int32_t *stackPt;
	struct tcb *nextPt;
};

typedef struct tcb tcbType;

tcbType tcbs[NUM_OF_THREADS];
tcbType *currentPt;

int32_t TCB_STACK[NUM_OF_THREADS][STACK_SIZE];              //stack

void osKernelStackInit(int i){                              // i is the thread number
	tcbs[i].stackPt = &TCB_STACK[i][STACK_SIZE-16];           //to make the sp points to the top of the stack of the same thread
	TCB_STACK[i][STACK_SIZE-1] = 0x01000000;                  // set bit 24 of the XPSR register to make the processor runs at the thumb mode
	
}


uint8_t osKernelAddThreads(void(*task0)(void),void(*task1)(void),void(*task2)(void))
{ //set the arrangement of the threads execution -circular linked list-
	__disable_irq();
	tcbs[0].nextPt = &tcbs[1];
	tcbs[1].nextPt = &tcbs[2];
	tcbs[2].nextPt = &tcbs[0];
	
	//initialze the stack of thread0
	osKernelStackInit(0);
	TCB_STACK[0][STACK_SIZE-2] = (int32_t)(task0);  //make the pc points to the function of task0
	
		//initialze the stack of thread1
	osKernelStackInit(1);
	TCB_STACK[0][STACK_SIZE-2] = (int32_t)(task1);  //make the pc points to the function of task0
	
		//initialze the stack of thread2
	osKernelStackInit(2);
	TCB_STACK[0][STACK_SIZE-2] = (int32_t)(task2);  //make the pc points to the function of task0
  currentPt = &tcbs[0];
	__enable_irq();
	return 1;
}

void osKernelInit(void)
{
		__disable_irq();
	MILIS_PRESCALER = BUS_FREQ / 1000;

}


void osKernelLaunch(uint32_t quanta)
{
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
	SysTick->LOAD = (quanta*MILIS_PRESCALER)-1;
	SysTick->CTRL = 0X00000007;
	osSchedulerLaunch();
}


