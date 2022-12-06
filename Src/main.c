#include <stdint.h>

int main(void)
{
	uint32_t *pClkCtrReg = (uint32_t*)0x40023830; //RCC_AHB1ENR pointer to register
	uint32_t *pPortAModeReg = (uint32_t*)0x40020000; //GPIOA_MODER pointer to GPIO A port mode
	uint32_t *pPortCModeReg = (uint32_t*)0x40020800; //GPIOA_MODER pointer to GPIO A port mode
	uint32_t *pPortAOutReg = (uint32_t*)0x40020014; //GPIOA_ODR pointer to GPIO A port output register
	uint32_t *pPortCInReg = (uint32_t*)0x40020810; //GPIOC_IDR pointer to GPIO C port input

	// Modifing code to use bitwise shift operators

	//1. Enable the clock for the GPIO A and GPIOC
	*pClkCtrReg |= (1 << 0); //Set bit 0 - GPIOAEN of pClkCtrReg register
	*pClkCtrReg |= (1 << 2); //Set bit 2 - GPIOCEN of pClkCtrReg register

	//2. Set GPIO PA5 as general purpose output mode
	*pPortAModeReg &= ~(3 << 10); //Clearing bits of MODE5 (10 and 11)
	*pPortAModeReg |= (1 << 10); //Set 10th bit of MODE register

	//3. Set GPIO PC13 as general purpose input mode
	*pPortCModeReg &= ~(3 << 26); //Clearing bits of MODE13 (26 and 27)

	while(1){ //4. Keep reading B1 and updating LD2

		uint16_t pinC13status = (uint16_t)(*pPortCInReg &  0x2000); //masks to clear all
		// non-important bits and keep result in 13th bit.

		if(pinC13status){ // Any value different than 0x00 is 1
			//Set the 5th bit (ODR5) of GPIOA_ODR to make pin 5 HIGH
			*pPortAOutReg |= (1 << 5);
		}else{
			//Set the 5th bit (ODR5) of GPIOA_ODR to make pin 5 LOW
			*pPortAOutReg &= ~(1 << 5);
		}
	}

}
