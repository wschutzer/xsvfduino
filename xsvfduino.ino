///
// A XSVF player for the Arduino
//

#if MENU_SERIAL != SerialUSB
  #error "Make sure that the SerialUSB option is selected"
#endif

#define LED PC13

uint8_t xsvf_buf[32]; // 32-bit chunks
int8_t xsvf_buf_ptr = 32, xsvf_buf_len = 0;

#include "ports.h"
#include "micro.h"
#include <stdio.h>
#include <stdarg.h>

void debug_printf(char *format,...)
{
  char buf[256];
  va_list args; 
  va_start(args, format);
  vsprintf(buf, format, args);
  va_end(args);
  Serial.print('G');
  Serial.print(buf);
}

uint8_t read_data()
{
  if (xsvf_buf_ptr < xsvf_buf_len)
    return xsvf_buf[xsvf_buf_ptr++];
  Serial.println("A"); // Todo: send extra info with the ack (debug info?)
  xsvf_buf_len = -1;
  xsvf_buf_ptr = 0;
  while(xsvf_buf_len < 32)
  {
    uint32_t t0 = millis(), t1 = t0;
    while(!Serial.available())
    {
      uint32_t t2 = millis();
      if (t2 - t1 > 500)
      {
        digitalWrite(LED, !digitalRead(LED)); // Toggles the LED
        t1 = t2;
      }
      if (t2 - t0 > 1000) // Timeout waiting for packet
      {
        if (xsvf_buf_ptr < xsvf_buf_len) // Any data on the buffer?
        {
          return xsvf_buf[xsvf_buf_ptr++]; // Yes, return it
        }
        // else
        // {
        //  Serial.println('A'); // Buffer stil empty, ask for more
        //  t0 = millis();
        // }
      }
      yield();
    }
    uint8_t c = Serial.read();
    if (xsvf_buf_len < 0)
    {
      if (c == 'D')  // Data packet TODO: packet numbering and CRC checking
        xsvf_buf_len = 0; // Receive the rest of the packet
      if (c == 'R')
        Serial.println("A");
    }
    else
    {
      if (xsvf_buf_len < 32)
      {
        xsvf_buf[xsvf_buf_len++] = c;      
      }
    }
  }
  return xsvf_buf[xsvf_buf_ptr++];
}

void u_sleep( uint32_t m )
{
  delayMicroseconds( m );
}


void setup() 
{
  initPorts();
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  Serial.begin(115200);
  Serial.println("A");
}

int cnt = 1;
String buf;

void loop() 
{
  xsvfExecute();
}
