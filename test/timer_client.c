/* FILE: timer_client.c
 * Used to test the application.
 */

/* Built-In Libraries. */
#include <stdio.h>
#include <stdbool.h>

/* Custom Libraries. */
#include "vtimer.h"


/* Function to test the application. */
void timer_client_test();

/* TEST SEGMENT. */
bool timer1_flag = false;
bool timer2_flag = false;

void timer1_callback()
{
    timer1_flag = false;
    printf("Timer1 elapsed\r\n");
    printf("\n");
}

void timer2_callback()
{
    timer2_flag = false;
    printf("Timer2 elapsed\r\n");
    printf("\n");
}

void timer_client_test()
{
    uint8_t timer1 = 20;
    uint8_t timer2 = 30;

    if(timer1_flag == false && 0 == vtimerSet(timer1, timer1_callback))
    {
        timer1_flag = true;
        printf("Timer1 set for %d sec\r\n", timer1);
    }

    if(timer2_flag == false && 0 == vtimerSet(timer2, timer2_callback))
    {
        timer2_flag = true;
        printf("Timer2 set for %d sec\r\n", timer2);
    }
}


