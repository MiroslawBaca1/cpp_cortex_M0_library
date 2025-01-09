#ifndef COMMUNICATION_MODULE_BASE_HPP
#define COMMUNICATION_MODULE_BASE_HPP

/**
 * @file CommunicationModuleBase.hpp
 * @brief Interfejs bazowy dla komunikacji. 
 *        Klasa bazowa, po której beda dziedziczyc konkretne implementacje
 *        (np. na mikrokontrolerze, na komputerze, itp.).
 */

class CommunicationModule {
protected:
    // Komendy ogólne
		inline static constexpr const char* PING = "ping";
    inline static constexpr const char* RESET = "reset";
    inline static constexpr const char* GET_SYSTEM_INFO = "readinfo";

    // Komendy dla czujnika temperatury
    inline static constexpr const char* READ_TEMPERATURE = "readtemp";

    // Komendy dla czujnika dotykowego
    inline static constexpr const char* READ_TOUCH = "readtouch";
    inline static constexpr const char* SET_TOUCH = "settouch";

    // Komendy dla LED RGB
    inline static constexpr const char* SET_LED_COLOR_RED = "setledcolorred";
    inline static constexpr const char* SET_LED_COLOR_GREEN = "setledcolorgreen";
    inline static constexpr const char* SET_LED_COLOR_BLUE = "setledcolorblue";

    // Komendy dla akcelerometru
    inline static constexpr const char* READ_ACCELERATION = "readaccel";
    inline static constexpr const char* SET_ACCELERATION_MODE_2 = "setaccelmode2";
    inline static constexpr const char* SET_ACCELERATION_MODE_4 = "setaccelmode4";
    inline static constexpr const char* SET_ACCELERATION_MODE_8 = "setaccelmode8";

public:
    virtual void init() = 0;
    virtual void print(const char* text) = 0;
    virtual void println(const char* text) = 0;
    virtual char* receiveData() = 0;
    virtual void handleCommand(const char* cmd) = 0;

    virtual ~CommunicationModule() = default;
};

#endif // COMMUNICATION_MODULE_BASE_HPP
