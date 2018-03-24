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
	LOOP_BOTH	= 4
}LoopState;

/**
 *  Program change config structure
 */
typedef struct ProgramChangeConfig_t {
	unsigned char channel;
	unsigned char pcNumber;
}ProgramChangeConfig;


/**
 * Preset structure
 */
typedef struct Preset_t {
	LoopState loopState;
	ProgramChangeConfig pcConfig1;
	ProgramChangeConfig pcConfig2;
	ProgramChangeConfig pcConfig3;
	ProgramChangeConfig pcConfig4;
}Preset;

/**
 * PresetList
 */
typedef Preset PresetList[PRESET_LIST_SIZE];





#endif // MAGICBOX_H