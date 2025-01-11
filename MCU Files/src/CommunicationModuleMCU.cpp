/*
 * Copyright (c) 2024 Miroslaw Baca
 * AGH - Object-Oriented Programming Language
 */

/**
 * @file CommunicationModuleMCU.cpp
 * @brief Implementation of CommunicationModuleMCU for the KL05Z MCU.
 */

#include "../inc/CommunicationModuleMCU.hpp"
#include "../inc/Uart.hpp"
#include <cstdio>
#include "../inc/BoardSupport.hpp"

namespace mb { // Start of namespace mb

CommunicationModuleMCU::CommunicationModuleMCU()
{
}



void CommunicationModuleMCU::init()
{
    std::memset(buffer, 0, sizeof(buffer));
    dataReady = false;
}

void CommunicationModuleMCU::print(const char* text)
{
    Uart::print(text);
}

void CommunicationModuleMCU::println(const char* text)
{
    Uart::println(text);
}

char* CommunicationModuleMCU::receiveData()
{
    while (!dataReady)
    {
        // Wait until data is ready
    }
    dataReady = false;
    return buffer;
}

void CommunicationModuleMCU::handleCommand(const char* cmd)
{
    if (std::strcmp(cmd, PING) == 0)
    {
        println("PONG");
    }
    else if (std::strcmp(cmd, RESET) == 0)
    {
        println("System resetting...");
        NVIC_SystemReset();
    }
    else if (std::strcmp(cmd, GET_SYSTEM_INFO) == 0)
    {
        char uidBuffer[50];
        uint32_t uidMid = SIM->UIDML;
        uint32_t uidLow = SIM->UIDL;
        std::snprintf(uidBuffer, sizeof(uidBuffer), "Device: FRDM-KL05ZJ\nUID: %08X-%08X", uidMid, uidLow);
        println(uidBuffer);
    }
    else if (std::strcmp(cmd, READ_TEMPERATURE) == 0)
    {
        float temperature = readTemperature();
        char temperatureBuffer[10];
        int intPart = static_cast<int>(temperature);
        int fracPart = static_cast<int>((temperature - intPart) * 100);
        std::snprintf(temperatureBuffer, sizeof(temperatureBuffer), "%d.%02dC", intPart, fracPart);
        println(temperatureBuffer);
    }
    else if (std::strcmp(cmd, READ_TOUCH) == 0)
    {
        uint8_t sliderVal = TSI_ReadSlider();
        char txt[15];
        std::sprintf(txt, "Slider = %u\r", sliderVal);
        println(txt);
    }
    else if (std::strcmp(cmd, SET_TOUCH) == 0)
    {
        println("TSI calibration...");
        self_calibration();
    }
    else if (std::strcmp(cmd, SET_LED_COLOR_RED) == 0)
    {
        setLedColor(true, false, false);
        println("LED set to RED!");
    }
    else if (std::strcmp(cmd, SET_LED_COLOR_GREEN) == 0)
    {
        setLedColor(false, true, false);
        println("LED set to GREEN!");
    }
    else if (std::strcmp(cmd, SET_LED_COLOR_BLUE) == 0)
    {
        setLedColor(false, false, true);
        println("LED set to BLUE!");
    }
    else if (std::strcmp(cmd, READ_ACCELERATION) == 0)
    {
        I2C::writeReg(0x1D, 0x2A, 1);
        I2C::writeReg(0x1D, 0x0E, 0x00);

        static char tempBuffer[36];
        static uint8_t arrayXYZ[6];

        I2C::readRegBlock(0x1D, 0x01, 6, arrayXYZ);
        std::snprintf(tempBuffer,
                      sizeof(tempBuffer),
                      "%d %d %d %d %d %d",
                      arrayXYZ[0],
                      arrayXYZ[1],
                      arrayXYZ[2],
                      arrayXYZ[3],
                      arrayXYZ[4],
                      arrayXYZ[5]);
        println(tempBuffer);

        DELAY(300);
    }
    else
    {
        println("Unknown command");
    }
}

void CommunicationModuleMCU::onCharReceived(char c)
{
    static size_t idx = 0;

    if (c == '\n')
    {
        buffer[idx] = '\0';
        idx = 0;
        dataReady = true;
    }
    else if (c == '\r')
    {
        // Ignore carriage return
    }
    else
    {
        if (idx < (BUFFER_SIZE - 1))
        {
            buffer[idx++] = c;
        }
    }
}

} // End of namespace mb