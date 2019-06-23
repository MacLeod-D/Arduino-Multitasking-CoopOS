// (C) 2019 Helmut Weber 

#include "Task.h"

#define Delay(n) Yield(n)

uint8_t Flag = 0;

#ifdef WDT
volatile unsigned int oldSwitchCount=55;
ISR(TIMER1_COMPA_vect)
{
extern bool YActive;
  //BITSETD4;           // for WDT_VALUE=5  test 
  //Flag=1;
  //BITCLEARD4;
  return;
  if (YActive) {   oldSwitchCount=SwitchCount; return;   }    // os is working: nothing to test
  if (SwitchCount==oldSwitchCount) { // multitask not working
    TIMSK1 = 0;
    Serial.print(F("\n\nSP 0x")); Serial.println(Tasks[current_task].sp_save,HEX);
    Serial.print(F("ID ")); Serial.println(current_task);
    Serial.println(F("ERROR WDT: no Yields !"));
    Serial.flush();
    sei();
    while(1);
  }
  oldSwitchCount=SwitchCount;
}
#endif

/*
CS12  CS11  CS10  Mode Description
0      0     0    Stop Timer/Counter 1
0      0     1    No Prescaler (Timer Clock = System Clock)
0      1     0    divide clock by 8
0      1     1    divide clock by 64
1      0     0    divide clock by 256
1      0     1    divide clock by 1024
*/



// TRACE ON and all other enabled:
// 500 * 500 ns == 250 µs
// #define WDT_VALUE 499


// TRACE ON disabled, all other enabled:
// 379 * 0.5 µs == 185 µs
// #define WDT_VALUE 379


// TRACE ON disabled, SHOWST disabled:
// 320 * 0.5 µs == 160 µs
// #define WDT_VALUE 319


// Only Task1 and Task2 are running
// 120 * 0.5 µs == 60 µs


/** @brief StartMultiTasking
 *  
 *  All preparations are done now and the multitasking begins
 */

/* clock divider 8* =~ 500 ns */
#ifdef WDT
#define StartMultiTasking()  \
  Serial.println(F("WDT enabled"));\
  SP=STACK;\
  cli();\
  TCCR1A = 0; \
  TCCR1B = bit(WGM12) | bit(CS11);\
  OCR1A =  WDT_VALUE; \
  TIMSK1 = bit (OCIE1A); \ 
  sei();\
  Tasks[0].function ();  

  
#else           // declared tasks. Idle will never stop 

#define StartMultiTasking() \
  SP=STACK;\
  Tasks[0].function();                        // Start multitasking with Idle
#endif






/**********************************************************************
*
*  Save / Restore registers
*
**********************************************************************/

/*  As found in ChibiOS et al. */
#define pushall() \
  asm volatile ("push r2  \n\t push r3  \n\t push r4  \n\t push r5  \n\t push r6  \n\t push r7  \n\t push r8  \n\t push r9  \n\t push r10 \n\t" \
                "push r11 \n\t push r12 \n\t push r13 \n\t push r14 \n\t push r15 \n\t push r16 \n\t push r17 \n\t push r28 \n\t push r29");

#define popall() \
asm volatile ("pop r29 \n\t pop r28 \n\t pop r17 \n\t pop r16 \n\t pop r15 \n\t pop r14 \n\t pop r13 \n\t pop r12 \n\t pop r11 \n\t" \
                "pop r10 \n\t pop r9  \n\t pop r8  \n\t pop r7  \n\t pop r6  \n\t pop r5  \n\t pop r4  \n\t pop r3  \n\t pop r2"); 


/**********************************************************************
*
* Check free Stack
*
**********************************************************************/

uint16_t memSearch(uint8_t *startp, uint8_t *endp, uint8_t v) 
{ uint8_t *ptr = startp;
  while (ptr < endp) if (*ptr++ != v) break;
  return ((uint16_t)ptr-(uint16_t)startp-1); 
}

#define stackFree(task) memSearch(Tasks[task].task_stack-Tasks[task].stackLen+1,Tasks[task].sp_save,0x55)
#define myStackFree() stackFree(current_task)



/**********************************************************************
*
* Kernel
*
**********************************************************************/

// used by a task to stop itself
void stopMe() {
  Tasks[current_task].state=BLOCKED;
  Tasks[current_task].state2=BLK;
  
}


