/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )

TaskHandle_t vButton_1_MonitorHandler = NULL;
TaskHandle_t vButton_2_MonitorHandler = NULL;
TaskHandle_t vPeriodic_TransmitterHandler = NULL;
TaskHandle_t vUart_ReceiverHandler = NULL;
TaskHandle_t vLoad_1_SimulationHandler = NULL;
TaskHandle_t vLoad_2_SimulationHandler = NULL;


pinState_t button1State;
pinState_t button2State;
signed char send[10] = "Hiii";

int T1_TxStamp = 0, T1_TzStamp = 0, T1_totalT;
int T2_TxStamp = 0, T2_TzStamp = 0, T2_totalT;
int system_time = 0;
int cpu_load = 0;

char runTimeStatsBuff[190];

volatile int misses = 0;

/* Task to be created. */
void Button_1_Monitor( void * pvParameters )
{
	
	TickType_t xLastWakeTime;
	
	xLastWakeTime = xTaskGetTickCount();
	
	vTaskSetApplicationTaskTag(NULL, (void *) 1);


    for( ;; )
    {
			
        /* Task code goes here. */
			button1State = GPIO_read(PORT_0, PIN0);
			
			vTaskGetRunTimeStats( runTimeStatsBuff );
			
			//xSerialPutChar('\n');
			
			//vSerialPutString(runTimeStatsBuff, 190);

			vTaskDelayUntil( &xLastWakeTime, 50 );
			
		}
}

void Button_2_Monitor( void * pvParameters )
{
	
	TickType_t xLastWakeTime;
	
	xLastWakeTime = xTaskGetTickCount();
	
	vTaskSetApplicationTaskTag(NULL, (void *) 2);


    for( ;; )
    {
			
        /* Task code goes here. */
			button2State = GPIO_read(PORT_0, PIN1);
			
			vTaskGetRunTimeStats( runTimeStatsBuff );
			
			
			//xSerialPutChar('\n');
			
			
			//vSerialPutString(runTimeStatsBuff, 190);

			vTaskDelayUntil( &xLastWakeTime, 50 );
			
		}	
}


void Periodic_Transmitter( void * pvParameters )
{
	
	
	TickType_t xLastWakeTime;
	
	xLastWakeTime = xTaskGetTickCount();
	
	vTaskSetApplicationTaskTag(NULL, (void *) 3);


    for( ;; )
    {
			
        /* Task code goes here. */
			
			vTaskGetRunTimeStats( runTimeStatsBuff );
			
			vSerialPutString(send, 10);
			//xSerialPutChar('\n');
			
			//vSerialPutString(runTimeStatsBuff, 190);

			GPIO_write(PORT_0, PIN6, PIN_IS_LOW);
			vTaskDelayUntil( &xLastWakeTime, 100 );
			GPIO_write(PORT_0, PIN6, PIN_IS_HIGH);
			
		}
}

void Uart_Receiver( void * pvParameters )
{
	
	TickType_t xLastWakeTime;
	
	xLastWakeTime = xTaskGetTickCount();
	
	vTaskSetApplicationTaskTag(NULL, (void *) 4);


    for( ;; )
    {
			
        /* Task code goes here. */
			
			vTaskGetRunTimeStats( runTimeStatsBuff );
			
			xSerialGetChar(send);
			//xSerialPutChar('\n');
			
			//vSerialPutString(runTimeStatsBuff, 190);

			GPIO_write(PORT_0, PIN7, PIN_IS_LOW);
			vTaskDelayUntil( &xLastWakeTime, 20 );
			GPIO_write(PORT_0, PIN7, PIN_IS_HIGH);
			
			
		}
}

void Load_1_Simulation( void * pvParameters )
{
	int i;
	TickType_t xLastWakeTime;
	
	xLastWakeTime = xTaskGetTickCount();
	
	vTaskSetApplicationTaskTag(NULL, (void *) 5);


    for( ;; )
    {
			
        /* Task code goes here. */
			/* Excution time about 1.7s */
			for(i = 0; i < 2000; i++)
			{
				i = i;
				
				GPIO_write(PORT_0, PIN3, PIN_IS_HIGH);
				GPIO_write(PORT_0, PIN4, PIN_IS_LOW);
				GPIO_write(PORT_0, PIN2, PIN_IS_LOW);
			}
			
			vTaskGetRunTimeStats( runTimeStatsBuff );
			
			//xSerialPutChar('\n');
			
			//vSerialPutString(runTimeStatsBuff, 190);

			vTaskDelayUntil( &xLastWakeTime, 10 );
			
		}
}


