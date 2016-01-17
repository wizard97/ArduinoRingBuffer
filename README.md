# ArduinoRingBuffer

This is a simple ring buffer library for the Arduino. It is written in vanilla C, and can easily be modified to work with other platforms simply be removing the `#include "Arduino.h" `.  


I have heard about object orinted progrming in pure C, and I decided to give it a shot with this library. Using C structs and function pointers, one creates a RingBuf object that is complete with its own methods and attributes.



## Use Cases

A ring buffer is used when passing asynchronous io between two threads. In the case of the Arduino, it is very useful for buffering data in an interrupt routine that is later processed in your `void loop()`.

## API


### Constructor
RingBuf* RingBuf_new(int size, int len);`

Creates a new RingBuf object of len elements that are size bytes each. A poetinter to the new RingBuf object is returned on success. On failure (lack of memory), a null pointer is returned.
This would be the equivalent of `new RingBuf(int size, int len)` in C++.

### Deconstructor

int RingBuf_delete(RingBuf *self);

Deletes the RingBuf, and frees up all the memory associated with it.
