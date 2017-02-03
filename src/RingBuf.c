/*
RingBuf.c - Library for implementing a simple Ring Buffer on Arduino boards.
Created by D. Aaron Wisner (daw268@cornell.edu)
January 17, 2016.
Released into the public domain.
*/
#include "RingBuf.h"
#include <string.h>


/////// Constructor //////////
RingBuf *RingBuf_new(size_t size, size_t maxElements)
{
    RingBuf *self = (RingBuf *)malloc(sizeof(RingBuf) + size*maxElements);
    if (!self) return NULL;
    memset(self, 0, sizeof(RingBuf) + size*maxElements);
    if (!RingBuf_init(self, (uint8_t *)self + sizeof(RingBuf), size, maxElements))
    {
        free(self);
        return NULL;
    }
    return self;
}

RingBuf *RingBuf_init(RingBuf *self, uint8_t *buf, size_t size, size_t maxElements)
{
    //First thing after ringbuf struct is buffer
    self->_buf = buf;

    memset(self->_buf, 0, size*maxElements);
    self->_size = size;
    self->_maxElements = maxElements;

    self->_head = 0;
    self->_elements = 0;

    //initlize the virtual public methods
    self->isFull = &RingBuf_isFull;
    self->isEmpty = &RingBuf_isEmpty;
    self->add = &RingBuf_add;
    self->numElements = &RingBuf_numElements;
    self->peek = &RingBuf_peek;
    self->pull = &RingBuf_pull;
    return self;
}

/////// Deconstructor //////////
bool RingBuf_delete(RingBuf *self)
{
    free(self);
    return true;
}

/////// PRIVATE METHODS //////////

// Calculate poistion of tail
size_t RingBuf_getTail(RingBuf *self)
{
    return (self->_head + (self->_maxElements - self->_elements))%self->_maxElements;
}

/////// PUBLIC METHODS //////////

// Add an object struct to RingBuf
bool RingBuf_add(RingBuf *self, const void *object)
{
    // Perform all atomic opertaions
    bool full = self->isFull(self);

    ATOMIC()
    {
        //if not full
        if (!full)
        {
            memcpy(&self->_buf[self->_size*self->_head], object, self->_size);
            self->_head = (self->_head + 1)%self->_maxElements;
            self->_elements++;
        }
    }

    return !full;
}

// Return pointer to the element at index, return null on empty or out of bounds
void *RingBuf_peek(RingBuf *self, size_t index)
{
    void *ret = NULL;
    // Perform all atomic opertaions
    ATOMIC()
    {
        //empty or out of bounds
        if (self->isEmpty(self) || index > self->_elements - 1) ret = NULL;
        else ret = &self->_buf[((RingBuf_getTail(self) + index)%self->_maxElements)*self->_size];
    }

    return ret;
}

// Returns and removes first buffer element
bool RingBuf_pull(RingBuf *self, void *object)
{
    bool ret = false;
    // Perform all atomic opertaions
    ATOMIC()
    {
        if (!self->isEmpty(self)) {
            memcpy(object, &self->_buf[RingBuf_getTail(self)*self->_size], self->_size);
            self->_elements--;
            ret = true;
        }
    }

    return ret;
}

// Returns number of elemnts in buffer
size_t RingBuf_numElements(RingBuf *self)
{
    size_t elements;

    // Perform all atomic opertaions
    ATOMIC()
    {
        elements = self->_elements;
    }

    return elements;
}

// Returns true if buffer is full
bool RingBuf_isFull(RingBuf *self)
{
    bool ret;

    // Perform all atomic opertaions
    ATOMIC()
    {
        ret = self->_elements == self->_maxElements;
    }

    return ret;
}

// Returns true if buffer is empty
bool RingBuf_isEmpty(RingBuf *self)
{
    bool ret;

    // Perform all atomic opertaions
    ATOMIC()
    {
        ret = !self->_elements;
    }

    return ret;
}
