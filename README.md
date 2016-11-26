# ArduinoRingBuffer

This is a simple ring (FIFO) buffer library for the Arduino. It is written in vanilla C, and can easily be modified to work with other platforms.  It can buffer any fixed size object (ints, floats, structs, etc...).

## Project History
I needed a way to buffer sensor events for a group engineering IOT project that I was working on at Cornell. We needed to record changes in IR trip wires that happened in ms timeframes, and tight loop polling was not working. We needed interrupts and a buffering library. I couldn't find any suitable Arduino Libraries that could buffer any sized object, so I wrote my own.

I decided to give object oriented programming a shot using only C (no C++) with this library, of course, it still compiles with C++ compilers such as in the Arduino IDE. Using C structs and function pointers, the library creates RingBuf objects that are complete with their own methods and attributes. Note that every method (except constructor), takes a `RingBuf *self` pointer. This is the equivalent of the `this` pointer in C++, but the C++ compiler automatically passes it behind the scenes. For this library, you must manually pass a the `RingBuf *self` pointer as the first argument, like this:


```c++
char *mystr = "I like C";

RINGBUF_STACKALLOC(sizeof(char *), 10);
buf->add(buf, &mystr);
```

## FAQ's
 <dl>
 <dt>Can I buffer C++ objects?</dt>
   <dd>The library only shallow copies objects into the buffer, it will not call the copy constructor. For many C++ objects this works fine, but if you require a deep copy you will have to look into libraries that supports something like C++ templates. And to be honest, you shouldn't be doing deep copies on a microcontroller or you could get random freezes from memory fragmentation.</dd>
 </dl>

## What about C++ templates?

I recently created a C++ alternative to this library that utilizes the power of C++ templates, now you can perform deep copies of objects.
Check it out: [C++ alternative library] (https://github.com/wizard97/Embedded_RingBuf_CPP).

## Use Cases

A ring buffer is used when passing asynchronous io between two threads. In the case of the Arduino, it is very useful for buffering data in an interrupt routine that is later processed in your `void loop()`.

## Supported Platforms
The library currently supports:
- AVR
- ESP8266
- Any other platform with a C compiler (you should implement the `RB_ATOMIC_START` and `RB_ATOMIC_END` macros)

## Install

This library is now available in the Arduino Library Manager, directly in the IDE. Go to `Sketch > Include Library > Manage Libraries` and search for `RingBuf`.

To manually install this library, download this file as a zip, and extract the resulting folder into your Arduino Libraries folder. [Installing an Arduino Library] (https://www.arduino.cc/en/Guide/Libraries).

## Examples

Look at the examples folder for several examples.

## Contributing

If you find this Arduino library helpful, click the Star button, and you will make my day.

Feel free to improve this library. Fork it, make your changes, then submit a pull request!

## API


### Constructor


```c++
// Heap allocate
RINGBUF_STACKALLOC(NAME, size size_t, size_t maxElements)
```
Create a new RingBuf object that can hold up to maxElements that are size bytes each. Note that memory is allocated from the stack, this is the preferred way to create it. A RingBuf object called `NAME` is created (`RingBuf`).

```c++
// Heap allocate
RINGBUF_HEAPALLOC(NAME, size size_t, size_t maxElements)
```
Allocate a new RingBuf object that can hold up to MAX_ELEMENTS that are size bytes each. Note that memory is allocated at run time from the heap.
A pointer to the new RingBuf object called `NAME` is created (`RingBuf *`). On failure (lack of memory), NAME is set to NULL. This would be the equivalent of using `new` in C++.

### Deconstructor

```c++
RINGBUF_DELETE(RingBuf *rb)
```

Deletes the RingBuf, and frees up all the memory associated with it. Only call if the RingBuf was created with `RINGBUF_HEAPALLOC()`

## Methods


### add()

```c++
bool add(RingBuf *self, const void *object)
```

Append an element to the buffer, where object is a pointer to object you wish to append. Returns true on success, false on a full buffer.

### peek()

```c++
void *peek(RingBuf *self, size_t index)
```

Peek at the element at index in the buffer. Returns a void pointer to the location element. If index is out of bounds or empty, a NULL pointer is returned. Cast the result of this call into a pointer of whatever type you are storing in the buffer. Note that this gives you direct memory access to the location of the element at index in the buffer, allowing you to directly edit elements in the buffer. Note that while all of RingBuf's public methods are thread safe (including this one), directly using the pointer returned from this method is not thread safe. If there is a possibility an interrupt could fire and remove/modify the item pointed to by the returned pointer, disable interrupts first with `noInterrupts()`, do whatever you need to do with the pointer, then you can reenable interrupts by calling `interrupts()`.

### pull()

```c++
bool pull(RingBuf *self, void *object)
```

Pull the first element out of the buffer. The first element is copied into the location pointed to by object. Returns true on success, false if the buffer is empty.


### numElements()
```c++
size_t numElements(RingBuf *self)
```

Returns number of elements in buffer.

### isFull()
```c++
bool isFull(RingBuf *self);
```

Returns true if buffer is full, otherwise false.


### isEmpty()

```c++
bool isEmpty(RingBuf *self);
```

Returns true if buffer is empty, false otherwise.

## License

This library is open-source, and licensed under the [MIT license](http://opensource.org/licenses/MIT). Do whatever you like with it, but contributions are appreciated.