void Load_2_Simulation( void * pvParameters )
{
	int i;
	TickType_t xLastWakeTime;
	
	xLastWakeTime = xTaskGetTickCount();
	
	vTaskSetApplicationTaskTag(NULL, (void *) 5);


    for( ;; )
    {
			
        /* Task code goes here. */
			/* Excution time about 12ms */
			for(i = 0; i < 4000; i++)
			{
				i = i;
				
				GPIO_write(PORT_0, PIN4, PIN_IS_HIGH);
				GPIO_write(PORT_0, PIN3, PIN_IS_LOW);
				GPIO_write(PORT_0, PIN2, PIN_IS_LOW);
			}
			
			vTaskGetRunTimeStats( runTimeStatsBuff );
			
			//xSerialPutChar('\n');
			
			//vSerialPutString(runTimeStatsBuff, 190);

			vTaskDelayUntil( &xLastWakeTime, 100 );
			
		}
}



	

/*Implement Tick Hook */
void vApplicationTickHook( void )
{
	GPIO_write(PORT_0, PIN5, PIN_IS_HIGH);
	GPIO_write(PORT_0, PIN5, PIN_IS_LOW);
}

/*Implement Idle Hook */
void vApplicationIdleHook( void )
{
	GPIO_write(PORT_0, PIN2, PIN_IS_HIGH);
	
	GPIO_write(PORT_0, PIN3, PIN_IS_LOW);
	GPIO_write(PORT_0, PIN4, PIN_IS_LOW);
}



/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/


/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();

	
	/* Create the task, storing the handle. */
	xTaskCreatePeriodic( 
										Button_1_Monitor,						/* Function that implements the task. */
                    "Button 1 Monitor",         /* Text name for the task. */
                    100,            	/* Stack size in words, not bytes. */
                    ( void * ) 0,    	/* Parameter passed into the task. */
                    1,              	/* Priority at which the task is created. */
                    vButton_1_MonitorHandler,    /* Used to pass out the created task's handle. */
                    50 );

	xTaskCreatePeriodic( 
										Button_2_Monitor,						/* Function that implements the task. */
                    "Button 2 Monitor",         /* Text name for the task. */
                    100,            	/* Stack size in words, not bytes. */
                    ( void * ) 0,    	/* Parameter passed into the task. */
                    1,              	/* Priority at which the task is created. */
                    vButton_2_MonitorHandler,    /* Used to pass out the created task's handle. */
                    50 );

	xTaskCreatePeriodic( 
										Periodic_Transmitter,						/* Function that implements the task. */
                    "Periodic Transmitter",         /* Text name for the task. */
                    100,            	/* Stack size in words, not bytes. */
                    ( void * ) 0,    	/* Parameter passed into the task. */
                    1,              	/* Priority at which the task is created. */
                    vPeriodic_TransmitterHandler,    /* Used to pass out the created task's handle. */
                    100 );
										
	xTaskCreatePeriodic( 
										Uart_Receiver,						/* Function that implements the task. */
                    "Uart Receiver",         /* Text name for the task. */
                    100,            	/* Stack size in words, not bytes. */
                    ( void * ) 0,    	/* Parameter passed into the task. */
                    1,              	/* Priority at which the task is created. */
                    vUart_ReceiverHandler,    /* Used to pass out the created task's handle. */
                    20 );												
										
	xTaskCreatePeriodic( 
										Load_1_Simulation,						/* Function that implements the task. */
                    "Load 1 Simulation",         /* Text name for the task. */
                    100,            	/* Stack size in words, not bytes. */
                    ( void * ) 0,    	/* Parameter passed into the task. */
                    1,              	/* Priority at which the task is created. */
                    vLoad_1_SimulationHandler,    /* Used to pass out the created task's handle. */
                    10 );	

	xTaskCreatePeriodic( 
										Load_2_Simulation,						/* Function that implements the task. */
                    "Load 2 Simulation",         /* Text name for the task. */
                    100,            	/* Stack size in words, not bytes. */
                    ( void * ) 0,    	/* Parameter passed into the task. */
                    1,              	/* Priority at which the task is created. */
                    vLoad_2_SimulationHandler,    /* Used to pass out the created task's handle. */
                    100 );											
										
	
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


