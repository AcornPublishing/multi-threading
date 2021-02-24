/* 12_sample_system.c

   Create two threads, one byte pool, and one message queue.
   The threads communicate with each other via the message queue.
   Arrays are used for the stacks and the queue storage space */


/****************************************************/
/*    Declarations, Definitions, and Prototypes     */
/****************************************************/

#include  "tx_api.h"
#include  <stdio.h>

#define   STACK_SIZE         1024
#define   QUEUE_SIZE         100
#define   QUEUE_MSG_SIZE     TX_1_ULONG
#define   QUEUE_TOTAL_SIZE   QUEUE_SIZE*sizeof(ULONG)*QUEUE_MSG_SIZE

/* Define thread stacks */
CHAR stack_speedy[STACK_SIZE];
CHAR stack_slow[STACK_SIZE];
CHAR queue_storage[QUEUE_TOTAL_SIZE];

/* Define the ThreadX object control blocks */

TX_THREAD  speedy_thread;
TX_THREAD  slow_thread;

TX_TIMER   stats_timer;

TX_QUEUE   my_queue;


/* Define the counters used in the PROJECT application...  */

ULONG  speedy_thread_counter=0, total_speedy_time=0;
ULONG  slow_thread_counter=0, total_slow_time=0;
ULONG  send_message[QUEUE_MSG_SIZE]={0X0}, received_message[QUEUE_MSG_SIZE];



/* Define thread prototypes.  */

void  speedy_thread_entry(ULONG thread_input);
void  slow_thread_entry(ULONG thread_input);
void  print_stats(ULONG);


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

void  tx_application_define(void *first_unused_memory)
{

   /* Put system definition stuff in here, e.g., thread creates
      and other assorted create information.  */

   /* Create the speedy_thread.  */
   tx_thread_create(&speedy_thread, "speedy_thread",
                    speedy_thread_entry, 0,
                    stack_speedy, STACK_SIZE, 5, 5,
                    TX_NO_TIME_SLICE, TX_AUTO_START);

   /* Create the slow thread */
   tx_thread_create(&slow_thread, "slow_thread",
                    slow_thread_entry, 1,
                    stack_slow, STACK_SIZE, 15, 15,
                    TX_NO_TIME_SLICE, TX_AUTO_START);


   /* Create the message queue used by both threads.  */

   tx_queue_create (&my_queue, "my_queue", QUEUE_MSG_SIZE,
                    queue_storage, QUEUE_TOTAL_SIZE);


   /* Create and activate the timer */
   tx_timer_create (&stats_timer, "stats_timer", print_stats,
                    0x1234, 500, 500, TX_AUTO_ACTIVATE);

}


/****************************************************/
/*              Function Definitions                */
/****************************************************/


/* Entry function definition of the "speedy thread"
   it has a higher priority than the "slow thread" */

void  speedy_thread_entry(ULONG thread_input)
{

UINT  status;
ULONG start_time, cycle_time=0, current_time=0;


   /* This is the higher priority "speedy thread" - it sends
      messages to the message queue */
   while(1)
   {

      /* Get the starting time for this cycle */
      start_time = tx_time_get();

      /* Activity 1:  2 ticks.  */
      tx_thread_sleep(2);

      /* Activity 2:  send a message to the queue, then sleep 5 ticks.  */
      send_message[QUEUE_MSG_SIZE-1]++;

      status = tx_queue_send (&my_queue, send_message, TX_WAIT_FOREVER);

      if (status != TX_SUCCESS)  break;  /* Check status */

      tx_thread_sleep(5);

      /* Activity 3:  4 ticks.  */
      tx_thread_sleep(4);

      /* Activity 4: send a message to the queue, then sleep 3 ticks */
      send_message[QUEUE_MSG_SIZE-1]++;

      status = tx_queue_send (&my_queue, send_message, TX_WAIT_FOREVER);

      if (status != TX_SUCCESS)  break;  /* Check status */

      tx_thread_sleep(3);


      /* Increment the thread counter and get timing info  */
      speedy_thread_counter++;

      current_time = tx_time_get();
      cycle_time = current_time - start_time;
      total_speedy_time = total_speedy_time + cycle_time;

   }
}

/************************************************************/

/* Entry function definition of the "slow thread"
   it has a lower priority than the "speedy thread" */

void    slow_thread_entry(ULONG thread_input)
{

UINT    status;
ULONG   start_time, current_time=0, cycle_time=0;


   /* This is the lower priority "slow thread" - it receives messages
      from the message queuue */
   while(1)
   {

      /* Get the starting time for this cycle */
      start_time = tx_time_get();

      /* Activity 5 - receive a message from the queue and sleep 12 ticks.   */
      status = tx_queue_receive (&my_queue, received_message, TX_WAIT_FOREVER);

      if (status != TX_SUCCESS)  break;  /* Check status */

      tx_thread_sleep(12);

      /* Activity 6:  8 ticks.  */
      tx_thread_sleep(8);

      /* Activity 7:  receive a message from the queue and sleep 11 ticks.   */

      /* receive a message from the queue  */
      status = tx_queue_receive (&my_queue, received_message, TX_WAIT_FOREVER);

      if (status != TX_SUCCESS)  break;  /* Check status */

      tx_thread_sleep(11);

      /* Activity 8:  9 ticks.  */
      tx_thread_sleep(9);

      /* Increment the thread counter and get timing info  */
      slow_thread_counter++;

      current_time = tx_time_get();
      cycle_time = current_time - start_time;
      total_slow_time = total_slow_time + cycle_time;

   }
}

/*****************************************************/
/* print statistics at specified times */
void print_stats (ULONG invalue)
{
   ULONG  current_time, avg_slow_time, avg_speedy_time;

   if ((speedy_thread_counter>0) && (slow_thread_counter>0))
   {
      current_time = tx_time_get();
      avg_slow_time = total_slow_time / slow_thread_counter;
      avg_speedy_time = total_speedy_time / speedy_thread_counter;

      printf("\n**** Chapter 12: 2 threads, 1 message queue, and 1 timer.\n\n");
      printf("     Current Time:               %lu\n", current_time);
      printf("         speedy_thread counter:  %lu\n", speedy_thread_counter);
      printf("        speedy_thread avg time:  %lu\n", avg_speedy_time);
      printf("           slow_thread counter:  %lu\n", slow_thread_counter);
      printf("          slow_thread avg time:  %lu\n", avg_slow_time);
      printf("               # messages sent:  %lu\n\n",
             send_message[QUEUE_MSG_SIZE-1]);
   }
   else printf("Bypassing print_stats function, Current Time: %lu\n",
               tx_time_get());
}
