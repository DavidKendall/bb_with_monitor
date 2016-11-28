#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include "buffer.h"

static pthread_mutex_t bufMutex;
static pthread_cond_t fullSlot;
static pthread_cond_t freeSlot;

static message_t buffer[BUF_SIZE];
static uint8_t front = 0;
static uint8_t back = 0;
static uint8_t nFull = 0;

void putBuffer(message_t const * const msg) {
  buffer[back] = *msg;
  back = (back + 1) % BUF_SIZE;
}

void getBuffer(message_t * const msg) {
  *msg = buffer[front];
  front = (front + 1) % BUF_SIZE;
}

void safeBufferInit(void) {
  pthread_mutex_init(&bufMutex, NULL);
  pthread_cond_init(&fullSlot, NULL);
  pthread_cond_init(&freeSlot, NULL);
}

void safeBufferPut(message_t const * const msg) {
  pthread_mutex_lock(&bufMutex);
  while (nFull == BUF_SIZE) {
    pthread_cond_wait(&freeSlot, &bufMutex);
  }
  putBuffer(msg);
  nFull += 1;
  pthread_cond_signal(&fullSlot);
  pthread_mutex_unlock(&bufMutex);
}


void safeBufferGet(message_t * const msg) {
  pthread_mutex_lock(&bufMutex);
  while (nFull == 0) {
    pthread_cond_wait(&fullSlot, &bufMutex);
  }
  getBuffer(msg);
  nFull -= 1;
  pthread_cond_signal(&freeSlot);
  pthread_mutex_unlock(&bufMutex);
}

void safeBufferPrint(void) {
  int i;
  pthread_mutex_lock(&bufMutex);
  for (i=0; i < nFull; i+=1) {
    printf("|%02ld, %03d|", buffer[(front + i) % BUF_SIZE].id, buffer[(front+i) % BUF_SIZE].data[0]);
  }
  printf("\n");
  pthread_mutex_unlock(&bufMutex);
}

