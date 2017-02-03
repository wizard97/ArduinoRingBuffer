/*
  RingBuf.h - Library for implementing a simple Ring Buffer on Arduino boards.
  Created by D. Aaron Wisner (daw268@cornell.edu)
  January 17, 2015.
  Released into the public domain.
*/
#ifndef RingBuf_h
#define RingBuf_h

#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <stdint.h>
#endif

#include "SimplyAtomic/SimplyAtomic.h"

// Suppress compier warnings
#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

// Magic Macro constructors //

/* Stack allocate the buffer (bit of a hack, but works)*/
#define RB_STACKALLOC(NAME, SIZE, MAX_ELEMENTS) \
    RingBuf NAME; \
    static uint8_t NAME##_STOR [SIZE*MAX_ELEMENTS] = { 0 }; \
    static bool UNUSED(NAME##_TMP) = RingBuf_init(&NAME, NAME##_STOR, SIZE, MAX_ELEMENTS)

/* Heap allocate the buffer*/
#define RB_HEAPALLOC(NAME, SIZE, MAX_ELEMENTS) \
    RingBuf *NAME = RingBuf_new(SIZE, MAX_ELEMENTS)

/* Delete the Ring Buf, only call when buffer was allocated with HEAPALLOC*/
#define RB_DELETE(NAME) \
    RingBuf_free(NAME)

typedef struct RingBuf RingBuf;

typedef struct RingBuf
{
  uint8_t *_buf;
  size_t _maxElements, _size, _head, _elements;

  //public:
  // Returns true if full
  bool (*isFull) (RingBuf*);
  // Returns true if empty
  bool (*isEmpty) (RingBuf*);
  // Returns number of elemnts in buffer
  size_t (*numElements)(RingBuf*);
  // Add, true on success
  bool (*add) (RingBuf*, const void*);
  // Returns pointer to nth element, NULL when nth element is empty
  void *(*peek) (RingBuf*, size_t);
  // Removes element and copies it to location pointed to by void *
  // Returns pointer passed in, NULL on empty buffer
  bool (*pull) (RingBuf*, void *);

} RingBuf;


#ifdef __cplusplus
extern "C" {
#endif

RingBuf *RingBuf_init(RingBuf *self, uint8_t *buf, size_t size, size_t maxElements);
RingBuf *RingBuf_new(size_t size, size_t maxElements);
bool RingBuf_delete(RingBuf *self);

// private
size_t RingBuf_getTail(RingBuf *self);

// public
bool RingBuf_add(RingBuf *self, const void *object);
void *RingBuf_peek(RingBuf *self, size_t num);
bool RingBuf_pull(RingBuf *self, void *object);
bool RingBuf_isFull(RingBuf *self);
bool RingBuf_isEmpty(RingBuf *self);
size_t RingBuf_numElements(RingBuf *self);

#ifdef __cplusplus
}
#endif


#endif
