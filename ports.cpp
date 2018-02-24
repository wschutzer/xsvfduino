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

 
// #include "stdio.h"
// extern FILE *in;
static uint8_t  g_iTCK = 0; /* For xapp058_example .exe */
static uint8_t  g_iTMS = 0; /* For xapp058_example .exe */
static uint8_t  g_iTDI = 0; /* For xapp058_example .exe */

/*BYTE *xsvf_data=0;*/


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
        g_iTMS = val;
        break;
      case TDI:
        g_iTDI = val;
        break;
      case TCK:
        g_iTCK = val;
        digitalWrite(TDI, g_iTDI); // Output signals are set right before the clock is set.
        digitalWrite(TMS, g_iTMS);
        digitalWrite(TCK, val);
        // printf( "TCK = %d;  TMS = %d;  TDI = %d\n", g_iTCK, g_iTMS, g_iTDI );
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
    static long tckCyclesPerMicrosec    = 1; /* must be at least 1 */
    long        tckCycles   = microsec * tckCyclesPerMicrosec;
    long        i;

#if 0
    /* This implementation is highly recommended!!! */
    /* This implementation requires you to tune the tckCyclesPerMicrosec 
       variable (above) to match the performance of your embedded system
       in order to satisfy the microsec wait time requirement. */
    for ( i = 0; i < tckCycles; ++i )
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

#if 1
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
