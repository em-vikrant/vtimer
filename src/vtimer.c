/* FILE: vtimer.c
 * Used to implement import functionality to support virtual timers on a single timer.
 */

/* Built-In Libraries. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Custom Libraries. */
#include "vtimer.h"
#include "hw_timer.h"


/* Structs & Unions. */
/* This structure is used to represent a timer instance list. */
struct TimerList
{
    vtimer_t timer;
    struct TimerList* next;
};

/* Typedef. */
typedef struct TimerList TimerList;

/* Private Variables. */
static TimerList* timerList;     // Virtual timers linked list.

/* Private Functions. Prototypes. */
void vtimerHandler();
TimerList* createTimerNode(uint32_t duration, uint32_t lastTime, timerCallback tmCb);
TimerList* insertTimerNode(TimerList* head, TimerList* timerNode);
uint32_t findMinTime(TimerList* head);


/* Public fucntions. */
/* Initialize the timer variables. */
int8_t vtimerInit()
{
    timerList = NULL;
    hw_timer_registerCallback(vtimerHandler);
    return(0);
}

/* Execute function. To be called continuously. */
int8_t vtimerExecute()
{
    return(0);
}

/* Used to set the timer. */
void vtimerSet(uint32_t duration, timerCallback tmCb)
{
    TimerList* timerNode = createTimerNode(duration, GetTick(), tmCb);
    if(timerNode == NULL)
    {
        fprintf(stderr, "Unable to create a timer\r\n");
        return;
    }

    /* Add the time node to the linked list. */
    timerList = insertTimerNode(timerList, timerNode);

    uint32_t minTime = findMinTime(timerList);
    if(minTime > 0)
    {
        hw_timer_start(minTime);
    }
}

/* Private Functions. Implementations. */
/* Used to handle the timer interrupts comming from the hardware tick device. */
void vtimerHandler()
{
    uint32_t currTime = GetTick();

    /* Refresh the timer linked list. */
    TimerList* prev = NULL;
    TimerList* ptr = timerList;

    while(ptr != NULL)
    {
        // Check for each timer node. If timer elapsed then disable the timer
        if(ptr->timer.expiryTime <= currTime)
        {
            ptr->timer.enabled = false;
            ptr->timer.tmCb();  // Give callback to the respective client.

            // Remove the timer node for which timer is elapsed.
            if(ptr == timerList)
            {
                TimerList* temp = ptr;
                ptr = ptr->next;
                temp->next = NULL;
                free(temp);

                timerList = ptr;
            }
            else if(ptr->next == NULL && prev != NULL)
            {
                prev->next = NULL;
                free(ptr);
                break;  // End node removed
            }
            else
            {
                TimerList* temp = ptr;
                prev->next = ptr->next;
                ptr = ptr->next;
                free(temp);
            }
        }
        else
        {
            // Update the timers of all nodes accordingly.
            ptr->timer.expiryTime = ptr->timer.duration + currTime;
            prev = ptr;
            ptr = ptr->next;
        }
    }

    /* Invoke the hardware timer for the refreshed list. */
    uint32_t minTime = findMinTime(timerList);
    if(minTime > 0)
    {
        hw_timer_start(minTime);
    }
}

/* Used to create a node at the end of the linked list. */
TimerList* createTimerNode(uint32_t duration, uint32_t lastTime, timerCallback tmCb)
{
    TimerList* newTimer = (TimerList*)malloc(sizeof(TimerList));
    if(newTimer == NULL)
    {
        return(newTimer);
    }

    newTimer->timer.enabled = true;
    newTimer->timer.timerId = 0;
    newTimer->timer.lastTime = lastTime;
    newTimer->timer.duration = duration;
    newTimer->timer.expiryTime = lastTime + duration;
    newTimer->timer.tmCb = tmCb;
    newTimer->next = NULL;
    return(newTimer);
}

TimerList* insertTimerNode(TimerList* head, TimerList* timerNode)
{
    if(head == NULL)
    {
        head = timerNode;
    }
    else
    {
        for(TimerList* ptr = head; ptr != NULL; ptr = ptr->next)
        {
            if(ptr->next = NULL)
            {
                ptr->next = timerNode;
                break;
            }
        }
    }
    return(head);
}

uint32_t findMinTime(TimerList* head)
{
    if(head == NULL)
    {
        return(0);
    }

    uint32_t minTime = head->timer.duration;
    for(TimerList* ptr = head->next; ptr != NULL; ptr = ptr->next)
    {
        if(minTime > ptr->timer.duration)
        {
            minTime = ptr->timer.duration;
        }
    }

    return(minTime);
}


