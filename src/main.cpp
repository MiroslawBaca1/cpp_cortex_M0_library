/*
 * Copyright (c) 2024 Miroslaw Baca
 * AGH - Object-Oriented Programming Language
 */

/**
 * @file main.cpp
 * @brief Main entry point of the FRDM-KL05Z application.
 */

#include <cstdio>
#include <cstdint>

extern "C" {
#include "MKL05Z4.h"
}

#include "../inc/BoardSupport.hpp"
#include "../inc/Uart.hpp"
#include "../inc/CommunicationModuleMCU.hpp"

int main()
{
    // The stack size has been increased in the linker script to 0x0400.
    // The clock setup is also configured to 1 for this project.

		mb::CommunicationModuleMCU comm_obj;
		mb::Uart start(9600, &comm_obj);

    I2C::init();
    LED_init();
    TSI_Init();
    ADC_Init();

    comm_obj.init();
    setLedColor(true, false, false);
    comm_obj.println("Waiting for commands...");

    while (true)
    {
        // Wait for incoming data and handle the received command.
        char* msg = comm_obj.receiveData();
        comm_obj.handleCommand(msg);
    }

    return 0;
}
