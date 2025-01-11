/*
 * Copyright (c) 2024 Miroslaw Baca
 * AGH - Object-Oriented Programming Language
 */

/**
 * @file CommunicationModuleBase.hpp
 * @brief Abstract base class for communication modules on the KL05Z platform.
 */

#ifndef COMMUNICATION_MODULE_BASE_HPP
#define COMMUNICATION_MODULE_BASE_HPP

/**
 * @class CommunicationModule
 * @brief Abstract interface for a communication module.
 */
class CommunicationModule
{
protected:
    // System commands
    inline static constexpr const char* PING              	= "ping";
    inline static constexpr const char* RESET             	= "reset";
    inline static constexpr const char* GET_SYSTEM_INFO   	= "readinfo";

    // Temperature command
    inline static constexpr const char* READ_TEMPERATURE  	= "readtemp";

    // TSI commands
    inline static constexpr const char* READ_TOUCH        	= "readtouch";
    inline static constexpr const char* SET_TOUCH         	= "settouch";

    // Led control commands
    inline static constexpr const char* SET_LED_COLOR_RED 	= "setledcolorred";
    inline static constexpr const char* SET_LED_COLOR_GREEN = "setledcolorgreen";
    inline static constexpr const char* SET_LED_COLOR_BLUE  = "setledcolorblue";

    // Accelerometer command
    inline static constexpr const char* READ_ACCELERATION   = "readaccel";

public:
    /**
     * @brief Pure virtual method to initialize communication.
     */
    virtual void init() = 0;

    /**
     * @brief Pure virtual method to send text without a newline.
     * @param text Pointer to the string buffer.
     */
    virtual void print(const char* text) = 0;

    /**
     * @brief Pure virtual method to send text with a newline appended.
     * @param text Pointer to the string buffer.
     */
    virtual void println(const char* text) = 0;

    /**
     * @brief Pure virtual method to receive data in a blocking manner.
     * @return Pointer to the received data buffer.
     */
    virtual char* receiveData() = 0;

    /**
     * @brief Pure virtual method to parse and handle commands.
     * @param cmd Pointer to the command string.
     */
    virtual void handleCommand(const char* cmd) = 0;

    /**
     * @brief Virtual destructor.
     */
    virtual ~CommunicationModule() = default;
};

#endif // COMMUNICATION_MODULE_BASE_HPP