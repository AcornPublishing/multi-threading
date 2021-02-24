/* 07_sample_system.c

   Create two threads, and one mutex. 
   Use an array for the thread stacks.
   The mutex protects the critical sections.  */

/****************************************************/
/*    Declarations, Definitions, and Prototypes     */
/****************************************************/

#include   "tx_api.h"
#include   <stdio.h>

#define     STACK_SIZE         1024

CHAR stack_speedy[STACK_SIZE];
CHAR stack_slow[STACK_SIZE];


/* Define the ThreadX object control blocks...  */

TX_THREAD               speedy_thread;
TX_THREAD               slow_thread;

TX_MUTEX                my_mutex;


/* Define thread prototypes.  */

void    speedy_thread_entry(ULONG thread_input);
void    slow_thread_entry(ULONG thread_input);


/****************************************************/
/*               Main Entry Point                   */
/****************************************************/

/* Define main entry point.  */

int main()
{

    /* Enter the ThreadX kernel.  */
    tx_kernel_enter();
}


/****************************************************/
/*             Application Definitions              */
/****************************************************/

/* Define what the initial system looks like.  */

void    tx_application_define(void *first_unused_memory)
{


   /* Put system definitions here,
      e.g., thread and mutex creates */

   /* Create the speedy_thread.  */
   tx_thread_create(&speedy_thread, "speedy_thread",
                    speedy_thread_entry, 0,
                    stack_speedy, STACK_SIZE,
                    5, 5, TX_NO_TIME_SLICE, TX_AUTO_START);

   /* Create the slow thread */
   tx_thread_create(&slow_thread, "slow_thread",
                    slow_thread_entry, 1,
                    stack_slow, STACK_SIZE,
                    15, 15, TX_NO_TIME_SLICE, TX_AUTO_START);

   /* Create the mutex used by both threads  */
   tx_mutex_create(&my_mutex, "my_mutex", TX_NO_INHERIT);
 
}


/****************************************************/
/*              Function Definitions                */
/****************************************************/

/* Define the activities for the Speedy Thread */

void    speedy_thread_entry(ULONG thread_input)
{
UINT    status;
ULONG   current_time;

   while(1)
   {

      /* Activity 1:  2 ticks. */
      tx_thread_sleep(2);

      /* Activity 2:  5 ticks   *** critical section ***
      Get the mutex with suspension. */

      status = tx_mutex_get(&my_mutex, TX_WAIT_FOREVER);
      if (status != TX_SUCCESS)  break;  /* Check status */

      tx_thread_sleep(5);

      /* Release the mutex. */
      status = tx_mutex_put(&my_mutex);
      if (status != TX_SUCCESS)  break;  /* Check status */

      /* Activity 3:  4 ticks. */
      tx_thread_sleep(4);

      /* Activity 4:  3 ticks   *** critical section ***
      Get the mutex with suspension. */

      status = tx_mutex_get(&my_mutex, TX_WAIT_FOREVER);
      if (status != TX_SUCCESS)  break;  /* Check status */

      tx_thread_sleep(3);

      /* Release the mutex. */
      status = tx_mutex_put(&my_mutex);
      if (status != TX_SUCCESS)  break;  /* Check status */

      current_time = tx_time_get();
      printf("Current Time:  %lu  Speedy Thread finished cycle...\n",
              current_time);

   }
}

/****************************************************/

/* Define the activities for the Slow Thread */

void    slow_thread_entry(ULONG thread_input)
{
UINT    status;
ULONG   current_time;

   while(1)
   {

      /* Activity 5:  12 ticks   *** critical section ***
      Get the mutex with suspension. */

      status = tx_mutex_get(&my_mutex, TX_WAIT_FOREVER);
      if (status != TX_SUCCESS)  break;  /* Check status */

      tx_thread_sleep(12);

      /* Release the mutex. */
      status = tx_mutex_put(&my_mutex);
      if (status != TX_SUCCESS)  break;  /* Check status */

      /* Activity 6:  8 ticks. */
      tx_thread_sleep(8);

      /* Activity 7:  11 ticks   *** critical section ***
      Get the mutex with suspension. */

      status = tx_mutex_get(&my_mutex, TX_WAIT_FOREVER);
      if (status != TX_SUCCESS)  break;  /* Check status */

      tx_thread_sleep(11);

      /* Release the mutex. */
      status = tx_mutex_put(&my_mutex);
      if (status != TX_SUCCESS)  break;  /* Check status */

      /* Activity 8:  9 ticks. */
      tx_thread_sleep(9);

      current_time = tx_time_get();
      printf("Current Time:  %lu  Slow Thread finished cycle...\n",
              current_time);

   }
}
