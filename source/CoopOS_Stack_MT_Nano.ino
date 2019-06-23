/**@file*/

// The Program

// (C) 2019 Helmut Weber



// This is only used for documentation, not for the program:
/** #include  "Doxyfile.dox"  // extension mapping dox=md does the trick ;) */


///<pre><code class="language-C">

#include "Pins.h"                 // Much faster Digital IO:



////--------------------------------- CONFIG STACK-------------------------------------
//// MUST be declared before including "TaskSwitch.h"
#define STACKALLOC 730
#define IDLE_STLEN 80
#define MAX_TASKS 8
////------------------------------------------------------------------------------------


////--------------------------------- CONFIG TRACE_ON-----------------------------------
#define STACKALLOC 730
//// MUST be declared before including "TaskSwitch.h"
//// TRACE_ON must be enabled to get a Stack Trace for the Debugger
//// But it will slow down to 1/2 performance !
//// Must be included before "TaskSwitch.h"
////#define TRACE_ON
////------------------------------------------------------------------------------------


///--------------------------------- CONFIG WDT-----------------------------------------
//// WDT MUST be enabled before including "TaskSwitch.h
//// WDT should only be enabled to test the MAX time (worst case) a task needs from
//// Yiedl(0) / Delay(n)  until the next    Yiedl(0) / Delay(n)
//// s. TaskSwitch.h for some examples
#define WDT
//// If only Task1 and Task2 are running you may try:
//// #define WDT_VALUE 119             // == 60 µs
//// If only Task1 and Task2 are running and ALL is enabled try:
////                   #define WDT_VALUE 10            // 5 µs ... for WDT-test: 200000 interrupts/s
//// together with WDT:
#define WDT_VALUE 499  // = 250 µs                                 // max . 65000
////------------------------------------------------------------------------------------


//// DO NOT MOVE THIS LINE !
#include "TaskSwitch.h"





////--------------------------------- CONFIG MODULES-------------------------------------
//// Here are the predefined Tasks
//// __MySer should be enabled if you have output to Serial.
//// THIS distributes serial output for better timing of all tasks
#define __MYSER

//// Enable __DEBUG to set BREAKPOINTs in the source and to use the Debug Button at D2
#define __DEBUG

//// ShowStack should only be enabled to finetune the stack sizes
#define __SHOWST
////------------------------------------------------------------------------------------





//// Prepare use of MySerial and Debug - after CONFIG
#include "DoPrep.h"



#define LED_On    BITSETD13;
#define LED_Off   BITCLEARD13


// for tasks
char              DisplayUsed=0;                                 /**< Used as mutex for the output */
unsigned int      BlinkCount;                                    /**<  Incremented by task 1 */
uint8_t           DbgHandle;                                     /**<  Handle for Debug = Waked up by interrupt */




// --------------------- Definition of Tasks ------------------------

// Idle must always be present and MUST have function name:  Idle !!!
void Idle (void)
{
  while (1)
  {
    IdleCount++;    // only for test, you may comment out this line
    Yield (0); 
  }
}





void Task1 (void)
{
  while (1)
  {
    stopMe();                                         // activated by next Yield/ Delay
    LED_Off;                                          // 45 µs since Task2
    BlinkCount++;
    Yield(0);                                         // Task1 has been stopped here
  }
}



void Task2 (void)
{
  while (1)
  {
    LED_On;
    BlinkCount++;
    Tasks[1].state = READY;                           // Start Task1
    Delay(200);                                       // <-------- NOT 0 
  }
}



