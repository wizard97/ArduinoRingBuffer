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

#ifndef __cplusplus
#ifndef bool
    #define bool uint8_t
#endif
#endif


#ifdef ARDUINO

    #if defined(ARDUINO_ARCH_AVR)
        #include <util/atomic.h>
        #define RB_ATOMIC_START ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        #define RB_ATOMIC_END }


    #elif defined(ARDUINO_ARCH_ESP8266)
        #ifndef __STRINGIFY
            #define __STRINGIFY(a) #a
        #endif

        #ifndef xt_rsil
            #define xt_rsil(level) (__extension__({uint32_t state; __asm__ __volatile__("rsil %0," __STRINGIFY(level) : "=a" (state)); state;}))
        #endif

        #ifndef xt_wsr_ps
            #define xt_wsr_ps(state)  __asm__ __volatile__("wsr %0,ps; isync" :: "a" (state) : "memory")
        #endif

        #define RB_ATOMIC_START do { uint32_t _savedIS = xt_rsil(15) ;
        #define RB_ATOMIC_END xt_wsr_ps(_savedIS); } while(0);

    #else
        #define RB_ATOMIC_START {
        #define RB_ATOMIC_END }
        #warning “This library only fully supports AVR and ESP8266 Boards.”
        #warning "Operations on the buffer in ISRs are not safe!"
    #endif

#else
    #define RB_ATOMIC_START {
    #define RB_ATOMIC_END }
    #warning "Operations on the buffer in ISRs are not safe!"
    #warning "Impliment RB_ATOMIC_START and RB_ATOMIC_END macros for safe ISR operation!"
#endif



// Magic Macro constructors //

/* Stack allocate the buffer (bit of a hack, but works)*/
#define RINGBUF_STACKALLOC(NAME, SIZE, MAX_ELEMENTS) \
    RingBuf NAME; \
    static uint8_t NAME##_STOR [SIZE*MAX_ELEMENTS] = { }; \
    bool NAME##_TMP = RingBuf_init(&NAME, NAME##_STOR, SIZE, MAX_ELEMENTS)

/* Heap allocate the buffer*/
#define RINGBUF_HEAPALLOC(NAME, SIZE, MAX_ELEMENTS) \
    RingBuf *NAME = RingBuf_new(SIZE, MAX_ELEMENTS)

/* Delete the Ring Buf, only call when buffer was allocated with HEAPALLOC*/
#define RINGBUF_DELETE(NAME) \
    RingBuf_free(NAME)

typedef struct RingBuf RingBuf;

typedef struct RingBuf
{
  // Invariant: end and start is always in bounds
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
