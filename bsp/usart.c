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

#include <usart.h>
#include <stdbool.h>
#include <xc.h>

/******************************************************************************
 * Function:        void USART_Initialize(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine initializes the UART to the value set by Host Terminal Application
 *
 * Note:
 *
 *****************************************************************************/

void USART_Initialize()
{
       unsigned char c;
     
       ANSELCbits.ANSC7 = 0;    // Make RC6 and RC7 pin digital
       ANSELCbits.ANSC6 = 0;
     
        TRISCbits.TRISC6=0;				// TX as output
        TRISCbits.TRISC7=1;				// RX as input

        TXSTA1bits.SYNC = 0;       	// Async
        TXSTA1bits.TX9 = 0;       	// 8bits
        TXSTA1bits.TXEN = 1;       	// TX enabled

        RCSTA1bits.RX9 = 0;       	// 8bits
        RCSTA1bits.CREN = 1;       	// Enable receiver
        RCSTA1bits.SPEN = 1;       	// Serial port enabled

        // Baudrate = 19 200 => BGGH =1, BRG16 = 1 / SPBRGx = 624 = 0x270
        // Baudrate = 31 250 => BGGH =1, BRG16 = 1 / SPBRGx = 383 = 0x17F
        BAUDCON1bits.BRG16 = 1;       	// 8bits
        TXSTA1bits.BRGH = 1;       	// 8bits
        SPBRG1 = 0x70;
        SPBRGH1 = 0x02;      	
        //SPBRG1 = 0x7F;
        //SPBRGH1 = 0x01;      	

        c = RCREG;				// read
    
    

}//end USART_Initialize

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
void USART_putcUSART(char c)
{
    while (TXSTA1bits.TRMT == 0) {}
    TXREG1 = c;
}

/******************************************************************************
 * Function:        void USART_getcUSART(char c)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          unsigned char c - character to received on the UART
 *
 * Side Effects:    None
 *
 * Overview:        Print the input character to the UART
 *
 * Note:
 *
 *****************************************************************************/
unsigned char USART_getcUSART ()
{
	char  c;

	if (RCSTAbits.OERR)  // in case of overrun error
	{                    // we should never see an overrun error, but if we do,
		RCSTAbits.CREN = 0;  // reset the port
		c = RCREG;
		RCSTAbits.CREN = 1;  // and keep going.
	}
	else
    {
		c = RCREG;
    }
// not necessary.  EUSART auto clears the flag when RCREG is cleared
//	PIR1bits.RCIF = 0;    // clear Flag

  
	return c;
}

/*******************************************************************************
Function: USART_printString( char *str )

Precondition:
    USART_Initialize must be called prior to calling this routine.

Overview:
    This function prints a string of characters to the UART.

Input: Pointer to a null terminated character string.

Output: None.

*******************************************************************************/
void USART_printString(const unsigned char *str )
{
    unsigned char c;

    while( (c = *str++) )
        USART_putcUSART(c);
}

/*******************************************************************************
Function: USART_putDec(unsigned char dec)

Precondition:
    USART_Initialize must be called prior to calling this routine.

Overview:
    This function converts decimal data into a string and outputs it to UART.

Input: Binary data.

Output: None.

*******************************************************************************/
void  USART_putDec(unsigned char dec)
{
    unsigned char res;
    unsigned char printed_already = 0;

    res = dec;

    if (res/100)
    {
        USART_putcUSART( res/100 + '0' );
        printed_already = 1;
    }
    res = res - (res/100)*100;

    if ((res/10) || (printed_already == 1))
    {
        USART_putcUSART( res/10 + '0' );
    }
    res = res - (res/10)*10;

    USART_putcUSART( res + '0' );
}

/*******************************************************************************
Function: USART_putHex

Precondition:
    USART_Initialize must be called prior to calling this routine.

Overview:
    This function converts hex data into a string and outputs it to UART.

Input: Binary data.

Output: None.

*******************************************************************************/

const unsigned char CharacterArray[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void USART_putHex( int toPrint )
{
    int printVar;

    printVar = toPrint;
    toPrint = (toPrint>>4) & 0x0F;
    USART_putcUSART( CharacterArray[toPrint] );

    toPrint = printVar & 0x0F;
    USART_putcUSART( CharacterArray[toPrint] );

    return;
}

