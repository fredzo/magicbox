/********************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *******************************************************************/

#ifndef USART_H
#define USART_H

#include <stdbool.h>




/*********************************************************************
* Function: void USART_Initialize(void);
*
* Overview: Initializes USART (RS-232 port)
*
* PreCondition: None
*
* Input: None
*
* Output: None
*
********************************************************************/
void USART_Initialize();

/******************************************************************************
 * Function:        void USART_putcUSART(char c)
 *
 * PreCondition:    None
 *
 * Input:           char c - character to print to the UART
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Print the input character to the UART
 *
 * Note:
 *
 *****************************************************************************/
void USART_Write(char);

/******************************************************************************
 * Function:        unsigned char USART_getcUSART()
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          unsigned char c - character to received on the UART
 *
 * Side Effects:    None
 *
 * Overview:        Get the input character from the UART
 *
 * Note:
 *
 *****************************************************************************/
unsigned char USART_Read(void);

/**
 * Returns the number of bytes available in the input buffer
 */
unsigned char USART_available();

/*******************************************************************************
Function: USART_printString( char *str )

Precondition:
    UART2Init must be called prior to calling this routine.

Overview:
    This function prints a string of characters to the UART.

Input: Pointer to a null terminated character string.

Output: None.

*******************************************************************************/
void USART_printString(unsigned const char *str );

/*******************************************************************************
Function: USART_putDec(unsigned char dec)

Precondition:
    USART_Initialize must be called prior to calling this routine.

Input: Binary data

Output: none

Side Effects: none

Overview: This function converts decimal data into a string 
          and outputs it to UART.

Note: none
*******************************************************************************/
void USART_putDec( unsigned char dec );

/*******************************************************************************
Function: USART_putHex

Precondition:
    USART_Initialize must be called prior to calling this routine.

Input: Binary data

Output: none

Side Effects: none

Overview: This function converts hex data into a string 
          and outputs it to UART.

Note: none
*******************************************************************************/
void USART_putHex( int toPrint );


/**
  @Summary
    Maintains the driver's transmitter state machine and implements its ISR.

  @Description
    This routine is used to maintain the driver's internal transmitter state
    machine.This interrupt service routine is called when the state of the
    transmitter needs to be maintained in a non polled manner.

  @Preconditions
    EUSART1_Initialize() function should have been called
    for the ISR to execute correctly.

  @Param
    None

  @Returns
    None
*/
void USART_Transmit_ISR(void);

/**
  @Summary
    Maintains the driver's receiver state machine and implements its ISR

  @Description
    This routine is used to maintain the driver's internal receiver state
    machine.This interrupt service routine is called when the state of the
    receiver needs to be maintained in a non polled manner.

  @Preconditions
    EUSART1_Initialize() function should have been called
    for the ISR to execute correctly.

  @Param
    None

  @Returns
    None
*/
void USART_Receive_ISR(void);

#endif //USART_H
