/* FILE: main.c (UNIX/LINUX)
 * Used to control the entire application.
 */

/* Built-In Libraries. */
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

/* Custom Libraries. */
#include "hw_timer.h"
#include "vtimer.h"

// TEST: Start
void* client1_test(void* arg);
void* client2_test(void* arg);
// TEST: End

bool appExecution = false;

/* Function to hanlde the timer application. */
void* timer_application_handler(void* arg)
{
    /* Init. */
    hw_timer_init();
    vtimerInit();

    printf("TIMER APP HANDLER, INIT, DONE\r\n");

    /* Execute. */
    while(true)
    {
        if(appExecution != true)
        {
            printf("TIMER APP HANDLER, EXECUTE, START\r\n");
            appExecution = true;
        }

        hw_timer_execute();
        vtimerExecute();
    }
}

int8_t main()
{
    printf("Timer Application, Main Function\r\n");

    // Test: check the timer for 5 seconds.
    pthread_t appHanlder_thread;
    pthread_t client1_thread;
    pthread_t client2_thread;

    // Start application handler first.
    pthread_create(&appHanlder_thread, NULL, timer_application_handler, NULL);
    printf("Timer Application, Main, App handler thread creatd\r\n");
    
    pthread_create(&client1_thread, NULL, client1_test, NULL);
    //pthread_create(&client2_thread, NULL, client2_test, NULL);
    printf("Timer Application, Main, client threads created\r\n");

    pthread_join(client1_thread, NULL);
    //pthread_join(client2_thread, NULL);
    pthread_join(appHanlder_thread, NULL);

    return(0);
}

// TEST Segment

bool cl1Flag = false;
bool cl2Flag = false;

void test1_cb()
{
    cl1Flag = true;
    printf("\nTIMER for test1 is elapsed\r\n");
    printf("\n");
}

void test2_cb()
{
    cl2Flag = true;
    printf("\nTIMER for test2 is elapsed\r\n");
    printf("\n");
}

void* client1_test(void* timeS)
{
    uint8_t timer = 5;
    vtimerSet(timer, test1_cb);
    printf("CL1, Timer set for %d sec\r\n", timer);

    while(true)
    {
        if(cl1Flag == true)
        {
            cl1Flag = false;
            vtimerSet(timer, test1_cb);
            printf("CL1, Timer set for %d sec\r\n", timer);
        }
    }
}

void* client2_test(void* timeS)
{
    uint8_t timer = 10;
    vtimerSet(timer, test2_cb);
    printf("CL2, Timer set for %d sec\r\n", timer);

    while(true)
    {
        if(cl2Flag == true)
        {
            cl2Flag = false;
            vtimerSet(timer, test2_cb);
            printf("CL2, Timer set for %d sec\r\n", timer);
        }
    }
}


