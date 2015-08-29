#define F_CPU 3579545UL

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <math.h>

#include "avr_compiler.h"
#include "rtc_driver.h"
#include "uart.h"
#include "usart_driver.h"
#include "clksys_driver.h"

static void InitMainClockPll(void);
static void EnableAllInterupts(void);
static void InitRfOutput(void);
static void SetRfOutput(uint8_t set);

int main(void)
{
  PORTE.DIRSET = PIN0_bm;

  InitMainClockPll();
  InitRfOutput();
  EnableAllInterupts();

  while(1)
  {
    SetRfOutput(1);
    _delay_ms(500);
    SetRfOutput(0);
    _delay_ms(500);
  }
}


static void InitMainClockPll(void)
{
  CLKSYS_XOSC_Config( OSC_FRQRANGE_2TO9_gc, 0, OSC_XOSCSEL_XTAL_16KCLK_gc );
  CLKSYS_Enable( OSC_XOSCEN_bm );
  do {} while ( CLKSYS_IsReady( OSC_XOSCRDY_bm ) == 0 );

  PORTE.OUTSET = PIN0_bm;

  CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_XOSC_gc );
  CLKSYS_Disable( OSC_RC2MEN_bm );
  CLKSYS_Disable( OSC_RC32MEN_bm );
}

static void EnableAllInterupts(void)
{
  PMIC.CTRL |= PMIC_LOLVLEN_bm;
  PMIC.CTRL |= PMIC_MEDLVLEN_bm;
  PMIC.CTRL |= PMIC_HILVLEN_bm;
  sei();
}

static void InitRfOutput(void)
{
  PORTC.DIRSET = PIN7_bm;
}

static void SetRfOutput(uint8_t set)
{
  if(set)
  {
    PORTCFG.CLKEVOUT = PORTCFG_CLKOUT_PC7_gc;
    PORTE.OUTSET = PIN0_bm;
  }
  else
  {
    PORTCFG.CLKEVOUT = PORTCFG_CLKOUT_OFF_gc;
    PORTE.OUTCLR = PIN0_bm;
  }
}