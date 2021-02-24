/* 10a_sample_system.c

   Create two threads, and one mutex. 
   Use arrays for the thread stacks.
   A binary semaphore protects the critical sections.
   Use an application timer to display thread timings. */

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

TX_SEMAPHORE            my_semaphore;

/* Declare the application timer */
TX_TIMER        stats_timer;

/* Declare the counters and accumulators */
ULONG           speedy_thread_counter = 0,
                total_speedy_time = 0;
ULONG           slow_thread_counter = 0,
                total_slow_time = 0;

/* Define prototype for expiration function */
void    print_stats(ULONG);

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
      e.g., thread and semaphore creates */

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

   /* Create the binary semaphore used by both threads  */
   tx_semaphore_create(&my_semaphore, "my_semaphore", 1);

   /* Create and activate the timer */
   tx_timer_create (&stats_timer, "stats_timer", print_stats,
                    0x1234, 500, 500, TX_AUTO_ACTIVATE);
 
}

/****************************************************/
/*              Function Definitions                */
/****************************************************/

/* Define the activities for the Speedy Thread */

void    speedy_thread_entry(ULONG thread_input)
{
UINT    status;
ULONG   current_time;
ULONG   start_time, cycle_time;

   while(1)
   {

      /* Get the starting time for this cycle */
      start_time = tx_time_get();

      /* Activity 1:  2 ticks. */
      tx_thread_sleep(2);

      /* Activity 2:  5 ticks   *** critical section ***
      Get an instance of the binary semaphore with suspension. */

      status = tx_semaphore_get(&my_semaphore, TX_WAIT_FOREVER);
      if (status != TX_SUCCESS)  break;  /* Check status */

      tx_thread_sleep(5);

      /* Place an instance in the binary semaphore. */
      status = tx_semaphore_put(&my_semaphore);
      if (status != TX_SUCCESS)  break;  /* Check status */

      /* Activity 3:  4 ticks. */
      tx_thread_sleep(4);

      /* Activity 4:  3 ticks   *** critical section ***
      Get an instance of the binary semaphore with suspension. */

      status = tx_semaphore_get(&my_semaphore, TX_WAIT_FOREVER);
      if (status != TX_SUCCESS)  break;  /* Check status */

      tx_thread_sleep(3);

      /* Place an instance in the binary semaphore. */
      status = tx_semaphore_put(&my_semaphore);
      if (status != TX_SUCCESS)  break;  /* Check status */

      /* Increment thread counter, compute cycle time & total time */
      speedy_thread_counter++;
      current_time = tx_time_get();
      cycle_time = current_time - start_time;
      total_speedy_time = total_speedy_time + cycle_time;

   }
}

/****************************************************/

/* Define the activities for the Slow Thread */

void    slow_thread_entry(ULONG thread_input)
{
UINT    status;
ULONG   current_time;
ULONG   start_time, cycle_time;

   while(1)
   {

      /* Get the starting time for this cycle */
      start_time = tx_time_get();

      /* Activity 5:  12 ticks   *** critical section ***
      Get an instance of the binary semaphore with suspension. */

      status = tx_semaphore_get(&my_semaphore, TX_WAIT_FOREVER);
      if (status != TX_SUCCESS)  break;  /* Check status */

      tx_thread_sleep(12);

      /* Place an instance in the binary semaphore. */
      status = tx_semaphore_put(&my_semaphore);
      if (status != TX_SUCCESS)  break;  /* Check status */

      /* Activity 6:  8 ticks. */
      tx_thread_sleep(8);

      /* Activity 7:  11 ticks   *** critical section ***
      Get an instance of the binary semaphore with suspension. */

      status = tx_semaphore_get(&my_semaphore, TX_WAIT_FOREVER);
      if (status != TX_SUCCESS)  break;  /* Check status */

      tx_thread_sleep(11);

      /* Place an instance in the binary semaphore. */
      status = tx_semaphore_put(&my_semaphore);
      if (status != TX_SUCCESS)  break;  /* Check status */

      /* Activity 8:  9 ticks. */
      tx_thread_sleep(9);
      
      /* Increment thread counter, compute cycle time & total time */
      slow_thread_counter++;
      current_time = tx_time_get();
      cycle_time = current_time - start_time;
      total_slow_time = total_slow_time + cycle_time;

   }
}

/****************************************************/

/* Display statistics at periodic intervals */

void print_stats (ULONG invalue)
{
   ULONG   current_time, avg_slow_time, avg_speedy_time;

   if ((speedy_thread_counter>0) && (slow_thread_counter>0))
   {
      current_time = tx_time_get();
      avg_slow_time = total_slow_time / slow_thread_counter;
      avg_speedy_time = total_speedy_time / speedy_thread_counter;

      printf("\n**** Timing Info Summary\n\n");
      printf("Current Time:               %lu\n", current_time);
      printf("  speedy_thread counter:  %lu\n", speedy_thread_counter);
      printf(" speedy_thread avg time:  %lu\n", avg_speedy_time);
      printf("    slow_thread counter:  %lu\n", slow_thread_counter);
      printf("   slow_thread avg time:  %lu\n\n", avg_slow_time);
   }
   else printf("Bypassing print_stats, Time: %lu\n", tx_time_get());
}
