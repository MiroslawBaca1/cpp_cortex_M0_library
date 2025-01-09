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

	
	static char temp[2256]; // Zwiekszenie bufora

static uint8_t readout = 0;
static uint8_t arrayXYZ[6];
	
	
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

    setLedColor(true, true, true);
		//DELAY(1500);
		setLedColor(true, false, false);
		
/*

    for (int testy=0; testy<=20; testy++)
    {

        DELAY(500);

        //ddczyt suwaka
			uint8_t sliderVal = TSI_ReadSlider();

        //wypisz
        char txt[32];
        sprintf(txt, "Slider = %u\r\n", sliderVal);
        g_comm.print(txt);
    }
		
		g_comm.println("ZMIANA ZA 2 SEK!");
		DELAY(2000);
		g_comm.println("ZMIANA!");
		self_calibration();
		DELAY(200);
		g_comm.println("PO!");
		DELAY(2000);
		
		 while(true)
    {

        DELAY(100);

        //ddczyt suwaka
			uint8_t sliderVal = TSI_ReadSlider();

        //wypisz
        char txt[32];
        sprintf(txt, "Slider = %u\r\n", sliderVal);
        g_comm.print(txt);
    }
		*/

    //petla obslugi komend
		
		
		    int stepsWalk = 0;
    int stepsRun  = 0;

    double accelerationVectorOld = 1.0;
		
	
		g_comm.println("\n-> MMA8451 first measurement.2");
	
		MyI2C::writeReg(0x1D, 0x2A, 1);//========================== :) =======================	 		// Set ACTIVE bit in CTRL_REG1 in MMA8451
		MyI2C::writeReg(0x1D, 0x0E, 0x00); // aktywuje pomiar dla 2g
	
	
		MyI2C::readReg(0x1D, 0x05, &readout); //========================== :) =======================// Get OUT_Z_MSB from MMA8451 (MSB 8 bits from 14 in total)
		
		sprintf(temp,"Gravity: %+1.2fg", ((double)((int16_t)(readout<<8)>>2)/4096)); // default 4096 counts/g sensitivity
		g_comm.println(temp);
	
	
    // Wiadomosc startowa
    g_comm.println("Starting measurement in 2s...");

		for(int xjdd = 2; xjdd >= 1; xjdd--) 
    {
        // Petla "pomiarowa"
        for(int xjd = 20; xjd >= 1; xjd--) 
        {
            // Odczyt 16 bajtów rejestrów z sensora (np. MMA8451, MMA8652, itp.)
            // Adres 0x1D, rejestr startowy 0x01, 16 bajtów -> arrayXYZ
            MyI2C::readRegBlock(0x1D, 0x01, 6, arrayXYZ);




					






					
					
								double x2_=((double)((int16_t)((arrayXYZ[0]<<8)|arrayXYZ[1])>>2)/4096);
								double y2_=((double)((int16_t)((arrayXYZ[2]<<8)|arrayXYZ[3])>>2)/4096);
								double z2_=((double)((int16_t)((arrayXYZ[4]<<8)|arrayXYZ[5])>>2)/4096);
								
					sprintf(temp,"jedynka (poprawnie bo z znakiem): %1.4f  %1.4f  %1.4f", x2_, y2_, z2_); // default 4096 counts/g sensitivity
								g_comm.println(temp);
            // Przetwarzanie X/Y/Z
            double x_ = (static_cast<int16_t>(((arrayXYZ[0] << 8) | arrayXYZ[1]) >> 2)) / 4096.0;
            double y_ = (static_cast<int16_t>(((arrayXYZ[2] << 8) | arrayXYZ[3]) >> 2)) / 4096.0;
            double z_ = (static_cast<int16_t>(((arrayXYZ[4] << 8) | arrayXYZ[5]) >> 2)) / 4096.0;
					

            // (Zakladam standardowa czulosc ~4096 LSB/g)

            // Wyswietlenie przez UART
            std::sprintf(temp, "dwojka: %1.4f  %1.4f  %1.4f", x_, y_, z_);
            g_comm.println(temp);

            // Oblicz wektor przyspieszenia 3D


						
            DELAY(300);
        }

        // uart.println("koniec");
        

    }
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
    while(true)
    {
        char* msg = g_comm.receiveData(); // czeka na input
        g_comm.handleCommand(msg);
				//__WFI();
    }

    return 0;
}
