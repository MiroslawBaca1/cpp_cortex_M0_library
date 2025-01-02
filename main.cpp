/*
 * Copyright (c) 2024 Miroslaw Baca
 * AGH - Object-Oriented Programming Language
 */
 
#include "CommunicationModuleMCU.hpp"     // Klasa Uart
#include "I2C.hpp"      // Klasa I2C
#include <cstdio>       // sprintf
#include <cmath>        // sqrt
#include <cstdint>

// delay makro
#ifndef DELAY
  #define DELAY(x)  for(uint32_t i = 0; i < (x * 10000U); i++) { __asm("nop"); }
#endif

static char temp[2256]; // Zwiekszenie bufora

static uint8_t readout = 0;
static uint8_t arrayXYZ[6];

int main() 
{
    // Inicjalizacja UART (z biblioteka Uart.hpp)
    // Domyslne tez 9600 w konstruktorze.
    CommunicationModuleMCU uart(9600);
		uart.init();
    
    // Inicjalizacja I2C (z biblioteka I2C.hpp)
    I2C::init();

    int stepsWalk = 0;
    int stepsRun  = 0;

    double accelerationVectorOld = 1.0;
		
	
		uart.println("\n-> MMA8451 first measurement.");
	
		I2C::writeReg(0x1D, 0x2A, 1);//========================== :) =======================	 		// Set ACTIVE bit in CTRL_REG1 in MMA8451
		I2C::writeReg(0x1D, 0x0E, 0x00); // aktywuje pomiar dla 2g
	
	
		I2C::readReg(0x1D, 0x05, &readout); //========================== :) =======================// Get OUT_Z_MSB from MMA8451 (MSB 8 bits from 14 in total)
		
		sprintf(temp,"Gravity: %+1.2fg", ((double)((int16_t)(readout<<8)>>2)/4096)); // default 4096 counts/g sensitivity
		uart.println(temp);
	
	
    // Wiadomosc startowa
    uart.println("Starting measurement in 2s...");

		for(int xjdd = 2; xjdd >= 1; xjdd--) 
    {
        // Petla "pomiarowa"
        for(int xjd = 20; xjd >= 1; xjd--) 
        {
            // Odczyt 16 bajtów rejestrów z sensora (np. MMA8451, MMA8652, itp.)
            // Adres 0x1D, rejestr startowy 0x01, 16 bajtów -> arrayXYZ
            I2C::readRegBlock(0x1D, 0x01, 6, arrayXYZ);




					






					
					
								double x2_=((double)((int16_t)((arrayXYZ[0]<<8)|arrayXYZ[1])>>2)/4096);
								double y2_=((double)((int16_t)((arrayXYZ[2]<<8)|arrayXYZ[3])>>2)/4096);
								double z2_=((double)((int16_t)((arrayXYZ[4]<<8)|arrayXYZ[5])>>2)/4096);
								
					sprintf(temp,"jedynka (poprawnie bo z znakiem): %1.4f  %1.4f  %1.4f", x2_, y2_, z2_); // default 4096 counts/g sensitivity
								uart.println(temp);
            // Przetwarzanie X/Y/Z
            double x_ = (static_cast<int16_t>(((arrayXYZ[0] << 8) | arrayXYZ[1]) >> 2)) / 4096.0;
            double y_ = (static_cast<int16_t>(((arrayXYZ[2] << 8) | arrayXYZ[3]) >> 2)) / 4096.0;
            double z_ = (static_cast<int16_t>(((arrayXYZ[4] << 8) | arrayXYZ[5]) >> 2)) / 4096.0;
					

            // (Zakladam standardowa czulosc ~4096 LSB/g)

            // Wyswietlenie przez UART
            std::sprintf(temp, "dwojka: %1.4f  %1.4f  %1.4f", x_, y_, z_);
            uart.println(temp);

            // Oblicz wektor przyspieszenia 3D


						
            DELAY(300);
        }

        // uart.println("Loop done, go to sleep...");
        

    }
    
    // Normalnie tu nie dojdzie, ale formalnie:
    return 0;
}
