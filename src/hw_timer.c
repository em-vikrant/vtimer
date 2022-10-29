/* FILE: hw_timer.c
 * Used to simulate a hardware timer (clock) on a microcontroller by using epoch time holder variables.
 */

/* Built-In Libraries. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Custom Libraries. */
#include "hw_timer.h"


/* #define. */
#define SET_TIMER_SEC(X,Y)  X=(GetTick()+Y)
#define IS_TIMER_ELAPSED(X) (X<=GetTick()?true:false)

/* Typedef. */
typedef uint32_t HWTIMER;

/* Private Variables. */
static HWTIMER hwTimer;
static hw_timer_interruptCb interruptCb;

/* Private Functions. */
static void hw_timer_interrupt_dummy()
{
    printf("Hardware Timer! Elapsed!\r\n");
}

/* Public Functions. */
int8_t hw_timer_init()
{
    /* Set the hardware timer. */
    hwTimer = 0;
    return(0);
}

int8_t hw_timer_execute()
{
    if(hwTimer != 0 && IS_TIMER_ELAPSED(hwTimer))
    {
        /* Invoke the interrupt. */
        interruptCb();
        hw_timer_stop();
    }
    return(0);
}

void hw_timer_registerCallback(hw_timer_interruptCb callback)
{
    if(callback == NULL)
    {
        interruptCb = hw_timer_interrupt_dummy;
    }
    else
    {
        interruptCb = callback;
    }
}

void hw_timer_start(uint32_t seconds)
{
    /* Set the timer. */
    SET_TIMER_SEC(hwTimer, seconds);
}

void hw_timer_stop()
{
    /* Set the timer to 0. */
    hwTimer = 0;
}


