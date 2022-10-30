/* FILE: main.c (UNIX/LINUX)
 * Used to control the entire application.
 */

/* Built-In Libraries. */
#include <stdio.h>
#include <unistd.h>

/* Custom Libraries. */
#include "hw_timer.h"
#include "vtimer.h"


/* TEST: Function used for testing. */
#ifdef ENABLE_TIMER_TEST
extern void timer_client_test();
#endif

/* Entry point. */
int8_t main()
{
    printf("########################\r\n");
    printf(" APP\t: VIRTUAL TIMER\r\n");
    printf(" AUTHOR\t: Vikrant\r\n");
    printf(" DATE\t: %s\r\n", __DATE__);
    printf(" TIME\t: %s\r\n", __TIME__); 
    printf("########################\r\n");

    /* Init. */
    hw_timer_init();
    vtimerInit();

    printf("\n");
    printf("MAIN, VTIMER APP, INIT, DONE\r\n");

    /* Execute. */
    while(true)
    {
        hw_timer_execute();
        vtimerExecute();

#ifdef ENABLE_TIMER_TEST
        timer_client_test();
#endif
        
        usleep(300);
    }
}
