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
#include "interrupt_manager.h"

#include <flash.h>

/** Static reference to current Preset */
static Preset currentPreset;
/** Static reference to globalConfig */
static GlobalConfig globalConfig;

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

    INTERRUPT_Initialize();
    
    // Read Global Config from flash
    readGlobalConfig(&globalConfig);
    
    USART_printString("\r\n");
    USART_printString("App init\r\n");

    USART_printString("Config flag = ");
    USART_putDec(globalConfig.configFlag);
    USART_printString("\r\n");
   
    // Write flash test
 
    if (globalConfig.configFlag != true)
    {   // Not yet configured
        
        // Earase flash
        eraseAllConfig();

        //Change Configuration Flag
        globalConfig.configFlag = true;
        writeGlobalConfigNoErase(&globalConfig);

        readGlobalConfig(&globalConfig);
        USART_printString("Config after set flag = ");
        USART_putDec(globalConfig.configFlag);
        USART_printString("\r\n");

        
        for(unsigned char i=0 ; i < 4 ; i++)
        {
            currentPreset.loopState = i%4;
            currentPreset.pcConfig[0].pcNumber = i%16; 
            currentPreset.pcConfig[1].pcNumber = i+1%16; 
            currentPreset.pcConfig[2].pcNumber = i+2%16; 
            currentPreset.pcConfig[3].pcNumber = i+3%16; 
            currentPreset.pcConfig[4].pcNumber = i+4%16; 
            writePresetNoErase(&currentPreset,i);
        }
    }
    else
    {
            currentPreset.loopState = LOOP_BOTH;
            currentPreset.pcConfig[0].pcNumber = 0x66; 
            currentPreset.pcConfig[1].pcNumber = 0x55; 
            currentPreset.pcConfig[2].pcNumber = 0x44; 
            currentPreset.pcConfig[3].pcNumber = 0x33; 
            currentPreset.pcConfig[4].pcNumber = 0x22;
            currentPreset.noteConfig[4].noteNumber = 0x69;
            writePreset(&currentPreset,2);
    }
    USART_printString("Presets from rom = \r\n");
    for(unsigned char i=0 ; i < 4 ; i++)
    {
        dumpPresetFromList(i);
    }
    USART_printString("Presets from read flash= \r\n");
    for(unsigned char i=0 ; i < 4 ; i++)
    {
        readPreset(&currentPreset,i);
        dumpPreset(&currentPreset);
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
        
        while(USART_available()>0)
        {   // Echo
            USART_Write(USART_Read());
        }

    }//end while
}//end main


/*******************************************************************************
 End of File
*/
