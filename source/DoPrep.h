#ifndef __MYSER
#define MySerial Serial
#else
#include "MySerial.h"
#endif


#ifdef __DEBUG
//----------------------------------------
// This is only for the Debugger: Debug
volatile bool STOP=false;
struct debug {
  uint16_t     sp;
  uint8_t      current;
  unsigned long   start;
  unsigned long   now;
  
} Debug;
// ---------------------------------------
#define BREAKPOINT STOP=true; Debug.sp = SP; Debug.current = current_task; Debug.start=micros(); resume_task(DbgHandle)
#endif

