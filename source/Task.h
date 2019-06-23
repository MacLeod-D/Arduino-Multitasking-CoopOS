// Task.h
// (C) 2019 Helmut Weber

#ifndef TASK_H
#define TASK_H



typedef void (* FuncPt) (void);                             // Function pointer

enum State  { BLOCKED, READY};                              /**<  State of task */
enum State2  { NON, RDY, RUN, DEL, BLK };                   /**<  SubState of task */
char *State2Txt[] = { "NON", "RDY", "RUN", "DEL", "BLK" };  /**<  full description of state */

/**
@class task
This is the structure of a TaskBLock
*/

struct task {
    char* name;                                             /**< Name of a task as const char* */
    FuncPt function;                                        /**< Pointer to function to execute as task */
    uint8_t prio;                                           /**< Priority of task, only relative values are used */
    uint16_t sp_save;                                       /**< Value of stack pointer saved before switched */
    uint16_t task_stack;                                    /**< Pointer to first Stackpointer of this task */
    uint16_t stackLen;                                      /**< Length of reserved stackspace  for this task */
    char     new_task;                                      /**< Allways true except Idle */
    unsigned long lastCalled;                               /**< Last time called in µs */
    unsigned long Delay;                                    /**< Delay in µs */
    State   state;                                          /**< State of task (READY, BLOCKED) */
    State2  state2;                                         /**< Substate of task (sa.) */
};

//-----------------------------Globals-----------------------------------
// for multitasking
uint8_t number_of_tasks;                                    /**< set and counted by Init() */
unsigned int FirstSP;                                       /**< StackPtr for all tasks at start of program*/
unsigned int StackLow;                                      /**< Low Boundery of all taskstacks */
unsigned int StackHi;                                       /**< High Boundery of all taskstacks */


char *STACK; 
volatile struct task Tasks[MAX_TASKS];                      /**< Table of all initialized task */
volatile uint8_t current_task=0;                            /**< Actual Task running */
unsigned int IdleCount;   
volatile unsigned int SwitchCount;                          /**<  incremented by Yield/Delay calls */
static int StackPt;


/**
 * @brief StackPrepepare Alloc the new stack for all tasks
 * 
 * @details
 * Malloc is used to get the amount of STACKALLOC bytes from the heap as a new stack for all tasks<br>
 * This is the only malloc in the program.<br>
 * The reserved block is used by TaskInit() to distribute parts of this block to the single tasks<br>
 * TaskInit() stops if there is not enough space for the stacks of the tasks.
 */
 
 
void StackPrepare() {
// Alloc the stack for tasks:
  STACK=malloc(STACKALLOC+1); 
  //Serial.println((uint16_t)STACK);
  StackLow=STACK;
  StackHi=STACK+STACKALLOC;
  STACK  =StackHi;
  StackPt=STACK;
  //Serial.println((uint16_t)STACK);
  
}
  


#endif
