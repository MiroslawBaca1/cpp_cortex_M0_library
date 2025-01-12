/*
 * Copyright (c) 2024 Miroslaw Baca
 * AGH - Object-Oriented Programming Language
 */

/**
 * @file main.cpp
 * @brief Main file for presentation the PC-based communication interface with FRDM-KL05Z.
 */

#include "CommunicationModulePC.hpp"
#include "AccelerometerClass.hpp"
#include <iostream>

int main() {
    mb::Accelerometer TestObj1;
    mb::Accelerometer TestObj2(2.0,1.3,7.0);
    mb::Accelerometer TestObj3(TestObj2);

    TestObj1.print();
    TestObj2.print();
    TestObj3.print();

    std::cout<< TestObj1 + TestObj2 + TestObj3 << std::endl;
    std::cout<< TestObj1 - TestObj2 - TestObj3 << std::endl;

    if (TestObj2 == TestObj3) {
        std::cout<< "TestObj2 == TestObj3" << std::endl;
    }

    if (TestObj2 <= TestObj3) {
        std::cout<< "TestObj2 <= TestObj3" << std::endl;
    }

    if (TestObj2 >= TestObj3) {
        std::cout<< "TestObj2 >= TestObj3" << std::endl;
    }

    if (TestObj1 != TestObj3) {
        std::cout<< "TestObj1 != TestObj3" << std::endl;
    }

    if (TestObj1 < TestObj3) {
        std::cout<< "TestObj1 < TestObj3" << std::endl;
    }

    try {
        mb::CommunicationModulePC comm("COM6", 9600); // Initialize communication on COM6
        std::string command;
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, command);

            if (command == "exit") {
                break; // Exit the application
            }
            comm.handleCommand(command.c_str()); // Handle user command
        }
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl; // Log any runtime error
        return 1;
    }

    return 0;
}
