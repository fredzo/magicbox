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


#define EUSART1_TX_BUFFER_SIZE 64
#define EUSART1_RX_BUFFER_SIZE 64

/**
  Section: Global Variables
*/
volatile unsigned char eusart1TxHead = 0;
volatile unsigned char eusart1TxTail = 0;
volatile unsigned char eusart1TxBuffer[EUSART1_TX_BUFFER_SIZE];
volatile unsigned char eusart1TxBufferRemaining;

volatile unsigned char eusart1RxHead = 0;
volatile unsigned char eusart1RxTail = 0;
volatile unsigned char eusart1RxBuffer[EUSART1_RX_BUFFER_SIZE];
volatile unsigned char eusart1RxCount;


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
    // disable interrupts before changing states
    PIE1bits.RC1IE = 0;
    PIE1bits.TX1IE = 0;

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
    
    // initializing the driver state
    eusart1TxHead = 0;
    eusart1TxTail = 0;
    eusart1TxBufferRemaining = sizeof(eusart1TxBuffer);

    eusart1RxHead = 0;
    eusart1RxTail = 0;
    eusart1RxCount = 0;

    // enable receive interrupt
    PIE1bits.RC1IE = 1;
    

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
void USART_Write(char c)
{
    while(0 == eusart1TxBufferRemaining)
    {
    }

    if(0 == PIE1bits.TX1IE)
    {
        TXREG1 = c;
    }
    else
    {
        PIE1bits.TX1IE = 0;
        eusart1TxBuffer[eusart1TxHead++] = c;
        if(sizeof(eusart1TxBuffer) <= eusart1TxHead)
        {
            eusart1TxHead = 0;
        }
        eusart1TxBufferRemaining--;
    }
    PIE1bits.TX1IE = 1;
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
unsigned char USART_Read ()
{
    unsigned char readValue  = 0;
    
    while(0 == eusart1RxCount)
    {
    }

    readValue = eusart1RxBuffer[eusart1RxTail++];
    if(sizeof(eusart1RxBuffer) <= eusart1RxTail)
    {
        eusart1RxTail = 0;
    }
    PIE1bits.RC1IE = 0;
    eusart1RxCount--;
    PIE1bits.RC1IE = 1;

    return readValue;
}

/**
 * Returns the number of bytes available in the input buffer
 */
unsigned char USART_available()
{
    return eusart1RxCount;
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
        USART_Write(c);
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
        USART_Write( res/100 + '0' );
        printed_already = 1;
    }
    res = res - (res/100)*100;

    if ((res/10) || (printed_already == 1))
    {
        USART_Write( res/10 + '0' );
    }
    res = res - (res/10)*10;

    USART_Write( res + '0' );
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
    USART_Write( CharacterArray[toPrint] );

    toPrint = printVar & 0x0F;
    USART_Write( CharacterArray[toPrint] );

    return;
}

void USART_Transmit_ISR(void)
{

    // add your EUSART1 interrupt custom code
    if(sizeof(eusart1TxBuffer) > eusart1TxBufferRemaining)
    {
        TXREG1 = eusart1TxBuffer[eusart1TxTail++];
        if(sizeof(eusart1TxBuffer) <= eusart1TxTail)
        {
            eusart1TxTail = 0;
        }
        eusart1TxBufferRemaining++;
    }
    else
    {
        PIE1bits.TX1IE = 0;
    }
}

void USART_Receive_ISR(void)
{
    
    if(1 == RCSTA1bits.OERR)
    {
        // EUSART1 error - restart

        RCSTA1bits.CREN = 0;
        RCSTA1bits.CREN = 1;
    }

    // buffer overruns are ignored
    eusart1RxBuffer[eusart1RxHead++] = RCREG1;
    if(sizeof(eusart1RxBuffer) <= eusart1RxHead)
    {
        eusart1RxHead = 0;
    }
    eusart1RxCount++;
}
