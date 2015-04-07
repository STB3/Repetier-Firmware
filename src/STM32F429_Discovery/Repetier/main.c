//--------------------------------------------------------------
// File     : main.c
// Datum    : 24.10.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F429
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der SDRAM-Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
// In Configuration diese Define hinzufügen :
// "STM32F429_439xx" , "__ASSEMBLY__" , "USE_STDPERIPH_DRIVER"
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_sdram.h"
#include "stm32_ub_led.h"

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}

int main(void)
{
  uint32_t adr;
  uint8_t sollwert, istwert;
  ErrorStatus ramcheck;

  SystemInit(); // Quarz Einstellungen aktivieren

  // init der LEDs
  UB_Led_Init();

  // init vom externen SDRAM (und test ob es vorhanden ist)
  ramcheck=UB_SDRAM_Init();
  if(ramcheck==SUCCESS) {
    // zum test RAM komplett füllen
    sollwert=0xA1B2;
    for(adr=0;adr<SDRAM_MAX_ADR;adr+=2) {
   	  UB_SDRAM_Write16b(adr,sollwert);
      sollwert++;
    }

    // Ram wieder auslesen und prüfen
    sollwert=0xA1B2;
    for(adr=0;adr<SDRAM_MAX_ADR;adr+=2) {
      istwert=UB_SDRAM_Read16b(adr);
      if(istwert!=sollwert) {
        // RAM-Inhalt fehlerhaft
        ramcheck=ERROR;
      }
      sollwert++;
    }
  }

  while(1)
  {
    if(ramcheck==SUCCESS) {
      // wenn kein Fehler -> gruene LED leuchtet
      UB_Led_On(LED_GREEN);
    }
    else {
      // wenn Fehler -> rote LED blinkt
      UB_Led_Toggle(LED_RED);
      Delay(5000000);
    }
  }
}

