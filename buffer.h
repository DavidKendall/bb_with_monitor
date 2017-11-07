#ifndef __BUFFER_H
#define __BUFFER_H

enum {
  BUF_SIZE = 6UL
};

typedef struct message {
  long id;
  unsigned int data[2];
  float fdata[2];
} message_t;

void put_buffer(message_t const * const);
void get_buffer(message_t * const);
void safe_buffer_init(void);
void safe_buffer_put(message_t const * const msg);
void safe_buffer_get(message_t * const msg);
void safe_buffer_print(void);

#endif
