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
 *  Program change config structure
 */
typedef struct NoteConfig_t {
	unsigned char noteNumber;
}NoteConfig;



#define PC_CONFIG_NUMBER    5
#define CC_CONFIG_NUMBER    5
#define NOTE_CONFIG_NUMBER  5
#define PRESET_PADDING      32-(PC_CONFIG_NUMBER+2*CC_CONFIG_NUMBER+NOTE_CONFIG_NUMBER)

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

#define GLOBAL_CONFOIG_PADDING  64-(3+PC_CONFIG_NUMBER+CC_CONFIG_NUMBER+NOTE_CONFIG_NUMBER)

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





#endif // MAGICBOX_H