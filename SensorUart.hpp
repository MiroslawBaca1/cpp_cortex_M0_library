/*
 * Copyright (c) 2024 Miroslaw Baca
 * AGH - Object-oriented programming languages
 */

#pragma once

#include <cstdint>

/** Minimal UART driver for KL05Z in C++. */

/** Base address of UART0. */
static constexpr std::uintptr_t UART0_BASE  = 0x4006A000u;
/** Base address of SIM (System Integration Module). */
static constexpr std::uintptr_t SIM_BASE    = 0x40047000u;
/** Base address of PORTB. */
static constexpr std::uintptr_t PORTB_BASE  = 0x4004A000u;
/** Base address of MCG (Multipurpose Clock Generator). */
static constexpr std::uintptr_t MCG_BASE    = 0x40064000u;

/** System Integration Module register map (only necessary fields). */
struct SIM_Type {
    volatile uint32_t SOPT1;
    volatile uint32_t SOPT1CFG;
    uint8_t  RESERVED_0[4092];
    volatile uint32_t SOPT2;
    uint8_t  RESERVED_1[4];
    volatile uint32_t SOPT4;
    volatile uint32_t SOPT5;
    uint8_t  RESERVED_2[4];
    volatile uint32_t SOPT7;
    uint8_t  RESERVED_3[8];
    volatile uint32_t SDID;
    uint8_t  RESERVED_4[12];
    volatile uint32_t SCGC4;
    volatile uint32_t SCGC5;
    volatile uint32_t SCGC6;
    volatile uint32_t SCGC7;
    volatile uint32_t CLKDIV1;
    uint8_t  RESERVED_5[4];
    volatile uint32_t FCFG1;
    volatile uint32_t FCFG2;
    uint8_t  RESERVED_6[4];
    volatile uint32_t UIDMH;
    volatile uint32_t UIDML;
    volatile uint32_t UIDL;
    uint8_t  RESERVED_7[156];
    volatile uint32_t COPC;
    volatile uint32_t SRVCOP;
};

/** PORT register map for controlling pin configuration. */
struct PORT_Type {
    volatile uint32_t PCR[32];
};

/** UART0 register map with 8-bit registers. */
struct UART0_Type {
    volatile uint8_t BDH;
    volatile uint8_t BDL;
    volatile uint8_t C1;
    volatile uint8_t C2;
    volatile uint8_t S1;
    volatile uint8_t S2;
    volatile uint8_t C3;
    volatile uint8_t D;
    volatile uint8_t MA1;
    volatile uint8_t MA2;
    volatile uint8_t C4;
    volatile uint8_t C5;
};

/** MCG register map (subset). */
struct MCG_Type {
    volatile uint8_t C1;
    volatile uint8_t C2;
    volatile uint8_t C3;
    volatile uint8_t C4;
    volatile uint8_t C5;
    volatile uint8_t C6;
    volatile uint8_t S;
    uint8_t  RESERVED_0;
    volatile uint8_t SC;
    uint8_t  RESERVED_1;
    volatile uint8_t ATCVH;
    volatile uint8_t ATCVL;
};

/** Returns pointer to SIM registers. */
inline SIM_Type* getSIM() {
    return reinterpret_cast<SIM_Type*>(SIM_BASE);
}

/** Returns pointer to PORTB registers. */
inline PORT_Type* getPORTB() {
    return reinterpret_cast<PORT_Type*>(PORTB_BASE);
}

/** Returns pointer to UART0 registers. */
inline UART0_Type* getUART0() {
    return reinterpret_cast<UART0_Type*>(UART0_BASE);
}

/** Returns pointer to MCG registers. */
inline MCG_Type* getMCG() {
    return reinterpret_cast<MCG_Type*>(MCG_BASE);
}

/** Clock gating bit for UART0 (SCGC4, bit 10). */
static constexpr uint32_t SIM_SCGC4_UART0_MASK = (1u << 10);
/** Clock gating bit for PORTB (SCGC5, bit 10). */
static constexpr uint32_t SIM_SCGC5_PORTB_MASK = (1u << 10);

/** Helper to set UART0 clock source in SOPT2 (bits [27:26]). */
inline constexpr uint32_t SIM_SOPT2_UART0SRC(uint8_t x) {
    return (static_cast<uint32_t>(x) << 26);
}

/** Core clock division (OUTDIV1 in CLKDIV1, bits [31:28]). */
inline constexpr uint32_t SIM_CLKDIV1_OUTDIV1(uint8_t x) {
    return (static_cast<uint32_t>(x) << 28);
}

/** Bus clock division (OUTDIV4 in CLKDIV1, bits [19:16]). */
inline constexpr uint32_t SIM_CLKDIV1_OUTDIV4(uint8_t x) {
    return (static_cast<uint32_t>(x) << 16);
}

/** Helper to set MUX bits [10:8] in PCR. */
inline constexpr uint32_t PORT_PCR_MUX(uint8_t x) {
    return (static_cast<uint32_t>(x) << 8);
}

/** Enable TX (bit 3) in C2. */
static constexpr uint8_t UART_C2_TE   = (1 << 3);
/** Enable RX (bit 2) in C2. */
static constexpr uint8_t UART_C2_RE   = (1 << 2);
/** Indicates TDRE (bit 7) in S1. */
static constexpr uint8_t UART_S1_TDRE = (1 << 7);

/**
 * Initializes the clock to ~24 MHz by setting DMX32=1 and DRST_DRS=01,
 * then dividing the core clock by 2 (and bus by 2).
 */
inline void systemInitCustom() {
    getMCG()->C4 = (1 << 7) | (1 << 5);
    getSIM()->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(1) | SIM_CLKDIV1_OUTDIV4(1);
}

/** Simple UART class for KL05Z. */
class Uart {
public:
    /**
     * Creates a UART object and configures UART0 with the specified baud rate.
     * Assumes the system clock is set to 48 MHz.
     */
    explicit Uart(uint32_t baudRate = 9600u) {
        systemInitCustom();
        getSIM()->SCGC4 |= SIM_SCGC4_UART0_MASK;
        getSIM()->SCGC5 |= SIM_SCGC5_PORTB_MASK;
        getSIM()->SOPT2 |= SIM_SOPT2_UART0SRC(1);

        getPORTB()->PCR[1] = PORT_PCR_MUX(2);
        getPORTB()->PCR[2] = PORT_PCR_MUX(2);

        getUART0()->C2 &= static_cast<uint8_t>(~(UART_C2_TE | UART_C2_RE));

        const uint32_t divisor = 48000000u / (16u * baudRate);
        getUART0()->BDH = static_cast<uint8_t>((divisor >> 8) & 0x1F);
        getUART0()->BDL = static_cast<uint8_t>(divisor & 0xFF);
        getUART0()->C4  = 0x0F;

        getUART0()->C2 |= (UART_C2_TE | UART_C2_RE);
    }

    /** Sends a null-terminated string (blocking). */
    void print(const char* text) const {
        while (*text) {
            sendChar(*text++);
        }
    }

    /** Sends a string followed by newline+carriage return. */
    void println(const char* text) const {
        print(text);
        print("\n\r");
    }

private:
    /** Sends one character, waiting for TDRE=1. */
    void sendChar(char c) const {
        while (!(getUART0()->S1 & UART_S1_TDRE)) {
            // wait
        }
        getUART0()->D = static_cast<uint8_t>(c);
    }
};
