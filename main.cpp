/*
 * Copyright (c) 2024 Miroslaw Baca
 * AGH - Object-Oriented Programming Language
 */

#include <cstdio>
#include <cstdint>

extern "C" {
#include "MKL05Z4.h"
}

#include "BoardSupport.hpp"
#include "Uart.hpp"
#include "CommunicationModuleMCU.hpp"

extern "C" void setCommunicationObject(CommunicationModuleMCU* obj);

//globalny obiekt do prezentacji
static CommunicationModuleMCU g_comm;


// delay makro
#ifndef DELAY
  #define DELAY(x)  for(uint32_t i = 0; i < (x * 10000U); i++) { __asm("nop"); }
#endif

	//========================= WAZNE! zapisz w doc
	// w projekcie zmienilem Stack_Size      EQU     0x0200 ;TUTAJ ROBILEM ZMIANE ROZMIARU STOSU wartosc oryginalna: 0x00000100
	// oraz zmienilem #define CLOCK_SETUP     1
	// ============================ 
	
int main()
{
    SystemCoreClockUpdate();

    //inicjalizacja peryferiów: LED, I2C, UART
    initLedPins();
    MyI2C::init();
    Uart::init(9600);
		TSI_Init();
	  ADC_Init(); //inicjalizacja ADC
	
    //konfiguracja kto bedzie odbieral znaki
    setCommunicationObject(&g_comm);

    //inicjalizacja wyzszej warstwy 
    g_comm.init();

    
    g_comm.println("\n-> MMA8451 first measurement.");

    MyI2C::writeReg(0x1D, 0x2A, 1);
    MyI2C::writeReg(0x1D, 0x0E, 0x00);

    g_comm.println("Debug 1");

    setLedColor(true, false, false);
		

/*
    while(true)
    {

        DELAY(50);

        //ddczyt suwaka
			uint8_t sliderVal = TSI_ReadSlider();

        //wypisz
        char txt[32];
        sprintf(txt, "Slider = %u\r\n", sliderVal);
        g_comm.print(txt);
    }

*/
    //petla obslugi komend
    while(true)
    {
        char* msg = g_comm.receiveData(); // czeka na input
        g_comm.handleCommand(msg);
				//__WFI();
    }

    return 0;
}
