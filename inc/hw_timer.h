/* FILE: hw_timer.h
 * File used to simulate a hardware timer (clock) on a microcontroller.
 */

#ifndef HW_TIMER_H
#define HW_TIMER_H

/* Built-In Libraries. */
#include <stdint.h>
#include <stdbool.h>
#include <time.h>


/* Typedef. */
typedef void (*hw_timer_interruptCb)();

/* Public Functions. */
int8_t hw_timer_init();
int8_t hw_timer_execute();

void hw_timer_registerCallback(hw_timer_interruptCb callback);
void hw_timer_start(uint32_t seconds);
void hw_timer_stop();

static inline uint32_t GetTick() { return(time(NULL)); }

#endif
