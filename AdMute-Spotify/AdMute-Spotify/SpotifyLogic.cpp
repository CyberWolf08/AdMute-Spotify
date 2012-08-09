#include "admute.h"



HWND GetSpotifyHandle()
{
	HWND SpotifyHandle;
	// Get the hwnd of the spotify window
	SpotifyHandle = FindWindow(L"SpotifyMainWindow", NULL);
	return SpotifyHandle;
}

int PlayPause(HWND SpotifyHandle)
{
	// send a message to spotify signaling a pause
	return PostMessage(SpotifyHandle, WM_APPCOMMAND, NULL , 0xE0000L);;
}

const wchar_t *GetNowPlaying(HWND SpotifyHandle)
{
	
	wchar_t *SpotifyTitle;
	int bufferSize;

	
	// Get the window title
	bufferSize = GetWindowTextLength(SpotifyHandle) + 1;
	SpotifyTitle = (wchar_t *) malloc(bufferSize * sizeof(wchar_t));
	GetWindowText(SpotifyHandle, SpotifyTitle, bufferSize);

	// Eliminate the "Spotify - " prefix
	SpotifyTitle = SpotifyTitle + 10;

	return SpotifyTitle;

}