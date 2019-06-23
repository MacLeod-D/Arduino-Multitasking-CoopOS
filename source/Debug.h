#ifdef __DEBUG




void Dbg_Task() {
static int stopp=0;
  while(1) {
    stopMe();
    Debug.now=micros();
    ////MySerial.println("Dbg_Task");
    if (STOP) {
      Serial.println(F("\n\n ----------- HERE IS THE DEBUGGER -------------"));
#ifndef TRACE_ON
      Serial.println(F("\n\ATTENTION: TRACE_ON is not enabled ! No Stack Trace!"));
#endif
      Serial.print  (F("Latency(µs)   "));   Serial.println(Debug.now-Debug.start);
      Serial.print  (F("Stopped in    "));   Serial.println(Tasks[Debug.current].name);
      Serial.print  (F("StackPt was   0x"));   Serial.println(Debug.sp, HEX);
      Serial.println(F("Last Tasks: µs ID deltaT Name FreeStack"));
      for (int i=0; i<20; i++) {
        Serial.print(oldMicros[i]); Serial.print(F(" "));
        Serial.print(oldTasks[i]); Serial.print(F(" "));
        
        if (i>0) { Serial.print(oldMicros[i]-oldMicros[i-1]); }
        else Serial.print(0);
        Serial.print(F(" "));
        Serial.print(Tasks[oldTasks[i]].name);
        Serial.print(F(" "));
        Serial.println(stackFree(oldTasks[i]));
      }
    }
    while (STOP);
    Yield(0);                                         // Task1 has been stopped here
  }
}



void IRQ_Answer() {
  STOP=!STOP;
  Debug.sp = SP;
  Debug.current = current_task;
  Debug.start=micros(),
  resume_task(DbgHandle);
}

#endif
