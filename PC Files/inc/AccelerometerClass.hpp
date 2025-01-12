/*
 * Copyright (c) 2024 Miroslaw Baca
 * AGH - Object-Oriented Programming Language
 */

/**
 * @file AccelerometerClass.hpp
 * @brief Class for parsing and representing accelerometer data.
 */

#ifndef ACCELEROMETER_CLASS_HPP
#define ACCELEROMETER_CLASS_HPP

#include <vector>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <iomanip>

namespace mb {

/**
 * @class Accelerometer
 * @brief Represents accelerometer data and provides methods for parsing and computations.
 */
    class Accelerometer {
    private:
        float x; /**< X-axis acceleration in g. */
        float y; /**< Y-axis acceleration in g. */
        float z; /**< Z-axis acceleration in g. */

    public:
        /**
         * @brief Default constructor initializing acceleration to zero.
         */
        Accelerometer() : x(0.0f), y(0.0f), z(0.0f) {}

        /**
         * @brief Parameterized constructor initializing acceleration to given values.
         * @param x X-axis acceleration.
         * @param y Y-axis acceleration.
         * @param z Z-axis acceleration.
         */
        Accelerometer(float x, float y, float z) : x(x), y(y), z(z) {}

        /**
         * @brief Copy constructor initializing acceleration by copying of the another Accelerometer objet.
         * @param other The Accelerometer object to copy from.
         */
        Accelerometer(const Accelerometer& other) : x(other.x), y(other.y), z(other.z) {}

        /**
         * @brief Sets the X-asis acceleration.
         * @param x_val New value of X-axis acceleration.
         */
        void setX(float x_val) {
            x=x_val;
        }

        /**
         * @brief Sets the Y-asis acceleration.
         * @param y_val New value of Y-axis acceleration.
         */
        void setY(float y_val) {
            y=y_val;
        }

        /**
         * @brief Sets the Z-asis acceleration.
         * @param z_val New value of Z-axis acceleration.
         */
        void setZ(float z_val) {
            z=z_val;
        }

        /**
         * @brief Returns the X-axis acceleration.
         * @return X-axis acceleration in g.
         */
        float getX() const { return x; }

        /**
         * @brief Returns the Y-axis acceleration.
         * @return Y-axis acceleration in g.
         */
        float getY() const { return y; }

        /**
         * @brief Returns the Z-axis acceleration.
         * @return Z-axis acceleration in g.
         */
        float getZ() const { return z; }

        /**
         * @brief Parses raw accelerometer data and scales it to g units.
         * @param rawData Vector containing 6 bytes of raw data.
         * @return True if parsing was successful, otherwise false.
         */
        bool parseRawData(const std::vector<uint8_t> &rawData);

        /**
         * @brief Prints the accelerometer values in a formatted manner.
         */
        void print() const;

        /**
         * @brief Computes the magnitude of the acceleration vector.
         * @return Magnitude (sqrt(x^2 + y^2 + z^2)).
         */
        float magnitude() const {
            return std::sqrt(x * x + y * y + z * z);
        }

        // Operator overloads
        Accelerometer operator+(const Accelerometer &other) const;
        Accelerometer &operator+=(const Accelerometer &other);
        Accelerometer operator-(const Accelerometer &other) const;
        Accelerometer &operator-=(const Accelerometer &other);
        bool operator==(const Accelerometer &other) const;
        bool operator!=(const Accelerometer &other) const;
        bool operator<(const Accelerometer &other) const;
        bool operator<=(const Accelerometer &other) const;
        bool operator>(const Accelerometer &other) const;
        bool operator>=(const Accelerometer &other) const;

        /**
         * @brief Outputs the accelerometer data in a human-readable format.
         * @param out Output stream.
         * @param accel Accelerometer object.
         * @return Reference to the output stream.
         */
        friend std::ostream &operator<<(std::ostream &out, const Accelerometer &accel);
    };

} // End of namespace

#endif // ACCELEROMETER_CLASS_HPP
