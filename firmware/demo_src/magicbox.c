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
#include "magicbox.h"
#include "usart.h"

#include <flash.h>

/**
 * Allocate space un program memory to store GlobalConfig and PresetList
 */
#define FLASH_GLOBAL_CONFIG_ADDRESS 0x06000 // A more-or-less arbitrary address that is a multiple of 1024       
#define FLASH_PRESETS_ADDRESS       FLASH_GLOBAL_CONFIG_ADDRESS+GLOBAL_CONFIG_SIZE 
const GlobalConfig GLOBAL_CONFIG@FLASH_GLOBAL_CONFIG_ADDRESS;
const PresetList PRESET_LIST@FLASH_PRESETS_ADDRESS;


void dumpPcConfig(const ProgramChangeConfig *pcConfig)
{
    USART_printString("[pcNum=");
    USART_putDec(pcConfig->pcNumber);
    USART_printString("]");
}

void dumpCcConfig(const ControlChangeConfig *ccConfig)
{
    USART_printString("[ccNum=");
    USART_putDec(ccConfig->ccNumber);
    USART_printString(",ccVal=");
    USART_putDec(ccConfig->ccValue);
    USART_printString("]");
}

void dumpPresetFromList(unsigned char presetIndex)
{
    dumpPreset(&PRESET_LIST[presetIndex]);
}

void dumpPreset(const Preset *preset)
{
    USART_printString("Preset[loop=");
    switch(preset->loopState) 
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
    dumpPcConfig(&preset->pcConfig[0]);
    USART_printString(",pcConfig2");
    dumpPcConfig(&preset->pcConfig[1]);
    USART_printString(",pcConfig3");
    dumpPcConfig(&preset->pcConfig[2]);
    USART_printString(",pcConfig4");
    dumpPcConfig(&preset->pcConfig[3]);
    USART_printString("]\r\n");
}

/**
 * Erase flash for the whole configuration area (global config + preset list)
 */
void eraseAllConfig()
{
    EraseFlash((unsigned long)(&GLOBAL_CONFIG), (unsigned long)(&GLOBAL_CONFIG)+sizeof(GlobalConfig)+sizeof(PresetList));
}

/**
 * Erse global config from flash memory
 */
void eraseGlboalConfig()
{
    EraseFlash((unsigned long)(&GLOBAL_CONFIG), (unsigned long)(&GLOBAL_CONFIG)+sizeof(GlobalConfig));
}

/**
 * Erase twe presets from flash memeory
 * @param firstPresetIndex the index of the first preset of the pair to erase
 */
void erasePresetPair(unsigned char firstPresetIndex)
{   // A flash page is 64 bytes large, a Preset is only 32 bytes large, so we need to erase presets by pair
    EraseFlash((unsigned long)(&PRESET_LIST[firstPresetIndex]),(unsigned long)(&PRESET_LIST[firstPresetIndex])+2*sizeof(Preset));
}

/**
 * Read global config from flash
 * @param globalConfig
 */
void readGlobalConfig(GlobalConfig* globalConfig)
{
    ReadFlash((unsigned long)(&GLOBAL_CONFIG), sizeof(GlobalConfig),(unsigned char *) globalConfig);
}

/**
 * Write global config to flash
 * @param globalConfig
 */
void writeGlobalConfig(const GlobalConfig* globalConfig)
{   // We need to erase flash first (write 0xFFs) since writing to flash can only change bits from 1 to 0
    eraseGlboalConfig();
    // Actualy write data to flash
    writeGlobalConfigNoErase(globalConfig);
}

/**
 * Write global config to flash, without erasing flash firt
 * @param globalConfig
 */
void writeGlobalConfigNoErase(const GlobalConfig* globalConfig)
{   // Actualy write data to flash
    WriteBytesFlash((unsigned long)(&GLOBAL_CONFIG), sizeof(GlobalConfig), (unsigned char *)globalConfig);
}

/**
 * Read the specified preset
 * @param preset
 * @param presetIndex
 */
void readPreset(Preset* preset, unsigned char presetIndex)
{
    ReadFlash((unsigned long)(&PRESET_LIST[presetIndex]), sizeof(Preset), (unsigned char *)preset);
}

/**
 * Write preset to flash
 * @param preset
 * @param presetIndex
 */
void writePreset(const Preset* preset, unsigned char presetIndex)
{   // We need to erase flash first (write 0xFFs) since writing to flash can only change bits from 1 to 0
    // Presets need to be erased by pairs (due to flash page size)
    unsigned char presetToBackupIndex;
    unsigned char presetToWriteIndex;
    unsigned char firstPresetIndex;
    if((presetIndex % 2) == 0)
    {   // Even preset number => backup second preset in pair
        presetToBackupIndex = presetIndex+1;
        presetToWriteIndex = presetIndex;
        firstPresetIndex = presetIndex;
    } 
    else
    {   // Odd preset number => backup first preset in pair
        presetToBackupIndex = presetIndex-1;
        presetToWriteIndex = presetIndex;
        firstPresetIndex = presetIndex-1;
    }
    Preset backup;
    readPreset(&backup,presetToBackupIndex);
    erasePresetPair(firstPresetIndex);
    // Actualy write data to flash
    writePresetNoErase(&backup,presetToBackupIndex);
    writePresetNoErase(preset,presetToWriteIndex);
}

/**
 * Write preset to flash, without erasing flash firt
 * @param preset
 * @param presetIndex
 */
void writePresetNoErase(const Preset* preset, unsigned char presetIndex)
{   
    WriteBytesFlash((unsigned long)(&PRESET_LIST[presetIndex]), sizeof(Preset), (unsigned char *)preset);
}

/*******************************************************************************
 End of File
*/
