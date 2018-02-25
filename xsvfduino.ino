///
// An XSVF player for the Arduino
//
// Tested with the STM32 Arduino clone known as the Blue Pill
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
#include <setjmp.h>

bool ongoing = false; // Is there an ongoing transmission?
jmp_buf glb_jmp_buf;

void debug_printf(char *format,...)
{
  char buf[256];
  va_list args; 
  va_start(args, format);
  vsprintf(buf, format, args);
  va_end(args);
  Serial.print('G');
  Serial.print(buf);
  Serial.flush();
  yield();
  if ( !strncmp(buf, "ERROR", 5) )
    longjmp(glb_jmp_buf, 1);
}

uint8_t read_data()
{
  if (xsvf_buf_ptr < xsvf_buf_len)
    return xsvf_buf[xsvf_buf_ptr++];
  if (ongoing)
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
        else
          if (ongoing && t2 - t0 > 3000)
            {
              Serial.print("QINFO: Timeout waiting for a packet. (End of Data?)\n");
              Serial.flush();
              yield();
              longjmp(glb_jmp_buf, 1); // Waiting for too long, reset the engine
            }
      }
      yield();
    }
    uint8_t c = Serial.read();
    ongoing = true;
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
  ongoing = false;
  if ( !setjmp(glb_jmp_buf) )
    xsvfExecute();
}
