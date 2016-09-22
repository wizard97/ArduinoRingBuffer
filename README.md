# ArduinoRingBuffer

This is a simple ring (FIFO) buffer library for the Arduino. It is written in vanilla C, and can easily be modified to work with other platforms.  It can buffer any fixed size object (ints, floats, structs, etc...).

## Project History
I needed a way to buffer sensor events for a group engineering IOT project that I was working on at Cornell. We needed to record changes in IR trip wires that happened in ms timeframes, and tight loop polling was not working. We needed interrupts and a buffering library. I couldn't find any suitable Arduino Libraries that could buffer any sized object, so I wrote my own.

I decided to give object oriented programming a shot using only C (no C++) with this library, of course, it still compiles with C++ compilers such as in the Arduino IDE. Using C structs and function pointers, the library creates RingBuf objects that are complete with their own methods and attributes. Note that every method (except constructor), takes a `RingBuf *self` pointer. This is the equivalent of the `this` pointer in C++, but the C++ compiler automatically passes it behind the scenes. For this library, you must manually pass a the `RingBuf *self` pointer as the first argument, like this:

```c++
char *mystr = "I like C";

RingBuf *buf = RingBuf_new(sizeof(char*), 100);
buf->add(buf, &mystr);
```

## What about C++ templates?

I recently created a C++ alternative to this library that utilizes the power of C++ templates, now you can perform deep copies of objects.
Check it out: [C++ alternative library] (https://github.com/wizard97/Embedded_RingBuf_CPP).

## Use Cases

A ring buffer is used when passing asynchronous io between two threads. In the case of the Arduino, it is very useful for buffering data in an interrupt routine that is later processed in your `void loop()`.

## Supported Platforms
The library currently supports:
- AVR
- ESP8266

## Install

This library is now availible in the Arduino Library Manager, directly in the IDE. Go to `Sketch > Include Library > Manage Libraries` and search for `RingBuf`.

To manually install this library, download this file as a zip, and extract the resulting folder into your Arduino Libraries folder. [Installing an Arduino Library] (https://www.arduino.cc/en/Guide/Libraries).

## Examples

Look at the examples folder for several examples.

## Contributing

If you find this Arduino library helpful, click the Star button, and you will make my day.

Feel free to improve this library. Fork it, make your changes, then submit a pull request!

## API


### Constructor

```c++
RingBuf *RingBuf_new(int size, int len);
```

Creates a new RingBuf object of len elements that are size bytes each. A pointer to the new RingBuf object is returned on success. On failure (lack of memory), a null pointer is returned.
This would be the equivalent of `new RingBuf(int size, int len)` in C++.

### Deconstructor

```c++
int RingBuf_delete(RingBuf *self);
```

Deletes the RingBuf, and frees up all the memory associated with it.

## Methods


### add()

```c++
int add(RingBuf *self, void *object);
```

Append an element to the buffer, where object is a pointer to object you wish to append. Returns -1 on a full buffer. On success, returns the position (index) in the buffer where the element was added.

### peek()

```c++
void *peek(RingBuf *self, unsigned int num);
```

Peek at the num'th element in the buffer. Returns a void pointer to the location of the num'th element. If num is out of bounds or the num'th element is empty, a NULL pointer is returned. Cast the result of this call into a pointer of whatever type you are storing in the buffer. Note that this gives you direct memory access to the location of the num'th element in the buffer, allowing you to directly edit elements in the buffer. Note that while all of RingBuf's public methods are thread safe (including this one), directly using the pointer returned from this method is not thread safe. If there is a possibility an interrupt could fire and remove/modify the item pointed to by the returned pointer, disable interrupts first with `noInterrupts()`, do whatever you need to do with the pointer, then you can reenable interrupts by calling `interrupts()`.

### pull()

```c++
void *pull(RingBuf *self, void *object);
```

Pull the first element out of the buffer. The first element is copied into the location pointed to by object. Returns a NULL pointer if the buffer is empty, otherwise returns object.


### numElements()
```c++
unsigned int numElements(RingBuf *self);
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
