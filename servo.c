#include <main.h>
#include <stdint.h>
#include <stdio.h>
#include "TM4C123GH6PM.h"                    // Device header
#include "inc/hw_ints.h"
#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "inc/hw_gpio.h"
#include "inc/hw_pwm.h"

#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"
#include "driverlib/cpu.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include <string.h>

#include "IO_Expander.h"
#include "Paser.h"

void PWM_Setup()
{
	SysCtlPWMClockSet(PWM_SYSCLK_DIV_64);
	//
	// Configure the PWM generator for count down mode with immediate updates
	// to the parameters.
	//
	PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	// Set the period. For a 50 KHz frequency, the period = 1/50,000, or 20
	// microseconds. For a 20 MHz clock, this translates to 400 clock ticks.
	// Use this value to set the period.
	//
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, MAXPERIOD);
	//
	// Set the pulse width of PWM1 for a 75% duty cycle.
	//
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, 0);
	
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, 0);
	// 50MHZ is to fast
	PWMClockSet(PWM1_BASE,PWM_SYSCLK_DIV_64);
	//
	// Start the timers in generator 3.
	//
	PWMGenEnable(PWM1_BASE, PWM_GEN_1);
	//
	// Enable the outputs.
	//
	PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT , true);

}
