/*
 * Copyright (c) 2024 Miroslaw Baca
 * AGH - Object-Oriented Programming Language
 */

/**
 * @file AccelerometerClass.cpp
 * @brief Implementation of Accelerometer class for parsing and handling acceleration data.
 */

#include "AccelerometerClass.hpp"
#include <iostream>
#include <iomanip>

namespace mb {

    bool Accelerometer::parseRawData(const std::vector<uint8_t> &rawData) {
        if (rawData.size() < 6) {
            std::cerr << "[WARN] Not enough data for accel parse." << std::endl;
            return false;
        }

        // Extract raw data: 6 bytes, each axis is represented by 2 bytes (big-endian format)
        int16_t rawX = (static_cast<int16_t>(rawData[0]) << 8) | rawData[1];
        int16_t rawY = (static_cast<int16_t>(rawData[2]) << 8) | rawData[3];
        int16_t rawZ = (static_cast<int16_t>(rawData[4]) << 8) | rawData[5];

        // Scale raw values to 'g' units (range: ±2g, represented by ±32768 for a 16-bit ADC)
        constexpr float accelSensitivity = 2.0f / 32768.0f; // ±2g scale for signed 16-bit data
        x = rawX * accelSensitivity;
        y = rawY * accelSensitivity;
        z = rawZ * accelSensitivity;

        return true;
    }

    void Accelerometer::print() const {
        std::cout << "[PC] ACCEL: X=" << std::fixed << std::setprecision(4) << x
                  << "  Y=" << y
                  << "  Z=" << z << std::endl;
    }

// Overloaded operators
    Accelerometer Accelerometer::operator+(const Accelerometer &other) const {
        return Accelerometer(x + other.x, y + other.y, z + other.z);
    }

    Accelerometer &Accelerometer::operator+=(const Accelerometer &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Accelerometer Accelerometer::operator-(const Accelerometer &other) const {
        return Accelerometer(x - other.x, y - other.y, z - other.z);
    }

    Accelerometer &Accelerometer::operator-=(const Accelerometer &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    bool Accelerometer::operator==(const Accelerometer &other) const {
        return (x == other.x && y == other.y && z == other.z); // Compare all components
    }

    bool Accelerometer::operator!=(const Accelerometer &other) const {
        return !(*this == other); // Negate equality
    }

    bool Accelerometer::operator<(const Accelerometer &other) const {
        return this->magnitude() < other.magnitude(); // Compare magnitudes
    }

    bool Accelerometer::operator<=(const Accelerometer &other) const {
        //testy dokoncz
        constexpr float tolerance = 1e-5;

        if (this->magnitude() < other.magnitude()) {
            return true;
        } else if (std::fabs(this->magnitude() - other.magnitude()) <= tolerance) {
            return true;
        } else {
            return false;
        }

    }

    bool Accelerometer::operator>(const Accelerometer &other) const {
        return this->magnitude() > other.magnitude();
    }

    bool Accelerometer::operator>=(const Accelerometer& other) const {
            // testy dokoncz
        if (this->magnitude() > other.magnitude()) {
            return true;
        }

        constexpr float precision = 1e-5;

        float diff = this->magnitude() - other.magnitude();
        float roundedDiff = std::round(diff / precision) * precision;


    }



    std::ostream &operator<<(std::ostream &out, const Accelerometer &accel) {
        out << "Accelerometer(" << accel.x << ", " << accel.y << ", " << accel.z << ")";
        return out;
    }

} // End of namespace
