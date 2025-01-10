/*
 * Copyright (c) 2024 Miroslaw Baca
 * AGH - Object-Oriented Programming Language
 */

/**
 * @file Uart.cpp
 * @brief Implementation of Uart class for the KL05Z MCU.
 */

#include "../inc/Uart.hpp"
#include "../inc/CommunicationModuleMCU.hpp"

mb::CommunicationModuleMCU* mb::Uart::g_commObject = nullptr;

extern "C" void UART0_IRQHandler(void)
{
	mb::Uart::handleIRQ();
}

namespace mb {

Uart::Uart() : Uart(9600, nullptr)
{
}

Uart::Uart(uint32_t baud, CommunicationModuleMCU* obj) : baudRate(baud)
{
    g_commObject = obj;
    SystemCoreClockUpdate();

    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    SIM->SOPT2 |= (1u << 26); // Select MCGFLLCLK as UART0 source

    PORTB->PCR[1] = PORT_PCR_MUX(2);
    PORTB->PCR[2] = PORT_PCR_MUX(2);

    UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);

    uint32_t divisor = 48000000u / (16u * baudRate);
    UART0->BDH = (uint8_t)((divisor >> 8) & 0x1F);
    UART0->BDL = (uint8_t)(divisor & 0xFF);
    UART0->C4  = 0x0F;

    UART0->C2 |= UART0_C2_RIE_MASK;
    UART0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);

    NVIC_EnableIRQ(UART0_IRQn);
}

Uart::Uart(const Uart& other) : Uart(other.baudRate, other.g_commObject)
{
}

Uart::~Uart()
{
    NVIC_DisableIRQ(UART0_IRQn);
}

void Uart::print(const char* text)
{
    while (*text)
    {
        sendChar(*text++);
    }
}

void Uart::println(const char* text)
{
    print(text);
    print("\n\r");
}

void Uart::sendChar(char c)
{
    while (!(UART0->S1 & UART0_S1_TDRE_MASK))
    {
        // Wait until the transmitter is ready
    }
    UART0->D = static_cast<uint8_t>(c);
}

void Uart::handleIRQ()
{
    while (UART0->S1 & UART0_S1_RDRF_MASK)
    {
        char c = static_cast<char>(UART0->D);
        if (g_commObject)
        {
            g_commObject->onCharReceived(c);
        }
    }
}

} // End of namespace mb