/*******************************************************/
/* file: ports.c                                       */
/* abstract:  This file contains the routines to       */
/*            output values on the JTAG ports, to read */
/*            the TDO bit, and to read a byte of data  */
/*            from the prom                            */
/* Revisions:                                          */
/* 12/01/2008:  Same code as before (original v5.01).  */
/*              Updated comments to clarify instructions.*/
/*              Add print in setPort for xapp058_example.exe.*/
/*******************************************************/
#include "ports.h"
/*#include "prgispx.h"*/
// #include "stm32_gpio.h"
 
static uint32_t port_ = 0;  /* Port vlaues */

/*BYTE *xsvf_data=0;*/

#if 1
inline void DLY()
{
  asm(
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
  );
}
#else
#define DLY()
#endif

void initPorts()
{
  pinMode(TCK, OUTPUT);
  pinMode(TMS, OUTPUT);
  pinMode(TDI, OUTPUT);
  pinMode(TDO, INPUT_PULLUP);
}

/* setPort:  Implement to set the named JTAG signal (p) to the new value (v).*/
/* if in debugging mode, then just set the variables */
void setPort(uint8_t p,uint8_t val)
{
    /* Printing code for the xapp058_example.exe.  You must set the specified
       JTAG signal (p) to the new value (v).  See the above, old Win95 code
       as an implementation example. */
    switch(p)
    {
      case TMS:
        // g_iTMS = val;
        if (val) port_ |= 1u << TMS_PIN; else port_ &= ~(1u << TMS_PIN);
        break;
        
      case TDI:
        // g_iTDI = val;
        if (val) port_ |= 1u << TDI_PIN; else port_ &= ~(1u << TDI_PIN);
        break;
        
      case TCK:
        // g_iTCK = val;
        if (val) port_ |= 1u << TCK_PIN; else port_ &= ~(1u << TCK_PIN);
#if 1      
        // uint32_t r = GPIOB->ODR;
        // if (g_iTMS) r |= 1u << TMS_PIN; else r &= ~(1u << TMS_PIN);
        // if (g_iTDI) r |= 1u << TDI_PIN; else r &= ~(1u << TDI_PIN);
        // if (g_iTCK) r |= 1u << TCK_PIN; else r &= ~(1u << TCK_PIN);
        // GPIOB->ODR = r;
        //GPIOB->ODR = ( GPIOB->ODR & ~( (1u << TMS_PIN) | (1u << TDI_PIN) | (1u << TCK_PIN) ) ) | port_;
        GPIOB->regs->ODR = ( GPIOB->regs->ODR & ~( (1u << TMS_PIN) | (1u << TDI_PIN) | (1u << TCK_PIN) ) ) | port_;

#else
        digitalWrite( TMS, g_iTMS );
        digitalWrite( TDI, g_iTDI );
        digitalWrite( TCK, g_iTCK );
#endif
        DLY();
        break;
        
    }
}


/* toggle tck LH.  No need to modify this code.  It is output via setPort. */
void pulseClock()
{
    setPort(TCK,0);  /* set the TCK port to low  */
    setPort(TCK,1);  /* set the TCK port to high */
}


/* readByte:  Implement to source the next byte from your XSVF file location */
/* read in a byte of data from the prom */
void readByte(uint8_t *data)
{
  *data = read_data();
    /**data=*xsvf_data++;*/
}

/* waitTime:  Implement as follows: */
/* REQUIRED:  This function must consume/wait at least the specified number  */
/*            of microsec, interpreting microsec as a number of microseconds.*/
/* REQUIRED FOR SPARTAN/VIRTEX FPGAs and indirect flash programming:         */
/*            This function must pulse TCK for at least microsec times,      */
/*            interpreting microsec as an integer value.                     */
/* RECOMMENDED IMPLEMENTATION:  Pulse TCK at least microsec times AND        */
/*                              continue pulsing TCK until the microsec wait */
/*                              requirement is also satisfied.               */
void waitTime(uint32_t microsec)
{
#if 0
    static long tckCyclesPerMicrosec    = 72/10; /* must be at least 1 */
    long        tckCycles   = microsec * tckCyclesPerMicrosec;
    long        i;


    /* This implementation is highly recommended!!! */
    /* This implementation requires you to tune the tckCyclesPerMicrosec 
       variable (above) to match the performance of your embedded system
       in order to satisfy the microsec wait time requirement. */
    for ( i = 0; i < tckCycles; ++i )
    {
        pulseClock();
    }
#endif

#if 1
   uint32_t t0 = micros();
   while( micros()-t0 < microsec )
   {
      pulseClock();
   }
#endif

#if 0
    /* Alternate implementation */
    /* For systems with TCK rates << 1 MHz;  Consider this implementation. */
    /* This implementation does not work with Spartan-3AN or indirect flash
       programming. */
    if ( microsec >= 50L )
    {
        /* Make sure TCK is low during wait for XC18V00/XCFxxS */
        /* Or, a running TCK implementation as shown above is an OK alternate */
        setPort( TCK, 0 );

        /* Use Windows Sleep().  Round up to the nearest millisec */
        _sleep( ( microsec + 999L ) / 1000L );
    }
    else    /* Satisfy FPGA JTAG configuration, startup TCK cycles */
    {
        for ( i = 0; i < microsec;  ++i )
        {
            pulseClock();
        }
    }
#endif

#if 0
    /* Alternate implementation */
    /* This implementation is valid for only XC9500/XL/XV, CoolRunner/II CPLDs, 
       XC18V00 PROMs, or Platform Flash XCFxxS/XCFxxP PROMs.  
       This implementation does not work with FPGAs JTAG configuration. */
    /* Make sure TCK is low during wait for XC18V00/XCFxxS PROMs */
    /* Or, a running TCK implementation as shown above is an OK alternate */
    setPort( TCK, 0 );
    /* Use Windows Sleep().  Round up to the nearest millisec */
    u_sleep( ( microsec + 999L ) / 1000L );
#endif
}
