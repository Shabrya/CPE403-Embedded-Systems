/*Shabrya Lott
 * Tiva_c Lab04
 * Usage:This is a simple program enables toggles LED using a timer and interrupt
 * Input: NONE
 * Output: Blue LED's toggling as specified freq and duty cycle
 */
#include <stdint.h>				//Variable definitions for the C99 standard
#include <stdbool.h>			//Boolean definitions for the C99 standard
#include "inc/tm4c123gh6pm.h"	//def. for the interrupt and register assignments on the Tiva C Series device on the launchPad board
#include "inc/hw_memmap.h"	 	//Macros defining the memory map of the Tiva C Series
#include "inc/hw_types.h"		//Defines common types and macros
#include "driverlib/sysctl.h"	//Defines macros for System Control API of Driverlib
#include "driverlib/interrupt.h"//defines & macros for NVIC Controller(Interrupt)API of driverlib.
#include "driverlib/gpio.h"		//Defines macros for GPIO API of Driverlib
#include "driverlib/timer.h"	//Defines and macros for Timer API of driverLib.


int main(void)
{
	uint32_t ui32Period;

	//System clock to 40Mhz (PLL= 400Mhz / 10 = 40Mhz)
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	//Port configuration (LEDS)
	//Enable GPIOF port
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	//set LEDS connected to pins as outputs
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	//Timer configurations

	//Enable TIMER0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	//Set Timer0 to Periodic mode
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	//PWM period at 10 Hz at 50% duty cycle
	ui32Period = (SysCtlClockGet() / 10) / 2;
	//Load timer period
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
	//Enable interrupts on Timer0
	IntEnable(INT_TIMER0A);
	//Set Timer0 to interrupt at timeout
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	//Enable master interrupt
	IntMasterEnable();
	//Enable Timer
	TimerEnable(TIMER0_BASE, TIMER_A);

	while(1)	//infinite loop
	{
	}
}
void Timer0IntHandler(void)
{
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	// Read the current state of the GPIO pin and
	// write back the opposite state
	if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
	{
		//turn off LEDS
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);

	}
	else
	{
		//Turn on Blue LED
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);

	}
}