void Task3 (void)
{
unsigned char missing;
  while (1)
  {
    BITSETD5;
    if (DisplayUsed==0) { 
      DisplayUsed=1;
      MySerial.print("Task 3 :");
      if (missing==0) {                               // Total output: 100 µs
        MySerial.print(" IdleCount/s: ");
        Yield(100);
        
        MySerial.print(IdleCount);
        Yield(100);
        MySerial.print(", BlinkCount/s: ");
        Yield(100);
        MySerial.print(BlinkCount);
        Yield(100);
        MySerial.print(", SwitchCount/s: ");
        Yield(100);
        MySerial.println(SwitchCount);
        Yield(100);
      }
      else { 
        missing=0;
        MySerial.println("missing values");
      }
      DisplayUsed=0;
      
    }
    else missing=1;
    IdleCount=0;
    BlinkCount=0;
    SwitchCount=0;
    
  
    BITCLEARD5;
    Delay(1000000);
  }
}


void Task4_fun() {
   BITSETD5;
    if (DisplayUsed==0) { MySerial.print("---------Task 4, SP: 0x"); MySerial.println(SP,16);}
    BITCLEARD5;
    Delay(100000);

    while (MySerial.available()) {
      char ch = MySerial.read();
      //MySerial.flush();
      Serial.print("\n---------------------- Serial input: ");  Serial.println(ch);

#ifdef __DEBUG      
      //// Test a BREAKPOINT - __DEBUG must be enabled, TRACE_ON should be enabled
      ////BREAKPOINT;
#endif      
    }
    
}


void Task4 (void)
{
  while (1)
  {

    Task4_fun();                                             // Test Yield in a called function  
//    BITSETD5;
//    if (DisplayUsed==0) { MySerial.print("---------Task 4, SP:"); MySerial.println(SP);}
//    BITCLEARD5;
//    Delay(100000);
  }
}



#ifdef __SHOWST
#include "ShowSt.h"
#endif


#ifdef __MYSER
#include "MySer.h"
#endif


#ifdef __DEBUG
#include "Debug.h"
#endif



void setup() {
  Serial.begin(500000);
  
#ifdef __MYSER  
  MySerial.setSerial(&Serial); // Redirect Serial 
#endif

  StackPrepare();
  

  if (STACK==NULL) {
    Serial.println(F("ERROR: Not enough room for Stack !!!"));
    while(1);
  }
  Serial.print(F("\n\nStack allocated: ")); Serial.println(STACKALLOC);
  Serial.print(F("Free Ram now   : ")); Serial.println(freeRam());
  Serial.println();
  
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT); // TaskSwitch
  pinMode(2,INPUT_PULLUP);
 
  // fill stack with 0x55 as markers to test stack usage
  uint8_t *pt=STACK;  while (pt>StackLow) { *pt--=0x55;}
    
  Serial.println(F("----------- Init start -----------"));
  
////       NAM   FUNC   STCK PRIO  DLY STATE  
  TaskInit("T1 ",  Task1,           90, 105,   0,        READY);
  TaskInit("T2 ",  Task2,           90, 106,   0,        READY);
  TaskInit("T3 ",  Task3,           80, 100,   0,        READY);
  TaskInit("T4 ",  Task4,           90, 102,   5000000,  BLOCKED);  // starts after 5 seconds



#ifdef __SHOWST  
  TaskInit("SHS",  ShowStack_Task, 110, 103,   0,        READY);
#endif


#ifdef __MYSER  
  TaskInit("MyS", MySer_Task,      80,  103,   0,        READY);
#endif

#ifdef __DEBUG
  DbgHandle=
  TaskInit("Dbg", Dbg_Task,       110,  102,   0, READY);
#endif



  Serial.println(F("----------- Init ready -----------"));
  Serial.print(F("\nFree Ram now   : ")); Serial.println(freeRam());

#ifdef __DEBUG//For Debugger-Button:
  cli(); //For Debugger-Button: Enable FALLING interrupt at D2
  pinMode(2, INPUT_PULLUP);
  attachInterrupt (digitalPinToInterrupt (2), IRQ_Answer, FALLING);  // attach interrupt handler for D2
  sei();
#endif

  
  StartMultiTasking();
  Serial.println(F("Returned from StartMultiTasking()"));
  Serial.flush();
  while(1);
}

  
void loop() {
  // never executed !!!
  MySerial.println("LOOP");

}
///</code></pre>
