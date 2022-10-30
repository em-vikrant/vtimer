/* FILE: vtimer.h
 * File for virtial timer, It contains important structures, function protypes, and other variables.
 */

#ifndef VTIMER_H
#define VTIMER_H

/* Built-In Libraries. */
#include <stdint.h>
#include <stdbool.h>


/* Typedef. */
typedef void (*timerCallback)();

/* Structures and Unions. */
/* STRUCT: Used as information for a virtual timer. */
typedef struct
{
    bool enabled;
    uint8_t timerId;
    uint32_t lastTime;
    uint32_t expiryTime;
    uint32_t duration;              // Duration
    timerCallback tmCb;             // Callback
}vtimer_t;

/* Public Functions. */
int8_t vtimerInit();
int8_t vtimerExecute();
int8_t vtimerSet(uint32_t duartion, timerCallback tmCb);

#endif
