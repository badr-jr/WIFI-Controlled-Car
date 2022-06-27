/*
 * UART_program.c
 *
 *  Created on: Jun 16, 2022
 *      Author: badrm
 */


#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "UART_interface.h"
#include "UART_private.h"
#include "UART_config.h"

void MUSART_voidInit(void)
{
	/* Baud Rate = (Fclk/(16*BRR))  */
	MUSART1 -> BRR = MUSART_BRR;		/* Baud Rate = 9600 */
	/* Enable USART */
	SET_BIT(MUSART1 ->CR1 ,13);
	/* Enable Receiver */
	SET_BIT(MUSART1 ->CR1 ,2);
	/* Enable Transmitter */
	SET_BIT(MUSART1 ->CR1 ,3);
	/* Clear Status Register */
	MUSART1 -> SR = 0;
}

void MUSART_voidTransimit(u8 arr[])
{	u8 i=0;
	while(arr[i] != '\0')
	{
		MUSART1 -> DR = arr[i];
		/* wait until transmission flag is fired */
		while((GET_BIT((MUSART1 -> SR),6)) == 0);
		i++;

	}
}

u8 MUSART_u8Receive(void)
{
	/* this variable is used to set limit for waiting time */
	u16 timeout = 0;
	/* wait until receive flag is fired */
	while(GET_BIT(MUSART1 -> SR,5) == 0)
	{
		if(timeout == 50000)
		{
			return 255;
		}
		timeout++;
	}

	return (0xFF & (MUSART1 -> DR));
}
