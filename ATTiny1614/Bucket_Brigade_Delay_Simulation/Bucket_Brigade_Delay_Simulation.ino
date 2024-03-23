
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ATtiny_ISR_Timer.h"

#define BUFFER_SIZE 20

class RingBuffer {
private:
  int buffer[BUFFER_SIZE];
  int head;
  int tail;

public:
  RingBuffer() {
    head = 7;
    tail = 0;
  }

  bool isEmpty() {
    return head == tail;
  }

  bool isFull() {
    return (head + 1) % BUFFER_SIZE == tail;
  }

  void enqueue(int value) {
    if (!isFull()) {
      buffer[head] = value;
      head = (head + 1) % BUFFER_SIZE;
    } else {
      // Buffer overflow, handle error
    }
  }

  int dequeue() {
    if (!isEmpty()) {
      int value = buffer[tail];
      tail = (tail + 1) % BUFFER_SIZE;
      return value;
    } else {
      // Buffer underflow, handle error
      return -1;  // or some other sentinel value
    }
  }

  int peek() {
    if (!isEmpty()) {
      return buffer[tail];
    } else {
      // Buffer is empty, handle error
      return -1;  // or some other sentinel value
    }
  }
};

// Variable definitions
int i = 1;
// MCU pin definitions
// Important - always use the full pin definition i.e. PXn where X is the port letter and n the pin number
const int ledPinBlue = PIN_PA3;// Pin 3 Port A

void setDefaultPinStates() {
  pinMode(ledPinBlue, OUTPUT); //set as output
  digitalWrite(ledPinBlue,HIGH); //start with LED off
  delay(100);
}

RingBuffer fifo;

void setup() {
  setDefaultPinStates();  //run function
  Serial.begin(9600);

  Serial.println();
  Serial.println();



}

void loop() {

  // Enqueue some values
  // for (int i = 0; i < 5; i++) {
  Serial.print(i);
  Serial.print(" ");
  fifo.enqueue(i);
  i++;
  delay(1000);

  // Dequeue and print values
  Serial.print(fifo.dequeue());
  Serial.print(" ");
  delay(1000);
  Serial.println();
}
