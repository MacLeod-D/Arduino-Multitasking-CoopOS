

#ifdef __SHOWST

/**
 * \brief ShowStack shows the stack of all tasks
 * 
 * It sets DisplayUsed=1 so other tasks can suppress their output<br>
 * The state and the full stack is shown for all tasks in the system.
 */


void ShowStack_Task (void)
{
int ii,j;
int count;
uint8_t endpt;

extern int freeRam();

  while (1)
  {
 
    //BREAKPOINT;
    while (DisplayUsed) Yield(100);; // Wait for Display
    DisplayUsed=1;
    for (ii=0; ii<number_of_tasks; ii++) {
      MySerial.println();
      Yield(100);
      MySerial.print(ii); 
      Yield(100);
      MySerial.print(" "); 
      Yield(100);
      MySerial.print(Tasks[ii].name);
      Yield(100);
      MySerial.print(" State  ");
      Yield(100);
      //MySerial.print( (Tasks[ii].state==READY) ? "READY" : "BLOCKED");
      if (Tasks[ii].state==READY) MySerial.print("READY"); else MySerial.print("BLOCKED"); 
      Yield(100);
      MySerial.print(" , ");
      Yield(100);
      MySerial.println( State2Txt[(Tasks[ii].state2)]); // ? "READY" : "NOTRDY");
      //MySerial.println(memSearch(Tasks[ii].task_stack-Tasks[ii].stackLen+1,Tasks[ii].sp_save,0x55)); 
      MySerial.print("Free stack space: "); 
      Yield(100);
      MySerial.println(stackFree(ii));
      Yield(100);

      //MySerial.print(F("StackLen: decimal "));
      MySerial.print("StackLen: decimal ");
      Yield(100);
      MySerial.println(Tasks[ii].stackLen);
      Yield(100);
      
      //MySerial.print(F("Stack: "));
      MySerial.print("Stack: ");
      for ( j=0; j<Tasks[ii].stackLen; j++) {
        Yield(10);
        if ((j%32)==0) {
          MySerial.println();
          MySerial.print("0x");
          
          MySerial.print(Tasks[ii].task_stack-j,HEX);
          MySerial.print(":  ");
          Yield(100);
        }
        Yield(10);

        if( (uint8_t)*(uint8_t *)(Tasks[ii].task_stack-j) < 16) MySerial.write('0');
        MySerial.print ( *  (uint8_t *)(Tasks[ii].task_stack-j),16);
        //MySerial.print ( *  (uint8_t *)(Tasks[ii].task_stack-j));
        //MySerial.print(F(" "));
        MySerial.print(" ");
        Yield(100);
      }
      MySerial.println();
      
    }
    MySerial.println();
    count++;
    DisplayUsed=0;

    //while(1);
    
    Yield(4000000);
  }
} 

#endif
