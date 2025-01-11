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
    try {
        mb::CommunicationModulePC comm("COM6", 9600); // Initialize communication on COM6

        mb::Accelerometer ObjShow;

        std::cout << ObjShow << std::endl;

        ObjShow.print();

        std::cout << "d";


        mb::Accelerometer ObjShow2(2.0,3.0,4.0);

        ObjShow += ObjShow2;

        ObjShow.print();

        if (ObjShow2 >= ObjShow) {
            std::cout << "2 is bigger" << std::endl;
        }

        std::cout << ObjShow2.magnitude() << std::endl;
        std::cout << ObjShow.magnitude() << std::endl;

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
