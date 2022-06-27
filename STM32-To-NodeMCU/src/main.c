/*
 * main.c
 *
 *  Created on: Jun 24, 2022
 *      Author: badrm
 */

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "RCC_interface.h"
#include "UART_interface.h"
#include "GPIO_interface.h"

/* Array Holds Data Fetched From USART */
u8 Response[10];
/* Function To Initialize Response Array */
void InitializeArray(){
	u8 i;
	for(i=0 ; i<10 ;i++)
	{
		Response[i]=0;
	}
}
void HNODEMCU_voidptrGetData()
{
	InitializeArray(Response);
		s8 i = 0;
		/* Get Response of ESP on our commands */
		while(1){
			Response[i]=MUSART_u8Receive();
			if(Response[i] == 255 || Response[i] == 0 || Response[i] == '\r')
				continue;
			/* If # Fetched Then That's The End Of Data Fetched */
			if(Response[i] == '#')
				break;
			i++;
		}
}
/* This function handles movement directions */
void DisableRemaining(u8 Copy_u8Exception)
{
	if(PIN1 != Copy_u8Exception)
	{
		MGPIO_voidSetPinValue(GPIOA, PIN1, GPIO_LOW);
	}
	if(PIN2 != Copy_u8Exception)
	{
		MGPIO_voidSetPinValue(GPIOA, PIN2, GPIO_LOW);
	}
	if(PIN3 != Copy_u8Exception)
	{
		MGPIO_voidSetPinValue(GPIOA, PIN3, GPIO_LOW);
	}
	if(PIN4 != Copy_u8Exception)
	{
		MGPIO_voidSetPinValue(GPIOA, PIN4, GPIO_LOW);
	}
}
void main()
{
	/* Initialize RCC To HSE */
	RCC_voidInitSysClock();
	RCC_voidEnableClock(RCC_APB2, 14);		/* Enable USART1 Clock */
	RCC_voidEnableClock(RCC_APB2, 2);		/* Enable GPIO Clock */
	/* Setting A9:TX pin as Output alternate function push pull w max speed 50 MHz */
	MGPIO_voidSetPinDirection(GPIOA,PIN9,OUTPUT_FREQ_50MHZ_AFPP);
	/* Setting A10:RX pin as input floating */
	MGPIO_voidSetPinDirection(GPIOA,PIN10,INPUT_FLOATING);
	/* Controlling front DC motor */
	MGPIO_voidSetPinDirection(GPIOA, PIN1, OUTPUT_FREQ_10MHZ_PP);
	MGPIO_voidSetPinDirection(GPIOA, PIN2, OUTPUT_FREQ_10MHZ_PP);
	/* Controlling back DC motor */
	MGPIO_voidSetPinDirection(GPIOA, PIN3, OUTPUT_FREQ_10MHZ_PP);
	MGPIO_voidSetPinDirection(GPIOA, PIN4, OUTPUT_FREQ_10MHZ_PP);
	/* Initialize USART with 115200 Baud Rate */
	MUSART_voidInit();
	while(1)
	{
		/* wait for data to get fetched from NodeMCU */
		HNODEMCU_voidptrGetData();
		/* f => forward */
		if(Response[0] == 'f')
		{
			MGPIO_voidSetPinValue(GPIOA, PIN3, GPIO_HIGH);
			DisableRemaining(PIN3);
		}
		/* b => back */
		else if(Response[0] == 'b')
		{
			MGPIO_voidSetPinValue(GPIOA, PIN4, GPIO_HIGH);
			DisableRemaining(PIN4);
		}
		/* r => right */
		else if(Response[0] == 'r')
		{
			MGPIO_voidSetPinValue(GPIOA, PIN1, GPIO_HIGH);
			DisableRemaining(PIN1);
		}
		/* l=> left */
		else if(Response[0] == 'l')
		{
			MGPIO_voidSetPinValue(GPIOA, PIN2, GPIO_HIGH);
			DisableRemaining(PIN2);
		}
		/* Stop */
		else
		{
			DisableRemaining(PIN5);
		}
	}
}

