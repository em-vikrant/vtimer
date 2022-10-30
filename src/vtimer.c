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
static bool hwTimerInUse;

/* Private Functions. Prototypes. */
void vtimerHandler();
TimerList* createTimerNode(uint32_t duration, uint32_t lastTime, timerCallback tmCb);
TimerList* insertTimerNode(TimerList* head, TimerList* timerNode);
uint32_t findMinTime(TimerList* head);

void printDuraitonInList(TimerList* head)
{
    printf(" TIMER LIST = [ |");
    for(TimerList* ptr = head; ptr != NULL; ptr = ptr->next)
    {
        printf("%d:%d| ", ptr->timer.duration, ptr->timer.expiryTime);
    }
    printf(" ]\r\n");
}

/* Public fucntions. */
/* Initialize the timer variables. */
int8_t vtimerInit()
{
    timerList = NULL;
    hwTimerInUse = false;
    hw_timer_registerCallback(vtimerHandler);
    return(0);
}

/* Execute function. To be called continuously. */
int8_t vtimerExecute()
{
    return(0);
}

/* Used to set the timer. */
int8_t vtimerSet(uint32_t duration, timerCallback tmCb)
{
    TimerList* timerNode = createTimerNode(duration, GetTick(), tmCb);
    if(timerNode == NULL)
    {
        fprintf(stderr, "Unable to create a timer\r\n");
        return(-1);
    }

    /* Add the time node to the end of linked list. */
    timerList = insertTimerNode(timerList, timerNode);
    if(timerList == NULL)
    {
        fprintf(stderr, "TIMER LIST NULL\r\n");
    }

    /* Serve the node having the minimum duration time. */
    uint32_t minTime = findMinTime(timerList);
    if(minTime <= 0)
    {
        return(-2);
    }
    
    /* Set the hardware timer. */
    if(hwTimerInUse != true)
    {
        hw_timer_start(minTime);
        hwTimerInUse = true;
    }
    
    return(0);
}

/* Private Functions. Implementations. */
/* Used to handle the timer interrupts comming from the hardware tick device. */
void vtimerHandler()
{
    uint32_t currTime = GetTick();
    hwTimerInUse = false;

    /* Refresh the timer linked list. */
    TimerList* prev = NULL;
    TimerList* ptr = timerList;

    while(ptr != NULL)
    {
        /* Check for each timer node. */
        uint32_t expirationTick = ptr->timer.duration + ptr->timer.lastTime;
        if(expirationTick <= currTime)
        {
            ptr->timer.enabled = false;
            timerCallback cbHolder = ptr->timer.tmCb;   // Hold the callback

            /* Remove the timer node for which timer is elapsed. */
            if(ptr == timerList)
            {
                TimerList* temp = ptr;
                ptr = ptr->next;
                temp->next = NULL;
                free(temp);

                timerList = ptr;
            }
            else
            {
                TimerList* temp = ptr;
                prev->next = ptr->next;
                ptr = ptr->next;
                free(temp);
            }

            /* Callback to the client when timer node is removed. */
            cbHolder();
        }
        else
        {
            /* Update the timers of all nodes accordingly. */
            ptr->timer.expiryTime = ptr->timer.expiryTime + ptr->timer.lastTime - currTime;
            prev = ptr;
            ptr = ptr->next;
        }
    }

    /* Invoke the hardware timer for the refreshed list. */
    uint32_t minTime = findMinTime(timerList);
    if(minTime > 0)
    {
        hw_timer_start(minTime);
        hwTimerInUse = true;
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
    newTimer->timer.expiryTime = duration;
    newTimer->timer.tmCb = tmCb;
    newTimer->next = NULL;
    return(newTimer);
}

/* Used to insert node at the end of the linked list. */
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
            if(ptr->next == NULL)
            {
                ptr->next = timerNode;
                break;  // Insertion at end
            }
        }
    }
    return(head);
}

/* Used to find the minimum duration time from the linked list. */
uint32_t findMinTime(TimerList* head)
{
    if(head == NULL)
    {
        return(0);
    }

    uint32_t minTime = head->timer.expiryTime;
    for(TimerList* ptr = head->next; ptr != NULL; ptr = ptr->next)
    {
        if(minTime > ptr->timer.expiryTime)
        {
            minTime = ptr->timer.expiryTime;
        }
    }

    return(minTime);
}


