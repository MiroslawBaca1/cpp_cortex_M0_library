/*
 * Copyright (c) 2024 Miroslaw Baca
 * AGH - Object-Oriented Programming Language
 */

/**
 * @file CommunicationModulePC.cpp
 * @brief Implementation of CommunicationModulePC for PC-based UART communication.
 */

#include "CommunicationModulePC.hpp"
#include "AccelerometerClass.hpp"
#include <iostream>
#include <cstring>
#include <sstream>
#include <cstdint>
#include <chrono>

namespace mb {

    CommunicationModulePC::CommunicationModulePC()
            : CommunicationModulePC("COM6", 9600) {}

    CommunicationModulePC::CommunicationModulePC(const std::string &port, unsigned int baudRate)
            : serial(port.c_str(), baudRate) {
        init();
    }

    CommunicationModulePC::~CommunicationModulePC() {
        serial.closeSerial(); // Ensure UART is closed properly
        std::cout << "[INFO] UART closed on port." << std::endl;
    }

    void CommunicationModulePC::init() {
        if (!serial.isConnected()) {
            throw std::runtime_error("[ERROR] Failed to connect to UART."); // Handle connection error
        }
        std::cout << "[INFO] UART initialized on port." << std::endl;
    }

    void CommunicationModulePC::clearBuffer() {
        unsigned char tempBuffer[256];
        while (serial.readSerialPort(reinterpret_cast<char *>(tempBuffer), sizeof(tempBuffer)) > 0) {
            // Read data but discard it to clear the buffer
        }
    }

    void CommunicationModulePC::print(const char *text) {
        serial.writeSerialPort(text, std::strlen(text)); // Send plain text
    }

    void CommunicationModulePC::println(const char *text) {
        std::ostringstream oss;
        oss << text << "\r\n";
        std::string str = oss.str();
        serial.writeSerialPort(str.c_str(), str.size()); // Send text with newline
    }

    char *CommunicationModulePC::receiveData() {
        static char recvBuffer[128];
        size_t bytesRead = serial.readSerialPort(recvBuffer, sizeof(recvBuffer) - 1);
        recvBuffer[bytesRead] = '\0'; // Null-terminate the buffer

        // Remove any trailing newline or carriage return characters
        for (size_t i = 0; i < bytesRead; ++i) {
            if (recvBuffer[i] == '\r' || recvBuffer[i] == '\n') {
                recvBuffer[i] = '\0';
                break;
            }
        }

        return recvBuffer;
    }

    void CommunicationModulePC::handleCommand(const char *cmd) {
        clearBuffer(); // Clear the buffer before sending a command

        println(cmd); // Send the command to the microcontroller

        char recvBuffer[128];
        size_t totalBytesRead = 0;
        int linesReceived = 0;
        const int maxLines = (std::strcmp(cmd, READ_ACCELERATION) == 0) ? 1 : 2; // Determine the expected number of lines
        const int timeoutMs = 250; // Timeout for response in milliseconds
        auto startTime = std::chrono::steady_clock::now();

        while (linesReceived < maxLines) {
            size_t bytesRead = serial.readSerialPort(recvBuffer + totalBytesRead,
                                                     sizeof(recvBuffer) - 1 - totalBytesRead);

            if (bytesRead > 0) {
                totalBytesRead += bytesRead;
                recvBuffer[totalBytesRead] = '\0'; // Null-terminate the buffer

                // Process each complete line received
                char *lineStart = recvBuffer;
                char *lineEnd = nullptr;

                while ((lineEnd = std::strchr(lineStart, '\n')) != nullptr) {
                    *lineEnd = '\0'; // Terminate the current line
                    if (std::strcmp(cmd, READ_ACCELERATION) == 0) {
                        std::cout << "[UART RESPONSE] " << lineStart << std::endl;
                        processRawAcceleration(lineStart);
                    } else {
                        std::cout << "[UART RESPONSE] " << lineStart << std::endl;
                    }

                    linesReceived++;
                    if (linesReceived >= maxLines) {
                        return;
                    }

                    lineStart = lineEnd + 1; // Move to the next line
                }

                // Preserve leftover data for the next read
                size_t remaining = totalBytesRead - (lineStart - recvBuffer);
                std::memmove(recvBuffer, lineStart, remaining);
                totalBytesRead = remaining;

                // Reset the timeout if data was received
                startTime = std::chrono::steady_clock::now();
            }

            // Check if the timeout has been exceeded
            if (std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - startTime)
                        .count() > timeoutMs) {
                if (linesReceived > 0) {
                    return; // If at least one line was received, return
                } else {
                    std::cerr << "[WARN] Timeout while waiting for response." << std::endl;
                    return;
                }
            }
        }
    }

    void CommunicationModulePC::processRawAcceleration(const char *rawLine) {
        uint8_t parsedData[6];
        int index = 0;

        // Parse the raw acceleration data (6 bytes expected)
        char *token = std::strtok(const_cast<char *>(rawLine), " ");
        while (token != nullptr && index < 6) {
            parsedData[index++] = static_cast<uint8_t>(std::atoi(token));
            token = std::strtok(nullptr, " ");
        }

        if (index == 6) {
            Accelerometer accel;
            if (accel.parseRawData(std::vector<uint8_t>(parsedData, parsedData + 6))) {
                accel.print();
            } else {
                std::cerr << "[ERROR] Failed to process acceleration data." << std::endl;
            }
        } else {
            std::cerr << "[WARN] Not enough data for accel parse." << std::endl;
        }
    }

} // End of namespace
