#include "SensorUart.hpp"

int main() {
	//SystemInit_Custom();
    Uart uart(9600);
    uart.println("Hello Worlddsadasd!");
		uart.println("Test2!");

}
