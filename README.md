# ArduinoRingBuffer

This is a simple ring buffer library for the Arduino. It is written in vanilla C, and can easily be modified to work with other platforms simply be removing the `#include "Arduino.h" `.  


I have heard about object orinted progrming in pure C, and I decided to give it a shot with this library. Using C structs and function pointers, one creates a RingBuf object that is complete with its own methods and attributes.



## Use Cases

A ring buffer is used when passing asynchronous io between two threads. In the case of the Arduino, it is very useful for buffering data in an interrupt routine that is later processed in your `void loop()`.

## API


### Constructor

```
RingBuf *RingBuf_new(int size, int len);
```

Creates a new RingBuf object of len elements that are size bytes each. A poetinter to the new RingBuf object is returned on success. On failure (lack of memory), a null pointer is returned.
This would be the equivalent of `new RingBuf(int size, int len)` in C++.

### Deconstructor

```
int RingBuf_delete(RingBuf *self);
```

Deletes the RingBuf, and frees up all the memory associated with it.

## Methods


### add()

```
int add(RingBuf *self, void *object);
```

Append an element to the buffer, where object is a pointer to object you wish to append. Returns -1 on a full buffer. On success, returns the position (index) in the buffer where the element was added.

### peek()

```
void *peek(RingBuf *self, unsigned int num);
```

Peek at the num'th element in the buffer. Returns a void pointer to the location of the num'th element. If num is out of bounds or the num'th element is empty, a NULL pointer is returned. Cast the result of this call into a pointer of whatever type you are storing in the buffer. Note that this gives you direct memory access to the location of the num'th element in the buffer, allowing you to directly edit elements in the buffer.

### pull()

```
void *pull(RingBuf *self, void *object);
```

Pull the first element out of the buffer. The first element is copied into the location pointed to by object. Returns a NULL pointer if the buffer is empty, otherwise returns object.


### isFull()
```
bool isFull(RingBuf *self);
```

Returns true if buffer is full, otherwise false.


### isEmpty()

```
bool isEmpty(RingBuf *self);
```

Returns true if buffer is empty, false otherwise.
