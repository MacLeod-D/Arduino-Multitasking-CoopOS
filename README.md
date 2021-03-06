**Arduino Multitasking Stackchange Beta 1.1** July 8 2019
 
 CoopOS with Stackframes<br>

 <b>You never thought that multitasking can be so easy:</b><br>
 
 This is a simple and fast approach to multitasking.
 
 <blockquote>
 <br>
 • Very easy to use even for beginners - nothing else comes close<br>
 • Usable for professionals<br>
 • Reliable timings<br>
 • Tasks have priorities and own stacks<br>
 • Full documentation<br>
 • Compatible with all Arduino Libraries<br>
 • In the simplified form only one file to include<br>
 • Could be combined with RTOS's as Idle-Task<br>
 • Easy to port to other processors
 • Valuable tools for development and tests
<h3> • 25000 (40 µs) TaskSwitches per second on Arduino-UNO / -NANO are possible<br>
 • Breakpoints in source or with Debug-Button (stop/resume) or serial input<br>
 • Breakpoint may show last 20 scheduled tasks with timestamp<br>
 • Watchdog-Interrupt to test Task Switching<br>
 • Up to <b>200000</b> Interrupts/s !</h3>
  <br>
</blockquote>
<p></p>
<b>More than 50 pages documentation !</b><br>
<br>
Extract the CoopOS_Stack_MT_Nano.zip file with all features to your Arduino-sketch directory.
<br>
<br>
But it is highly **recommended** to read the documentation and experiment at first with the Demo files.<br>
This is the simple Demo0 - here we use Cooperative Multitasking with stack-change!<br>

 ![Demo](./CooperativeMT.png?raw=true "Cooperative Multitasking")



<blockquote>
<pre><code class="language-C">



	//// CoopOS_Stack_MT - Demo   (C) 2019 Helmut Weber
	//// Demo0

	#include "TaskSwitchDemo.h"   
	#define LED 13

	void Task1() {                                    // Blink LED
	    while(1) {                                    // <<< You will find these while(1) loops in all RTOS's examples
		digitalWrite(LED,HIGH);                   // if you call this in a normal Arduino sketch nothing else will run !
		Delay(50000);
		digitalWrite(LED,LOW);
		Delay(50000); // microseconds
	    }
	}


	void Task2() {                                    // Write to Serial Line
	    while(1) {                                    // Another infinit loop !
		Serial.println("Hello World");
		Delay(1000000);
	    }
	}

	void setup() {
	    Serial.begin(500000);
	    StackInit();                                  // Init the stacks for all tasks
	   
	    TaskInit("T1", Task1, 90, 100,   0, READY);   // define 2 tasks for multitasking:
	    TaskInit("T2", Task2, 90, 100,   0, READY);
	   
	    StartMultiTasking();                          // start the system:
	}

	void loop() {                                     // loop is never called
	    //// this is never called !!!
	    Serial.println("Hoops? How did you come to this line ???");
	}


//// CoopOS_Stack_MT - Demo   (C) 2019 Helmut Weber
//// Demo0

With the final program "TheProgram" it is possible to proof with the WDT<br>
that no task needs more than **100 µs** !<br>
That means: **highly deterministic** !<br>
<br>

 ![Demo](./Timing.jpg?raw=true "Timing of the system")

</code></pre>


