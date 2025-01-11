/*
 * Copyright (c) 2024 Miroslaw Baca
 * AGH - Object-Oriented Programming Language
 */

/**
 * @file CommunicationModulePC.hpp
 * @brief Class for handling communication with FRDM-KL05Z from a PC.
 */

#ifndef COMMUNICATION_MODULE_PC_HPP
#define COMMUNICATION_MODULE_PC_HPP

#include "CommunicationModuleBase.hpp"
#include <string>
#include <vector>
#include "SerialPort.hpp"
#include <cstdint>

namespace mb {

/**
 * @class CommunicationModulePC
 * @brief Implements the communication interface for a PC connected to the FRDM-KL05Z via UART.
 */
    class CommunicationModulePC : public CommunicationModule {
    private:
        SerialPort serial; /**< SerialPort object for low-level UART communication. */

    public:
        /**
         * @brief Default constructor for communication with default port and baud rate.
         */
        CommunicationModulePC();

        /**
         * @brief Parameterized constructor for communication with custom port and baud rate.
         * @param port COM port to connect to (in my example "COM6").
         * @param baudRate Baud rate for UART communication (In my example 9600).
         */
        CommunicationModulePC(const std::string &port, unsigned int baudRate = 9600);

        /**
         * @brief Destructor for releasing resources and closing the UART port.
         */
        ~CommunicationModulePC() override;

        /**
         * @brief Clears the input buffer of the UART connection.
         */
        void clearBuffer();

        /**
         * @brief Initializes the communication module.
         * @throws std::runtime_error if UART connection fails.
         */
        void init() override;

        /**
         * @brief Sends a plain text message over UART.
         * @param text Null-terminated string to send.
         */
        void print(const char *text) override;

        /**
         * @brief Sends a message over UART with a newline appended.
         * @param text Null-terminated string to send.
         */
        void println(const char *text) override;

        /**
         * @brief Receives data from UART as a null-terminated string.
         * @return Pointer to the received data buffer.
         */
        char *receiveData() override;

        /**
         * @brief Handles commands sent to the microcontroller.
         * @param cmd Command to process.
         */
        void handleCommand(const char *cmd) override;

        /**
         * @brief Processes raw acceleration data received from the microcontroller.
         * @param rawLine Null-terminated string containing raw data.
         */
        void processRawAcceleration(const char *rawLine);
    };

}

#endif // COMMUNICATION_MODULE_PC_HPP
