#include "SensorUart.hpp"

int main() {
	//SystemInit_Custom();
    UART uart(9600);
    uart.Println("Hello Worlddsadasd!");
	uart.Println("nibba!");

}
