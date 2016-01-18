/*
  RingBufExample.ino - A basic demo of adding a struct, then removing one
  Created by D. Aaron Wisner (daw268@cornell.edu)
  January 17, 2015.
  Released into the public domain.
*/

#include <RingBuf.h>

struct mystruct
{
  int index;
  char randomstring[10];
  unsigned int mycrap;
  unsigned long long timestamp;
};

// Create a RinBuf object designed to hold a 20 of mystructs
RingBuf *my_buf = RingBuf_new(sizeof(struct mystruct), 20);


void setup() {
  Serial.begin(9600);
  // Check if null pointer
  if (!my_buf)
  {
    Serial.println("Not enough memory");
    while (1);
  }

  // Create element I want to add
  struct mystruct bar, foo;

  // Zero both of them out
  memset(&bar, 0, sizeof(struct mystruct));
  memset(&foo, 0, sizeof(struct mystruct));

  bar.index = 1;
  bar.timestamp = millis();

  // Copy bar into the ring_buf
  my_buf->add(my_buf, &bar);

  // Pull first element out of buffer (should be same as bar) into foo
  my_buf->pull(my_buf, &foo);

  // Prove that foo is the same as bar
  if (!memcmp(&bar, &foo, sizeof(struct mystruct)))
  {
    Serial.println("As expected, foo is a copy of bar");
  }

  // Delete RingBuf object
  RingBuf_delete(my_buf);
  my_buf = NULL;

}



void loop() {
  // put your main code here, to run repeatedly:

}