// stop tasks with id t
void stop_task (uint8_t t)
{
  Tasks[t].state = BLOCKED;
  Tasks[t].state2=BLK;
}

// resume tasks with id t
void resume_task (uint8_t t)
{
  Tasks[t].state = READY;
  Tasks[t].state2 = RDY;
}





extern bool YActive;
uint8_t oldTasks[20];
unsigned long oldMicros[20];


/**
 * @brief TaskSwitch
 * 
 * @details
 * TaskSwitch is called by Yield() [ Delay()]
 * It saves the state of the running task and switch to newer task
 */
uint8_t Sreg;
 
inline void TaskSwitch (uint8_t old, uint8_t newer)
{
struct task *TP;

  pushall();                                // save old tasks register
  Sreg=SREG;
  cli();
  Tasks[old].sp_save = SP;                  // save old tasks stackpointer
  SREG=Sreg;
  
#ifdef TRACE_ON
  for ( uint8_t i=1; i<20; i++) {
    oldTasks[i-1]=oldTasks[i];
    oldMicros[i-1]=oldMicros[i];
  }
  oldTasks[19]=newer;
  oldMicros[19]=micros();
#endif
  

  current_task=newer;
  TP=&Tasks[current_task];
  
  if (TP->new_task == true)               // a task marked as NEW should be installed
  {                                       // it just runs without return other than Yield();
   // All tasks in Init(...); will come to this point
   // at first run. They start with their own stack pointer
   // all registers are filled with scratch but that doesn't matter because they start new !
    
    TP->new_task = false;
    Sreg=SREG;                            // we want to save SREG: Interrupts enabled/disable is the same for new task
    cli();
    SP = TP->task_stack;                  // set SP to the new task. It will save it's stack when Yielded
    SREG=Sreg;  
    
    BITCLEARD6;
                                          // Registers are pushed and old Stackpointer is save and this tasks Stackpointer is set
    TP->function ();                      // now run the task amrked as new. It should not return ( starting a while(1)-loop)

    // if there is no while-loop or while is left:
    // a task has ended!
    TP->state = BLOCKED;                  // all finished task ( jumped out of while(1) ) will end here
    TP->new_task == true;                 // if resumed they start again        
                                          // this could make sense: such tasks may be resumed by interrupt or other tasks
                                          // for a "one-shot-action"
  }

  
  else                                    // tasks, which have run and saved their registers are invoked here:
  {
                                          // Yield had select the this task: highest priority and READY
    Sreg=SREG;                            // save interrupt enabled state
    cli();                                // switch to the SP of this task, own stack is saved (s.a. pushall )
    SP = TP->sp_save;                     // switch the stack to the saved stackpt
    SREG=Sreg;                            // restore interrupt enabled state
    popall();                             // pop saved registers and return from last run => goto last Yield-Point of switched out task
    
    //YActive=false;
    
    BITCLEARD6;
    return;
  }

}



// This is the kernel of CoopOS_MT_Stack
//
// This function is called from the task to give up CPU
// OR
// from StartMultiTasking() with Idle-task
// If mics >0 then the task ist delayed (micro seconds)

bool YActive=false;  

/**
 * @brief Yield() is the Scheduler
 * 
 * @details
 * Yield() is called by Yield(0) and Delay(micros)
 * Yield decides which task should run next and starts that task
 * via TaskSwitch(old, new)
 */
void Yield(unsigned long mics) {
unsigned long m=micros();
struct task *tp, *tp2;
  if (YActive) return;

YieldLoop:
  YActive=true;
  
  SwitchCount++;
  BITSETD6;
  tp=&Tasks[current_task];
  tp->lastCalled=m;

  // Delay(x) ?
  if (tp->state==READY) {
    if (mics>0) {
      tp->Delay=mics;
      //tp->stopped=1;
      tp->state = BLOCKED;
      tp->state2 = DEL;
    }
  }

  // Search for the next task to run
  uint8_t prio=0;
  uint8_t oldTask=current_task;

  oldTask=current_task;                             // save the old task for TaskSwitch(old, new)
  uint8_t HiPrio=0, HiNum=0;
  extern uint8_t number_of_tasks;
  uint8_t i;

  
  for (i=1; i < number_of_tasks; i++) {
    
    tp2=&Tasks[i];
    if (tp2->state==BLOCKED) {                      // test: BLOCKED -> READY? Delay is over?
      ///MySerial.println(i);
      if (tp2->Delay) {
        if ((m-tp2->lastCalled)>=tp2->Delay) {      // is a new task ready ? 
          tp2->Delay=0;
          tp2->state=READY;
          tp2->state2=RDY;
        }  
      }
    }

    // Search Task with highest priority:
    if (tp2->state== READY) {
       if (tp2->prio > HiPrio) {                
         HiPrio=tp2->prio;
         HiNum=i;
       }
    }
    
  }
    
  current_task=HiNum;                         // This is the new task to run
  Tasks[current_task].state=READY;
  Tasks[current_task].state2=RUN;
  YActive=false;

  BITSETD4;
  TaskSwitch(oldTask, current_task);  
  BITCLEARD4;
  
}











