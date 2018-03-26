/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/

/** INCLUDES *******************************************************/
#include "system.h"

#include "app_device_audio_midi.h"
#include "app_led_usb_status.h"

#include "usb_device.h"
#include "usb_device_midi.h"

#include "magicbox.h"
#include "usart.h"

#include <flash.h>

#define FLASH_GLOBAL_CONFIG_ADDRESS 0x06000 // A more-or-less arbitrary address that is a multiple of 1024       
#define FLASH_PRESETS_ADDRESS FLASH_GLOBAL_CONFIG_ADDRESS+64 // A more-or-less arbitrary address that is a multiple of 1024       
const GlobalConfig globalConfig@FLASH_GLOBAL_CONFIG_ADDRESS;
const PresetList presetList@FLASH_PRESETS_ADDRESS;

static Preset currentPreset;

void dumpPcConfig(ProgramChangeConfig pcConfig)
{
    USART_printString("[pcNum=");
    USART_putDec(pcConfig.pcNumber);
    USART_printString("]");
}

void dumpCcConfig(ControlChangeConfig ccConfig)
{
    USART_printString("[ccNum=");
    USART_putDec(ccConfig.ccNumber);
    USART_printString(",ccVal=");
    USART_putDec(ccConfig.ccValue);
    USART_printString("]");
}

void dumpPreset(Preset preset)
{
    USART_printString("Preset[loop=");
    switch(preset.loopState) 
    {
        case LOOP_OFF :
            USART_printString("Off");
            break;
        case LOOP_A :
            USART_printString("A");
            break;
        case LOOP_B :
            USART_printString("B");
            break;
        case LOOP_BOTH :
            USART_printString("Both");
            break;
        default :
            USART_printString("Invalid");
            break;
    }
    USART_printString(",pcConfig1");
    dumpPcConfig(preset.pcConfig[0]);
    USART_printString(",pcConfig2");
    dumpPcConfig(preset.pcConfig[1]);
    USART_printString(",pcConfig3");
    dumpPcConfig(preset.pcConfig[2]);
    USART_printString(",pcConfig4");
    dumpPcConfig(preset.pcConfig[3]);
    USART_printString("]\r\n");
}

/********************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *******************************************************************/
MAIN_RETURN main(void)
{
    // Initialize UART
    USART_Initialize();

    SYSTEM_Initialize(SYSTEM_STATE_USB_START);

    USBDeviceInit();
    USBDeviceAttach();
    
    USART_printString("\r\n");
    USART_printString("App init\r\n");

    USART_printString("Config flag const = ");
    USART_putDec(globalConfig.configFlag);
    USART_printString("\r\n");
   
    // Write flash test
    unsigned char CheckState;
 
    ReadFlash(FLASH_GLOBAL_CONFIG_ADDRESS, 1, &CheckState);
    
    USART_printString("Config flag read = ");
    USART_putDec(CheckState);
    USART_printString("\r\n");
    
 
    if ( CheckState != 0x01 )
    {   // Not yet configured
        
        // Earese flash
        EraseFlash(FLASH_GLOBAL_CONFIG_ADDRESS, FLASH_GLOBAL_CONFIG_ADDRESS+2+sizeof(presetList));

        //Change Configuration Flag
        CheckState = 0x01;
        WriteBytesFlash(FLASH_GLOBAL_CONFIG_ADDRESS, 1, &CheckState);

        USART_printString("Config after set flag const = ");
        USART_putDec(globalConfig.configFlag);
        USART_printString("\r\n");

        // Write flash test
        unsigned char CheckStateResult;

        ReadFlash(FLASH_GLOBAL_CONFIG_ADDRESS, 1, &CheckStateResult);

        USART_printString("Config after set flag read = ");
        USART_putDec(CheckStateResult);
        USART_printString("\r\n");
        
        for(int i=0 ; i < 4 ; i++)
        {
            currentPreset.loopState = i%4;
            currentPreset.pcConfig[0].pcNumber = i%16; 
            currentPreset.pcConfig[1].pcNumber = i+1%16; 
            currentPreset.pcConfig[2].pcNumber = i+2%16; 
            currentPreset.pcConfig[3].pcNumber = i+3%16; 
            currentPreset.pcConfig[4].pcNumber = i+4%16; 
            WriteBytesFlash((unsigned long)(&presetList[i]),sizeof(currentPreset),(unsigned char *)&currentPreset);
        }
    }
    USART_printString("Presets from rom = \r\n");
    for(int i=0 ; i < 4 ; i++)
    {
        dumpPreset(presetList[i]);
    }
    USART_printString("Presets from read flash= \r\n");
    for(int i=0 ; i < 4 ; i++)
    {
        ReadFlash((unsigned long)(&presetList[i]),sizeof(Preset),(unsigned char *)&currentPreset);
        dumpPreset(currentPreset);
    }
    
    
    // End write flash test

    while(1)
    {
        SYSTEM_Tasks();

        #if defined(USB_POLLING)
            // Interrupt or polling method.  If using polling, must call
            // this function periodically.  This function will take care
            // of processing and responding to SETUP transactions
            // (such as during the enumeration process when you first
            // plug in).  USB hosts require that USB devices should accept
            // and process SETUP packets in a timely fashion.  Therefore,
            // when using polling, this function should be called
            // regularly (such as once every 1.8ms or faster** [see
            // inline code comments in usb_device.c for explanation when
            // "or faster" applies])  In most cases, the USBDeviceTasks()
            // function does not take very long to execute (ex: <100
            // instruction cycles) before it returns.
            USBDeviceTasks();
        #endif

        //Application specific tasks
        APP_DeviceAudioMIDITasks();

    }//end while
}//end main


/*******************************************************************************
 End of File
*/
