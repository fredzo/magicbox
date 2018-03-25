#ifndef MAGICBOX_H
#define MAGICBOX_H

#define PRESET_LIST_SIZE 128

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
 * Preset structure
 */
typedef struct Preset_t {
	LoopState loopState;
	ProgramChangeConfig pcConfig[5];
	ControlChangeConfig ccConfig[5];
}Preset;

/**
 * PresetList
 */
typedef Preset PresetList[PRESET_LIST_SIZE];

/**
 * Global config structure
 */
typedef struct GlobalConfig_t {
	LoopState loopState;
	ProgramChangeConfig pcConfig[5];
	ControlChangeConfig ccConfig[5];
    unsigned char padding[64-sizeof(LoopState)];
}GlobalConfig;





#endif // MAGICBOX_H