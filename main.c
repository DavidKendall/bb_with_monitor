/* 
 * main.c
 *
 * Bounded buffer test
 *
 */    

#include <assert.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include "buffer.h"


enum {N_PRODUCERS = 10,
      N_CONSUMERS = 10,
      N_ITERATIONS = 200};

static void *producer(void *arg); 
static void *consumer(void *arg); 
static void beginThreads(void);
static void endThreads(void);
static void randomDelay(void);

static pthread_t thread[N_PRODUCERS + N_CONSUMERS];

int main(int argc, char *argv[]) {
  safeBufferInit();
  beginThreads();
  endThreads();
  printf("\nFINISHED\n");
  return 0;
}

/*
 * The function implementing the producer  behaviour.
 * The parameter 'arg' identifies the producer: 0, 1, 2, ... 
 */
static void *producer(void *arg) {

  message_t msg;
  msg.id = (long)(arg);

  for (int n = 0; n < N_ITERATIONS; n+=1) {
    randomDelay();
    msg.data[0] = rand() % 100;
    safeBufferPut(&msg);
    safeBufferPrint();
  }
  pthread_exit(NULL);        
}

/*
 * The function implementing the consumer behaviour.
 * The parameter 'arg' identifies the consumer: 0, 1, 2, ... 
 */
static void *consumer(void *arg) {

  message_t msg;

  for (int n = 0; n < N_ITERATIONS; n+=1) {
    randomDelay();
    safeBufferGet(&msg);
    safeBufferPrint();
  }
  pthread_exit(NULL);        
}


static void beginThreads(void) {
  int rc;

  for (long i=0; i<N_PRODUCERS; i+=1) {
    rc = pthread_create(&thread[i], NULL,
			producer, (void *)i);
    assert(rc == 0);
  }
  for (long i=0; i<N_CONSUMERS; i+=1) {
    rc = pthread_create(&thread[N_PRODUCERS + i], NULL,
			consumer, (void *)i);
    assert(rc == 0);
  }
}


static void endThreads(void) {
  int rc;

  for (int i=0; i<N_PRODUCERS + N_CONSUMERS; i+=1) {
    rc = pthread_join(thread[i], NULL);
    assert(rc == 0);
  }
}


static void randomDelay(void) {
  struct timespec delay = {0, (rand() % 49 + 1) * 20000000};  

  nanosleep(&delay, NULL);
}
