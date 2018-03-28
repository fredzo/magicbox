#ifndef MAGICBOX_H
#define MAGICBOX_H

#define GLOBAL_CONFIG_SIZE  64
#define PRESET_LIST_SIZE    128


/**
 *  Loop state enum
 */
typedef enum LoopState_t
{
	LOOP_OFF	= 0,
	LOOP_A      = 1,
	LOOP_B      = 2,
	LOOP_BOTH	= 3
}LoopState;

/**
 *  MidiInRouting enum
 */
typedef enum MidiInRouting_t
{
	MIDI_IN_ROUTING_OFF         = 0,
	MIDI_IN_ROUTING_MIDI_OUT    = 1,
	MIDI_IN_ROUTING_USB_OUT     = 2,
	MIDI_IN_ROUTING_BOTH        = 3
}MidiInRouting;

/**
 *  UsbInRouting enum
 */
typedef enum UsbInRouting_t
{
	USB_IN_ROUTING_OFF         = 0,
	USB_IN_ROUTING_MIDI_OUT    = 1,
	USB_IN_ROUTING_USB_OUT     = 2,
	USB_IN_ROUTING_BOTH        = 3
}UsbInRouting;

/**
 *  Program change config structure
 */
typedef struct ProgramChangeConfig_t {
	unsigned char pcNumber;
}ProgramChangeConfig;

/**
 *  Control change config structure
 */
typedef struct ControlChangeConfig_t {
	unsigned char ccNumber;
	unsigned char ccValue;
}ControlChangeConfig;

/**
 *  Program change config structure
 */
typedef struct NoteConfig_t {
	unsigned char noteNumber;
}NoteConfig;



#define PC_CONFIG_NUMBER    5
#define CC_CONFIG_NUMBER    5
#define NOTE_CONFIG_NUMBER  5
#define PRESET_PADDING      32-(1 /*LoopState */+PC_CONFIG_NUMBER+2*CC_CONFIG_NUMBER+NOTE_CONFIG_NUMBER)

/**
 * Preset structure
 */
typedef struct Preset_t {
	LoopState loopState;
	ProgramChangeConfig pcConfig[PC_CONFIG_NUMBER];
	ControlChangeConfig ccConfig[CC_CONFIG_NUMBER];
	NoteConfig          noteConfig[NOTE_CONFIG_NUMBER];
    unsigned char       padding[PRESET_PADDING];
}Preset;

/**
 * PresetList
 */
typedef Preset PresetList[PRESET_LIST_SIZE];

#define GLOBAL_CONFOIG_PADDING  GLOBAL_CONFIG_SIZE-(3+PC_CONFIG_NUMBER+CC_CONFIG_NUMBER+NOTE_CONFIG_NUMBER)

/**
 * Global config structure
 */
typedef struct GlobalConfig_t {
	bool                configFlag;
	MidiInRouting       midiInRouting;
	UsbInRouting        usbInRouting;
	unsigned char       pcChanel[PC_CONFIG_NUMBER];
	unsigned char       ccChanel[CC_CONFIG_NUMBER];
	unsigned char       noteChanel[NOTE_CONFIG_NUMBER];
    unsigned char       padding[GLOBAL_CONFOIG_PADDING];
}GlobalConfig;


/**
 * Dump ProgramChangeConfig to uart
 * @param pcConfig
 */
void dumpPcConfig(const ProgramChangeConfig *pcConfig);

/**
 * Dump ControlChangeConfig to uart
 * @param ccConfig
 */
void dumpCcConfig(const ControlChangeConfig *ccConfig);

/**
 * Dum the provided Preset to uart
 * @param preset
 */
void dumpPreset(const Preset *preset);

/**
 * Dump the specified Preset to uart
 * @param presetIndex
 */
void dumpPresetFromList(unsigned char presetIndex);

/**
 * Erase flash for the whole configuration area (global config + preset list)
 */
void eraseAllConfig();

/**
 * Erse global config from flash memory
 */
void eraseGlboalConfig();

/**
 * Erase twe presets from flash memeory
 * @param firstPresetIndex the index of the first preset of the pair to erase
 */
void erasePresetPair(unsigned char firstPresetIndex);

/**
 * Read global config from flash
 * @param globalConfig
 */
void readGlobalConfig(GlobalConfig *globalConfig);

/**
 * Write global config to flash
 * @param globalConfig
 */
void writeGlobalConfig(const GlobalConfig *globalConfig);

/**
 * Write global config to flash, without erasing flash firt
 * @param globalConfig
 */
void writeGlobalConfigNoErase(const GlobalConfig *globalConfig);

/**
 * Read the specified preset
 * @param preset
 * @param presetIndex
 */
void readPreset(Preset *preset, unsigned char presetIndex);

/**
 * Write preset to flash
 * @param preset
 * @param presetIndex
 */
void writePreset(const Preset *preset, unsigned char presetIndex);

/**
 * Write preset to flash, without erasing flash firt
 * @param preset
 * @param presetIndex
 */
void writePresetNoErase(const Preset *preset, unsigned char presetIndex);


#endif // MAGICBOX_H