# target-ButtonRead
This project uses STM32CubeIDE and it's a program created to practice my C habilities during the course 'Microcontroller Embedded C Programming: Absolute Beginners' from FastBit Embedded Brain Academy.

## Let's code

We are going to program the pushbutton B1 of the NUCLEO-F401RE. The following image shows its connection.

![image](https://user-images.githubusercontent.com/58916022/205989443-d46fad1e-e2e0-4bac-b755-2dfb125c08ec.png)

SB17 jumper is connected by a 0R 0805 resistor and provides connection between B1 to PC13.

The settings of LD2 is explained in [target-LedOn code](https://github.com/Rafaelatff/target-LedOn), so here, we are going to focus only in the input configuration.

First, we need to start the clock also for GPIOC. For that, we need to know the initial address registers for all the registers we need to access.

![image](https://user-images.githubusercontent.com/58916022/205990802-0169b225-2523-48a2-b1de-4545a4143608.png)

To configure the RCC (clock for GPIOC), we need to set the AHB1ENR bit 2 (GPIOCEN). The address value of RCC AHB1 peripheral clock enable register is 0x40023830 + 0x30 (offset). 

![image](https://user-images.githubusercontent.com/58916022/205992500-c7a9115e-9501-4683-b1f1-0e145e7b4b35.png)

To set GPIOCEN, we add the following line to code: *pClkCtrlReg |= (1 << 2);

Then, to configure PC13 as input, we must configure 26th and 27th bit as 00. To do so, we sum the initial register address of GPIOC (0x4002 0800) to the offset (0x0) presented on the reference manual RM0368. For that, first we have to declare and initialize the correspondent pointer: uint32_t *pPortCModeReg = (uint32_t*)0x40020800;. And finally, we clear the bits of MODER13 by *pPortCModeReg &= ~(3 <<26);.

![image](https://user-images.githubusercontent.com/58916022/205993758-9ccde24f-f2b6-4bca-af77-96c6216c5a78.png)

And finally, we need to read the IDR13 bit of the GPIO C port input data register (GPIOC_IDR). To find the register address of GPIOC_IDR, we sum the GPIOC address (0x4002 0800) to the offset (0x10) for IDR register and we initialize a pointer with its value:  uint32_t *pPortCInReg = (uint32_t*) 0x40020810;.

![image](https://user-images.githubusercontent.com/58916022/205994478-53b14587-0f1a-4f3f-82d2-235481e4ca02.png)

This register is a reading-only (letter 'r' in bit fields). 

Then, to read the input of PC13, the instructor presented the following method:

```c
uint16_t pinC13status = (uint16_t)(*pPortCInReg &  0x2000); //masks to clear all non-important bits and keep result in 13th bit.  

If(pinC13status){ 
	//Set the 5th bit (ODR5) of GPIOA_ODR to make pin 5 HIGH 
	*pPortAOutReg |= (1 << 5); 
}else{ 
	//Set the 5th bit (ODR5) of GPIOA_ODR to make pin 5 LOW 
	*pPortAOutReg &= ~(1 << 5); 
} 
```
The complete code is showed next:

![image](https://user-images.githubusercontent.com/58916022/206012590-a81e9e9f-37bc-463d-867d-918491d0125b.png)

## Debugging

Let's add the Variable's view:

![image](https://user-images.githubusercontent.com/58916022/205995557-34e1e377-4759-4a81-8741-e6293acb684e.png)

B1 is connected to a pull-up resistor (external resistor - R30), meaning that when the button is released, the 13th bit of *pPortCinReg must be 1. With that configuration (PC13 high), pinC13 status must be 0x2000. To confirm that, we debug the code:

![image](https://user-images.githubusercontent.com/58916022/205996243-0c6024cc-9e99-45c4-b9c1-5f4359723354.png)

Now, with B1 pressed, we keep running the code (step by step), and confirmed that with PC13 receiving low level signal, pinC13status changed to 0x0.

![image](https://user-images.githubusercontent.com/58916022/205996553-f2c663ac-f628-4ac8-8872-a1487e1161ec.png)

Code works perfectly!

## Results

NUCLEO-F401RE working with code 'target-ButtonRead'.

![B1andLD2](https://user-images.githubusercontent.com/58916022/205996652-bc2f736b-0bfc-4b29-9e28-4e18b0385d92.gif)