extern void Idle();
int freeRam();

/**
 * @brief TaskInit() fills the Table of Tasks to start
 * 
 * @details
////         NAM     FUNC            STCK PRIO  DLY        STATE  
  TaskInit("T1 ",  Task1,            90, 104,   0,        READY);
              |       |               |   |      |          |
            NAM is the const char* to a name     |          |
                      |               |   |      |          |
            FUNC is the name of the task function the source|
                                      |   |      |          |                    
            STCK is the amount of stack used by this task   |
                                          |      |          |
            PRIO is the priority of this task    |          |
                                                 |          |
            DLY is the delay for this task in microseconds  |
                                                            |     
            STATE can be READY or BLOCKED (if DLY > 0)      |
*/
uint8_t TaskInit(char* _name,
    FuncPt _function,
    int16_t _stackLen,
    uint8_t _prio,
    unsigned long _delay,
    State _state) 

{
static int task_num=0;
//static int StackPt=STACK;
extern int __heap_start, *__brkval; 
  if (task_num>=MAX_TASKS) {
    //MySerial.println(F("INIT: MAX_TASKS OVERFLOW:"));
    Serial.println("INIT: MAX_TASKS OVERFLOW:");
    Serial.println(_name);
    while(1);
  }

  // auto-create idle task Idle
  if (task_num==0) {
    //TaskInit("T0", task0, 0, St_len, false , 0, READY);
    Tasks[task_num].name = "Idle";
    Tasks[task_num].function = Idle;
    Tasks[task_num].prio = 0;
    Tasks[task_num].stackLen=IDLE_STLEN;

    if (freeRam<100) {
      //MySerial.println(F("FreeRam < 100 Bytes !!!"));
      Serial.println(F("FreeRam < 100 Bytes !!!"));
      while(1);
    }
   
    Tasks[task_num].task_stack = STACK;
    Tasks[task_num].stackLen = IDLE_STLEN;
    
    Tasks[task_num].new_task = false;
    Tasks[task_num].lastCalled = micros();
    Tasks[task_num].Delay = 0;
    Tasks[task_num].state = READY;
    Tasks[task_num].state2 = RDY;
    
    task_num++;
    number_of_tasks=task_num;
    StackPt-=IDLE_STLEN;

//    Serial.println((int)StackLow);
//    Serial.println((int)IDLE_STLEN);
//    Serial.println((int)StackPt);
    Serial.print("Idle"); Serial.print(-IDLE_STLEN); Serial.print(F(": Stack free for next task: ")); Serial.println((int)(StackPt-StackLow));
  
  }
  

  Tasks[task_num].name = _name;
  Tasks[task_num].function = _function;
  Tasks[task_num].prio = _prio;
  //Tasks[task_num].task_stack = _task_stack;
  Tasks[task_num].task_stack = StackPt;
  StackPt-=_stackLen;
  Tasks[task_num].stackLen=_stackLen;
  
  if ( StackPt < StackLow ) {
    //MySerial.println(F("ERROR: STACK too small !"));
    Serial.println(F("ERROR: STACK too small !"));
    while(1);
  }

  //Serial.println((int)_stackLen);
  //Serial.println((int)StackPt);
  Serial.print(_name);  Serial.print(-_stackLen); Serial.print(F(": Stack free for next task: ")); Serial.println((int)(StackPt-StackLow));
  
  
  Tasks[task_num].new_task = true;
  Tasks[task_num].lastCalled = micros();
  Tasks[task_num].Delay = _delay;
  if (_delay != 0) _state=BLOCKED;
  Tasks[task_num].state = _state;
    
  task_num++;
  number_of_tasks=task_num;
  return task_num-1;

}



int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
