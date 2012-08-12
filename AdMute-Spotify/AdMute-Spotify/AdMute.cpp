#include "admute.h"
#include <stdio.h>
#include <conio.h>

#define SETTINGSFILE "settings.ini"

#define DEBUG 1

void MuteAd();
int InitSettings(wchar_t AdList[20][256]);
int TrackIsAd(const wchar_t *Track, wchar_t AdList[20][256], int AdCount);// 0 false, 1 true
int WriteSettings(wchar_t *AdName);
#define RefreshTime 1000

int main(int argc, char **argv) {
	wchar_t AdList[20][256], *CurrentTrack;
	HWND SpotifyHandle;
	int isMuted = 0, AdCount = 0;
	char auxChar;
	
	// load the settings
	AdCount = InitSettings(AdList);

#if DEBUG == 1
	for(int i=0;i<AdCount&&AdCount>0; i++)
		printf("%ls \n", AdList[i]);
#endif

	CurrentTrack = (wchar_t *)malloc(256 * sizeof(wchar_t));

	// begin a infinite loop. This will be dealt with later
	while(1)
	{
		// at each iteration check if spotify is running
		SpotifyHandle = GetSpotifyHandle();
		Sleep(RefreshTime);
		if(SpotifyHandle == NULL)
		{
			printf("Spotify is closed. Retrying to find spotify in 5 seconds.\n");
			continue;
		}
		// see if the track has changed
		if(wcscmp(CurrentTrack, GetNowPlaying(SpotifyHandle))) {
			wcscpy(CurrentTrack, GetNowPlaying(SpotifyHandle));
			wprintf(L"\nTrack changed to %ls\n Is this track an ad? (press y to confirm) ", CurrentTrack);
		}

		if(kbhit()) {
			auxChar = _getch();
			if(auxChar == 'y' || auxChar == 'Y') {
				wprintf(L"\nTrack %ls added to the AdList\n", CurrentTrack);
				wcscpy(&AdList[AdCount][0], CurrentTrack);
				AdCount++;
				WriteSettings(CurrentTrack);
			}
		}

		// if track is an ad and volume is not muted, then mute the ad
		if(TrackIsAd(CurrentTrack, AdList, AdCount) && !isMuted)
		{
			MuteAd();
			isMuted = 1;
			printf("Track \"%ls\" is a ad, we will mute it\n", CurrentTrack);
			continue;
		}
		// if track is not an ad and volume is muted, unmute
		if(!TrackIsAd(CurrentTrack, AdList, AdCount) && isMuted)
		{
			printf("Track \"%ls\" is a not ad, we will unmute it\n", CurrentTrack);
			MasterVolumeMuteUnmute();
			isMuted = 0;
		}
	}

	getchar();
	return 0;
}

int InitSettings(wchar_t AdList[20][256]) {
	FILE *SettingsFILE;
	//try to open the settings file
	SettingsFILE = fopen(SETTINGSFILE, "rb, ccs=UNICODE");
	
	if(SettingsFILE == NULL) {
		puts("No settings file found. Carrying on with no AdList");
		return 0;
	}

	// there are no more than 20 ads usually 
	int i = 0;
	while (fgetws ( AdList[i], 512, SettingsFILE ) != NULL)
	{
		// remove the newline char
		AdList[i][wcslen(AdList[i]) - 1] = L'\0';
		i++;
	}
	fclose(SettingsFILE);
	return i;
}

int TrackIsAd(const wchar_t *Track, wchar_t AdList[20][256], int AdCount) {
	wchar_t *aux;

	for(int i = 0;i < AdCount; i++) {
		aux = &AdList[i][0];
		if(!wcscmp(Track, aux))
			return 1;
	}
	return 0;
}

int WriteSettings(wchar_t *AdName) {
	// open the settings file and append the ad to it.
	FILE *SettingsFILE = fopen(SETTINGSFILE, "ab, ccs=UNICODE");

	fwprintf(SettingsFILE, L"%ls\n", AdName);

	fclose(SettingsFILE);
	return 0;

}

void MuteAd() {
	MasterVolumeMuteUnmute();
	Sleep(500);
	PlayPause(GetSpotifyHandle());
}
