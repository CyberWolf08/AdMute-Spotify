#include "admute.h"


#define VK_VOLUME_MUTE 0xAD // Virtual-key code for the mute key


int MasterVolumeMuteUnmute() {
	// Structure used to create the mute key	
	INPUT key;

	key.type = INPUT_KEYBOARD;
	key.ki.wScan = 0; // hardware scan code for key
	key.ki.time = 0;
	key.ki.dwExtraInfo = 0;
	
	// Press mute key
	key.ki.wVk = VK_VOLUME_MUTE;
	key.ki.dwFlags = 0; // press the key
	SendInput(1, &key, sizeof(INPUT));

	// Release the key
	key.ki.dwFlags = KEYEVENTF_KEYUP; 
	SendInput(1, &key, sizeof(INPUT));
	
	return 0;
}
