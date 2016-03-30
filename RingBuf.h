/*
  RingBuf.h - Library for implementing a simple Ring Buffer on Arduino boards.
  Created by D. Aaron Wisner (daw268@cornell.edu)
  January 17, 2015.
  Released into the public domain.
*/
#ifndef RingBuf_h
#define RingBuf_h

#include "Arduino.h"

#ifndef __cplusplus
#ifndef bool
#define bool uint8_t
#endif
#endif

typedef struct RingBuf RingBuf;

typedef struct RingBuf
{
  // Invariant: end and start is always in bounds
  unsigned char *buf;
  unsigned int len, size, start, end, elements;

  // Private:
  int (*next_end_index) (RingBuf*);
  int (*incr_end_index) (RingBuf*);

  int (*incr_start_index) (RingBuf*);

  //public:
  // Returns true if full
  bool (*isFull) (RingBuf*);
  // Returns true if empty
  bool (*isEmpty) (RingBuf*);
  // Returns number of elemnts in buffer
  unsigned int (*numElements)(RingBuf*);
  // Add Event, Returns index where added in buffer, -1 on full buffer
  int (*add) (RingBuf*, const void*);
  // Returns pointer to nth element, NULL when nth element is empty
  void *(*peek) (RingBuf*, unsigned int);
  // Removes element and copies it to location pointed to by void *
  // Returns pointer passed in, NULL on empty buffer
  void *(*pull) (RingBuf*, void *);

} RingBuf;

#ifdef __cplusplus
extern "C" {
#endif

RingBuf *RingBuf_new(int size, int len);
int RingBuf_init(RingBuf *self, int size, int len);
int RingBuf_delete(RingBuf *self);

int RingBufNextEndIndex(RingBuf *self);
int RingBufIncrEnd(RingBuf *self);
int RingBufIncrStart(RingBuf *self);
int RingBufAdd(RingBuf *self, const void *object);
void *RingBufPeek(RingBuf *self, unsigned int num);
void *RingBufPull(RingBuf *self, void *object);
bool RingBufIsFull(RingBuf *self);
bool RingBufIsEmpty(RingBuf *self);
unsigned int RingBufNumElements(RingBuf *self);

#ifdef __cplusplus
}
#endif


// For those of you who cant live without pretty C++ objects....
#ifdef __cplusplus
class RingBufC
{

public:
    RingBufC(int size, int len) { buf = RingBuf_new(size, len); }
    ~RingBufC() { RingBuf_delete(buf); }

    bool isFull() { return RingBufIsFull(buf); }
    bool isEmpty() { return RingBufIsEmpty(buf); }
    unsigned int numElements() { return RingBufNumElements(buf); }

    unsigned int add(const void *object) { return RingBufAdd(buf, object); }
    void *peek(unsigned int num) { return RingBufPeek(buf, num); }
    void *pull(void *object) { return RingBufPull(buf, object); }

    // Use this to check if memory allocation failed
    bool allocFailed() { return !buf; }

private:
    RingBuf *buf;
};
#endif


#endif
