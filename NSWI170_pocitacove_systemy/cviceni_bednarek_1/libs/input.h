#ifndef SERIAL_INPUT_HANDLER_H
#define SERIAL_INPUT_HANDLER_H

#include "funshield.h"

/**
 * Encapsulates serial input and buffers the messages.
 */
struct SerialInputHandler
{
  String currentMessage = "Hello World"; // message being displayed (handles allocation)
  String nextMessage = "Hello World"; // message ready to be swapped for current message
  String readingBuffer; // raw reading buffer holds the last line of the serial input

  SerialInputHandler()
  {
    currentMessage = "Hello World";                  // Message being displayed (initialy led by 3 spaces for cooler message entrance effect) 
    nextMessage = "Hello World";
    readingBuffer = "";    
  }

  /**
   * Initializes the serial connection.
   * Must be invoked in setup().
   */
  void initialize()
  {
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB
    }    
  }
  
  /**
   * Fetches current message as C string pointer.
   * The pointer is guaranteed to be valid until the next invocation of getMessage().
   * @return message to be displayed
   */
  const char* getMessage()
  {
    currentMessage = String(nextMessage);
    return currentMessage.c_str();
  }

  /**
   * Handles reading of serial input.
   * Should be called in every loop().
   */
  void updateInLoop()
  {
    while (Serial.available()) {
      char ch = (char)Serial.read();
      if (ch == '\n') {
        nextMessage = String(readingBuffer);
        readingBuffer = "";
      } else {
        readingBuffer += ch;
      }
    }
  }
};

#endif
